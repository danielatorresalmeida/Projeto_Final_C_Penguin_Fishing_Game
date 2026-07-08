#include "game.h"
#include <ctype.h>
#include <errno.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void mostrarMenu(void) {
  printf("\n==============================\n");
  printf("     PENGUIN FISHING GAME\n");
  printf("==============================\n");
  printf("1 - Solo: mais peixes\n");
  printf("2 - Solo: mais peso\n");
  printf("3 - Solo: empilhar peixes\n");
  printf("4 - 2 jogadores: mais peixes\n");
  printf("5 - 2 jogadores: mais peso\n");
  printf("6 - 2 jogadores: empilhar peixes\n");
  printf("7 - Pratica: mais peixes\n");
  printf("8 - Pratica: mais peso\n");
  printf("9 - Pratica: empilhar peixes\n");
  printf("10 - Mudar idioma\n");
  printf("0 - Sair\n");
  printf("Escolhe uma opcao: ");
}

static void limparRestoLinha(void) {
  int c;

  while ((c = getchar()) != '\n' && c != EOF) {
    // Remove caracteres que ficaram no buffer.
  }
}

int lerOpcao(void) {
  char linha[64];
  char *fim;
  long valor;

  if (fgets(linha, sizeof(linha), stdin) == NULL) {
    return OPCAO_INVALIDA;
  }

  if (strchr(linha, '\n') == NULL) {
    limparRestoLinha();
  }

  linha[strcspn(linha, "\n")] = '\0';

  errno = 0;
  valor = strtol(linha, &fim, 10);

  while (isspace((unsigned char)*fim)) {
    fim++;
  }

  if (fim == linha || *fim != '\0' || errno == ERANGE) {
    return OPCAO_INVALIDA;
  }

  if (valor < OPCAO_MINIMA || valor > OPCAO_MAXIMA) {
    return OPCAO_INVALIDA;
  }

  return (int)valor;
}

const char *nomeModo(ModoJogo modo) {
  if (modo == MAIS_PEIXES) {
    return "Mais Peixes";
  }

  if (modo == MAIS_PESO) {
    return "Mais Peso";
  }

  if (modo == EMPILHAR) {
    return "Empilhar Peixes";
  }

  return "Menu";
}

static void iniciarCores(void) {
  int fundo = COLOR_BLACK;

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

  for (int linha = TABULEIRO_LINHA; linha < TABULEIRO_LINHA + LINHAS; linha++) {
    mvaddch(linha, esquerda, '|');
    mvaddch(linha, direita, '|');
  }

  if (coresAtivas) {
    attroff(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }
}

static void prepararJogador(Jogador *jogador, const char *nome, char simbolo,
                            int linha, int coluna) {
  strcpy(jogador->nome, nome);
  jogador->simbolo = simbolo;
  jogador->linha = linha;
  jogador->coluna = coluna;
  jogador->peixes = 0;
  jogador->peso = 0;
  jogador->empilhados = 0;
}

static int posicaoOcupada(const EstadoJogo *jogo, int linha, int coluna) {
  if (linha == jogo->p1.linha && coluna == jogo->p1.coluna) {
    return 1;
  }

  if (linha == jogo->p1.linha && coluna == jogo->p1.coluna + 1) {
    return 1;
  }

  if (jogo->jogadores == 2) {
    if (linha == jogo->p2.linha && coluna == jogo->p2.coluna) {
      return 1;
    }

    if (linha == jogo->p2.linha && coluna == jogo->p2.coluna + 1) {
      return 1;
    }
  }

  return 0;
}

static void definirTipoPeixe(Peixe *peixe) {
  peixe->cor = rand() % 3;
  peixe->tipo = 1 + rand() % 3;
  peixe->ativo = 1;

  if (peixe->tipo == 1) {
    peixe->peso = 1;
  } else if (peixe->tipo == 2) {
    peixe->peso = 3;
  } else {
    peixe->peso = 5;
  }

  if (peixe->cor == 1) {
    peixe->simbolo = 'R';
  } else if (peixe->cor == 2) {
    peixe->simbolo = 'Y';
  } else {
    peixe->simbolo = 'F';
  }
}

static void gerarPeixe(EstadoJogo *jogo) {
  int tentativas = 0;

  definirTipoPeixe(&jogo->peixe);

  do {
    jogo->peixe.linha = 2 + rand() % (LINHAS - 4);
    jogo->peixe.coluna = 2 + rand() % (COLUNAS - 4);
    tentativas++;
  } while (posicaoOcupada(jogo, jogo->peixe.linha, jogo->peixe.coluna) &&
           tentativas < 100);
}

void iniciarJogo(EstadoJogo *jogo, int opcao) {
  jogo->ativo = 1;

  if ((opcao >= 1 && opcao <= 3) || (opcao >= 7 && opcao <= 9)) {
    jogo->jogadores = 1;
  } else {
    jogo->jogadores = 2;
  }

  if (opcao == 1 || opcao == 4 || opcao == 7) {
    jogo->modo = MAIS_PEIXES;
    jogo->tempo = 60;
  } else if (opcao == 2 || opcao == 5 || opcao == 8) {
    jogo->modo = MAIS_PESO;
    jogo->tempo = 45;
  } else {
    jogo->modo = EMPILHAR;
    jogo->tempo = 60;
  }

  if (opcao >= 7 && opcao <= 9) {
    jogo->tempo = 9999;
  }

  prepararJogador(&jogo->p1, "Pinguim Vermelho", 'P', LINHAS - 2, 5);
  prepararJogador(&jogo->p2, "Pinguim Amarelo", 'Y', LINHAS - 2, COLUNAS - 6);

  gerarPeixe(jogo);
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

static void obterZonaCaptura(const Jogador *jogador, ModoJogo modo,
                             int numeroJogador, int *linha, int *coluna) {
  if (modo == EMPILHAR) {
    *linha = jogador->linha - 1;
    *coluna = jogador->coluna;
    return;
  }

  *linha = jogador->linha;

  if (numeroJogador == 1) {
    *coluna = jogador->coluna + 2;
  } else {
    *coluna = jogador->coluna - 1;
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

static void desenharJogo(const EstadoJogo *jogo) {
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

static void moverJogador1(Jogador *jogador, int tecla) {
  if ((tecla == 'w' || tecla == 'W') && jogador->linha > 1) {
    jogador->linha--;
  } else if ((tecla == 's' || tecla == 'S') && jogador->linha < LINHAS - 1) {
    jogador->linha++;
  } else if ((tecla == 'a' || tecla == 'A') && jogador->coluna > 1) {
    jogador->coluna--;
  } else if ((tecla == 'd' || tecla == 'D') && jogador->coluna < COLUNAS - 3) {
    jogador->coluna++;
  }
}

static void moverJogador2(Jogador *jogador, int tecla) {
  if (tecla == KEY_UP && jogador->linha > 1) {
    jogador->linha--;
  } else if (tecla == KEY_DOWN && jogador->linha < LINHAS - 1) {
    jogador->linha++;
  } else if (tecla == KEY_LEFT && jogador->coluna > 1) {
    jogador->coluna--;
  } else if (tecla == KEY_RIGHT && jogador->coluna < COLUNAS - 3) {
    jogador->coluna++;
  }
}

static void moverPeixe(Peixe *peixe) {
  int direcao = rand() % 4;

  if (direcao == 0 && peixe->linha > 1) {
    peixe->linha--;
  } else if (direcao == 1 && peixe->linha < LINHAS - 2) {
    peixe->linha++;
  } else if (direcao == 2 && peixe->coluna > 1) {
    peixe->coluna--;
  } else if (direcao == 3 && peixe->coluna < COLUNAS - 2) {
    peixe->coluna++;
  }
}

static void aplicarPontuacao(EstadoJogo *jogo, int numeroJogador) {
  Peixe *peixe = &jogo->peixe;

  if (jogo->modo == MAIS_PEIXES) {
    if (numeroJogador == 1) {
      if (peixe->cor == 1) {
        jogo->p1.peixes += 2;
      } else if (peixe->cor == 2 && jogo->jogadores == 2) {
        jogo->p2.peixes += 1;
      } else {
        jogo->p1.peixes += 1;
      }
    } else {
      if (peixe->cor == 2) {
        jogo->p2.peixes += 2;
      } else if (peixe->cor == 1) {
        jogo->p1.peixes += 1;
      } else {
        jogo->p2.peixes += 1;
      }
    }
  }

  if (jogo->modo == MAIS_PESO) {
    if (numeroJogador == 1) {
      if (peixe->cor == 1) {
        jogo->p1.peso += peixe->peso + 2;
      } else if (peixe->cor == 2 && jogo->jogadores == 2) {
        jogo->p2.peso += peixe->peso;
      } else {
        jogo->p1.peso += peixe->peso;
      }
    } else {
      if (peixe->cor == 2) {
        jogo->p2.peso += peixe->peso + 2;
      } else if (peixe->cor == 1) {
        jogo->p1.peso += peixe->peso;
      } else {
        jogo->p2.peso += peixe->peso;
      }
    }
  }

  if (jogo->modo == EMPILHAR) {
    if (numeroJogador == 1) {
      if (peixe->cor == 2) {
        if (jogo->p1.empilhados > 0) {
          jogo->p1.empilhados--;
        }
      } else {
        jogo->p1.empilhados++;
      }
    } else {
      if (peixe->cor == 1) {
        if (jogo->p2.empilhados > 0) {
          jogo->p2.empilhados--;
        }
      } else {
        jogo->p2.empilhados++;
      }
    }
  }
}

static int jogadorCapturou(const Jogador *jogador, const Peixe *peixe,
                           ModoJogo modo, int numeroJogador) {
  int linhaCaptura;
  int colunaCaptura;

  obterZonaCaptura(jogador, modo, numeroJogador, &linhaCaptura, &colunaCaptura);

  return peixe->ativo && peixe->linha == linhaCaptura &&
         peixe->coluna == colunaCaptura;
}

static void verificarCapturas(EstadoJogo *jogo) {
  if (jogadorCapturou(&jogo->p1, &jogo->peixe, jogo->modo, 1)) {
    aplicarPontuacao(jogo, 1);
    gerarPeixe(jogo);
    return;
  }

  if (jogo->jogadores == 2 &&
      jogadorCapturou(&jogo->p2, &jogo->peixe, jogo->modo, 2)) {
    aplicarPontuacao(jogo, 2);
    gerarPeixe(jogo);
  }
}

void cicloJogo(EstadoJogo *jogo) {
  int tecla;
  int contadorMovimentoPeixe = 0;
  time_t ultimoSegundo = time(NULL);

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
  iniciarCores();

  while (jogo->ativo && jogo->tempo > 0) {
    desenharJogo(jogo);

    tecla = getch();

    if (tecla == 'q' || tecla == 'Q') {
      jogo->ativo = 0;
      break;
    }

    moverJogador1(&jogo->p1, tecla);

    if (jogo->jogadores == 2) {
      moverJogador2(&jogo->p2, tecla);
    }

    contadorMovimentoPeixe++;

    if (contadorMovimentoPeixe >= 5) {
      moverPeixe(&jogo->peixe);
      contadorMovimentoPeixe = 0;
    }

    verificarCapturas(jogo);

    if (jogo->tempo != 9999 && time(NULL) != ultimoSegundo) {
      jogo->tempo--;
      ultimoSegundo = time(NULL);
    }

    napms(80);
  }

  endwin();

  jogo->ativo = 0;
}

void mostrarResultado(const EstadoJogo *jogo) {
  printf("\n==============================\n");
  printf("          FIM DO JOGO\n");
  printf("==============================\n");

  if (jogo->modo == MAIS_PEIXES) {
    printf("Resultado por peixes:\n");
    printf("P1: %d peixes\n", jogo->p1.peixes);

    if (jogo->jogadores == 2) {
      printf("P2: %d peixes\n", jogo->p2.peixes);
    }
  }

  if (jogo->modo == MAIS_PESO) {
    printf("Resultado por peso:\n");
    printf("P1: %d peso\n", jogo->p1.peso);

    if (jogo->jogadores == 2) {
      printf("P2: %d peso\n", jogo->p2.peso);
    }
  }

  if (jogo->modo == EMPILHAR) {
    printf("Resultado por peixes empilhados:\n");
    printf("P1: %d empilhados\n", jogo->p1.empilhados);

    if (jogo->jogadores == 2) {
      printf("P2: %d empilhados\n", jogo->p2.empilhados);
    }
  }
}
