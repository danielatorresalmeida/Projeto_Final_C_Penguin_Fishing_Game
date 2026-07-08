#include "game.h"
#include "logic.h"
#include "render.h"
#include <ncurses.h>

#define TABULEIRO_LINHA 10
#define TABULEIRO_COLUNA 2

enum {
  COR_JOGADOR_1 = 1,
  COR_JOGADOR_2,
  COR_PEIXE_VERMELHO,
  COR_PEIXE_AMARELO,
  COR_PEIXE_NORMAL,
  COR_AGUA,
  COR_BORDA
};

static int coresAtivas = 0;

void inicializarInterface(void) {
  int fundo = COLOR_BLACK;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  if (!has_colors()) {
    coresAtivas = 0;
    return;
  }

  start_color();

  if (use_default_colors() == OK) {
    fundo = -1;
  }

  init_pair(COR_JOGADOR_1, COLOR_RED, fundo);
  init_pair(COR_JOGADOR_2, COLOR_YELLOW, fundo);
  init_pair(COR_PEIXE_VERMELHO, COLOR_RED, fundo);
  init_pair(COR_PEIXE_AMARELO, COLOR_YELLOW, fundo);
  init_pair(COR_PEIXE_NORMAL, COLOR_GREEN, fundo);
  init_pair(COR_AGUA, COLOR_CYAN, fundo);
  init_pair(COR_BORDA, COLOR_WHITE, fundo);

  coresAtivas = 1;
}

void finalizarInterface(void) { endwin(); }

static void desenharTextoComCor(int linha, int coluna, const char *texto,
                                int cor) {
  if (coresAtivas) {
    attron(COLOR_PAIR(cor) | A_BOLD);
  }

  mvprintw(linha, coluna, "%s", texto);

  if (coresAtivas) {
    attroff(COLOR_PAIR(cor) | A_BOLD);
  }
}

static void desenharCelula(int linha, int coluna, char simbolo) {
  int cor = 0;

  if (simbolo == '.') {
    cor = COR_AGUA;
  } else if (simbolo == 'R') {
    cor = COR_PEIXE_VERMELHO;
  } else if (simbolo == 'Y') {
    cor = COR_PEIXE_AMARELO;
  } else if (simbolo == 'F') {
    cor = COR_PEIXE_NORMAL;
  } else if (simbolo == 'H' || simbolo == 'B') {
    cor = COR_JOGADOR_1;
  } else if (simbolo == 'h' || simbolo == 'b') {
    cor = COR_JOGADOR_2;
  }

  if (coresAtivas && cor != 0) {
    attron(COLOR_PAIR(cor));
  }

  mvaddch(linha, coluna, simbolo);

  if (coresAtivas && cor != 0) {
    attroff(COLOR_PAIR(cor));
  }
}

static void desenharBordaTabuleiro(void) {
  int topo = TABULEIRO_LINHA - 1;
  int fundo = TABULEIRO_LINHA + LINHAS;
  int esquerda = TABULEIRO_COLUNA - 1;
  int direita = TABULEIRO_COLUNA + COLUNAS;

  if (coresAtivas) {
    attron(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }

  mvaddch(topo, esquerda, '+');
  mvaddch(topo, direita, '+');
  mvaddch(fundo, esquerda, '+');
  mvaddch(fundo, direita, '+');

  for (int coluna = TABULEIRO_COLUNA; coluna < TABULEIRO_COLUNA + COLUNAS;
       coluna++) {
    mvaddch(topo, coluna, '-');
    mvaddch(fundo, coluna, '-');
  }

  for (int linha = TABULEIRO_LINHA; linha < TABULEIRO_LINHA + LINHAS;
       linha++) {
    mvaddch(linha, esquerda, '|');
    mvaddch(linha, direita, '|');
  }

  if (coresAtivas) {
    attroff(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }
}

static void criarTabuleiro(char tabuleiro[LINHAS][COLUNAS]) {
  for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
      tabuleiro[i][j] = '.';
    }
  }
}

static int dentroDoTabuleiro(int linha, int coluna) {
  return linha >= 0 && linha < LINHAS && coluna >= 0 && coluna < COLUNAS;
}

static void colocarNoTabuleiro(char tabuleiro[LINHAS][COLUNAS], int linha,
                               int coluna, char simbolo) {
  if (dentroDoTabuleiro(linha, coluna)) {
    tabuleiro[linha][coluna] = simbolo;
  }
}

static void desenharControlosModo(const EstadoJogo *jogo) {
  if (jogo->jogadores == 2) {
    mvprintw(3, 0, "Controlos: PR = WASD | PY = setas | Q = sair");
  } else {
    mvprintw(3, 0, "Controlos: PR = WASD | Q = sair");
  }
}

static void desenharLegendaModo(const EstadoJogo *jogo) {
  if (jogo->modo == MAIS_PEIXES) {
    mvprintw(5, 0, "Objetivo: apanhar o maior numero de peixes.");
    mvprintw(8, 0, "R/Y = peixes especiais | F = peixe normal | H/h = anzol");
    return;
  }

  if (jogo->modo == MAIS_PESO) {
    mvprintw(5, 0, "Objetivo: apanhar peixes com maior peso total.");
    mvprintw(8, 0, "R/Y = peixes especiais | F = peixe normal | H/h = anzol");
    return;
  }

  if (jogo->modo == EMPILHAR) {
    mvprintw(5, 0, "Objetivo: empilhar o maior numero de peixes.");
    mvprintw(8, 0, "R/Y = peixes especiais | F = peixe normal | B/b = cesto");
  }
}

static void desenharPontuacaoModo(const EstadoJogo *jogo) {
  if (jogo->modo == MAIS_PEIXES) {
    desenharTextoComCor(6, 0, "PR", COR_JOGADOR_1);
    mvprintw(6, 3, "peixes=%d", jogo->p1.peixes);

    if (jogo->jogadores == 2) {
      desenharTextoComCor(7, 0, "PY", COR_JOGADOR_2);
      mvprintw(7, 3, "peixes=%d", jogo->p2.peixes);
    }

    return;
  }

  if (jogo->modo == MAIS_PESO) {
    desenharTextoComCor(6, 0, "PR", COR_JOGADOR_1);
    mvprintw(6, 3, "peso=%d", jogo->p1.peso);

    if (jogo->jogadores == 2) {
      desenharTextoComCor(7, 0, "PY", COR_JOGADOR_2);
      mvprintw(7, 3, "peso=%d", jogo->p2.peso);
    }

    return;
  }

  if (jogo->modo == EMPILHAR) {
    desenharTextoComCor(6, 0, "PR", COR_JOGADOR_1);
    mvprintw(6, 3, "empilhados=%d", jogo->p1.empilhados);

    if (jogo->jogadores == 2) {
      desenharTextoComCor(7, 0, "PY", COR_JOGADOR_2);
      mvprintw(7, 3, "empilhados=%d", jogo->p2.empilhados);
    }
  }
}

void desenharJogo(const EstadoJogo *jogo) {
  char tabuleiro[LINHAS][COLUNAS];
  int linhaCaptura;
  int colunaCaptura;

  criarTabuleiro(tabuleiro);

  if (jogo->peixe.ativo) {
    colocarNoTabuleiro(tabuleiro, jogo->peixe.linha, jogo->peixe.coluna,
                       jogo->peixe.simbolo);
  }

  obterZonaCaptura(&jogo->p1, jogo->modo, 1, &linhaCaptura, &colunaCaptura);

  if (jogo->modo == EMPILHAR) {
    colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'B');
  } else {
    colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'H');
  }

  if (jogo->jogadores == 2) {
    obterZonaCaptura(&jogo->p2, jogo->modo, 2, &linhaCaptura, &colunaCaptura);

    if (jogo->modo == EMPILHAR) {
      colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'b');
    } else {
      colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'h');
    }
  }

  erase();

  mvprintw(0, 0, "Penguin Fishing Game");
  mvprintw(1, 0, "Modo: %s", nomeModo(jogo->modo));

  if (jogo->tempo == 9999) {
    mvprintw(2, 0, "Tempo: pratica infinita");
  } else {
    mvprintw(2, 0, "Tempo: %d", jogo->tempo);
  }

  desenharControlosModo(jogo);
  desenharLegendaModo(jogo);
  desenharPontuacaoModo(jogo);

  desenharBordaTabuleiro();

  for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
      desenharCelula(TABULEIRO_LINHA + i, TABULEIRO_COLUNA + j,
                     tabuleiro[i][j]);
    }
  }

  desenharTextoComCor(TABULEIRO_LINHA + jogo->p1.linha,
                      TABULEIRO_COLUNA + jogo->p1.coluna, "PR", COR_JOGADOR_1);

  if (jogo->jogadores == 2) {
    desenharTextoComCor(TABULEIRO_LINHA + jogo->p2.linha,
                        TABULEIRO_COLUNA + jogo->p2.coluna, "PY",
                        COR_JOGADOR_2);
  }

  refresh();
}
