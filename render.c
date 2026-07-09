#include "render.h"
#include "game.h"
#include "logic.h"
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <string.h>

#define TABULEIRO_LINHA 7
#define TABULEIRO_COLUNA 27

#define PAINEL_ESQUERDO_LINHA 3
#define PAINEL_ESQUERDO_COLUNA 0
#define PAINEL_ESQUERDO_LARGURA 24
#define PAINEL_ESQUERDO_ALTURA 10

#define PAINEL_DIREITO_LINHA 3
#define PAINEL_DIREITO_COLUNA 72
#define PAINEL_DIREITO_LARGURA 30
#define PAINEL_DIREITO_ALTURA 18

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
  leaveok(stdscr, TRUE);
  set_escdelay(25);
  curs_set(0);
  clear();
  refresh();

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

static void desenharTitulo(int linha, int coluna, const char *texto) {
  if (coresAtivas) {
    attron(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }

  mvprintw(linha, coluna, "%s", texto);

  if (coresAtivas) {
    attroff(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }
}

static void desenharCaixa(int linha, int coluna, int altura, int largura,
                          const char *titulo) {
  int fimLinha = linha + altura - 1;
  int fimColuna = coluna + largura - 1;

  if (coresAtivas) {
    attron(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }

  mvaddch(linha, coluna, '+');
  mvaddch(linha, fimColuna, '+');
  mvaddch(fimLinha, coluna, '+');
  mvaddch(fimLinha, fimColuna, '+');

  for (int x = coluna + 1; x < fimColuna; x++) {
    mvaddch(linha, x, '-');
    mvaddch(fimLinha, x, '-');
  }

  for (int y = linha + 1; y < fimLinha; y++) {
    mvaddch(y, coluna, '|');
    mvaddch(y, fimColuna, '|');
  }

  if (coresAtivas) {
    attroff(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }

  if (titulo != NULL && titulo[0] != '\0') {
    mvprintw(linha, coluna + 2, " %s ", titulo);
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

static int pontuacaoJogador(const Jogador *jogador, ModoJogo modo) {
  if (modo == MAIS_PEIXES) {
    return jogador->peixes;
  }

  if (modo == MAIS_PESO) {
    return jogador->peso;
  }

  if (modo == EMPILHAR) {
    return jogador->empilhados;
  }

  return 0;
}

static const char *textoPontuacao(ModoJogo modo, int valor) {
  if (modo == MAIS_PEIXES) {
    if (valor == 1) {
      return "peixe";
    }

    return "peixes";
  }

  if (modo == MAIS_PESO) {
    return "peso";
  }

  if (modo == EMPILHAR) {
    if (valor == 1) {
      return "empilhado";
    }

    return "empilhados";
  }

  if (valor == 1) {
    return "ponto";
  }

  return "pontos";
}

static int peixeDaCorDoJogador(const Peixe *peixe, int numeroJogador) {
  // cor 1 pertence ao PR, cor 2 pertence ao PY e cor 0 e neutra.
  if (numeroJogador == 1) {
    return peixe->cor == 1;
  }

  return peixe->cor == 2;
}

static int ganhoPeixeAtual(const EstadoJogo *jogo, int numeroJogador) {
  const Peixe *peixe = &jogo->peixe;

  if (!peixe->ativo) {
    return 0;
  }

  if (jogo->modo == MAIS_PEIXES) {
    if (peixeDaCorDoJogador(peixe, numeroJogador)) {
      return 2;
    }

    return 1;
  }

  if (jogo->modo == MAIS_PESO) {
    if (peixeDaCorDoJogador(peixe, numeroJogador)) {
      return peixe->peso + 2;
    }

    return peixe->peso;
  }

  if (jogo->modo == EMPILHAR) {
    if (peixeDaCorDoJogador(peixe, numeroJogador)) {
      return 2;
    }

    return 1;
  }

  return 0;
}

static int corPeixeVisual(char simbolo) {
  if (simbolo == 'R') {
    return COR_PEIXE_VERMELHO;
  }

  if (simbolo == 'Y') {
    return COR_PEIXE_AMARELO;
  }

  return COR_PEIXE_NORMAL;
}

static const char *objetivoCurto(ModoJogo modo) {
  if (modo == MAIS_PEIXES) {
    return "Maior numero";
  }

  if (modo == MAIS_PESO) {
    return "Maior peso";
  }

  if (modo == EMPILHAR) {
    return "Maior pilha";
  }

  return "Pontuar";
}

static int contarDigitos(int valor) {
  int digitos = 1;

  if (valor < 0) {
    valor = -valor;
    digitos++;
  }

  while (valor >= 10) {
    valor /= 10;
    digitos++;
  }

  return digitos;
}

static int larguraPontuacaoJogador(const Jogador *jogador, ModoJogo modo) {
  int pontos = pontuacaoJogador(jogador, modo);

  return 2 + 1 + contarDigitos(pontos) + 1 +
         (int)strlen(textoPontuacao(modo, pontos));
}

static void desenharTempo(const EstadoJogo *jogo) {
  char texto[30];
  int linha = TABULEIRO_LINHA - 1;
  int esquerdaTabuleiro = TABULEIRO_COLUNA - 1;
  int larguraTabuleiro = COLUNAS + 2;
  int coluna;

  if (jogo->tempo == 9999) {
    snprintf(texto, sizeof(texto), "[PRATICA]");
  } else {
    snprintf(texto, sizeof(texto), "[TEMPO: %2d s]", jogo->tempo);
  }

  coluna = esquerdaTabuleiro + (larguraTabuleiro - (int)strlen(texto)) / 2;

  if (jogo->tempo == 9999) {
    if (coresAtivas) {
      attron(COLOR_PAIR(COR_BORDA) | A_BOLD);
    }

    mvprintw(linha, coluna, "%s", texto);

    if (coresAtivas) {
      attroff(COLOR_PAIR(COR_BORDA) | A_BOLD);
    }

    return;
  }

  if (jogo->tempo <= 10) {
    if (coresAtivas) {
      attron(COLOR_PAIR(COR_PEIXE_VERMELHO) | A_BOLD);
    }

    mvprintw(linha, coluna, "%s", texto);

    if (coresAtivas) {
      attroff(COLOR_PAIR(COR_PEIXE_VERMELHO) | A_BOLD);
    }

    return;
  }

  if (coresAtivas) {
    attron(COLOR_PAIR(COR_PEIXE_AMARELO) | A_BOLD);
  }

  mvprintw(linha, coluna, "%s", texto);

  if (coresAtivas) {
    attroff(COLOR_PAIR(COR_PEIXE_AMARELO) | A_BOLD);
  }
}

static void desenharPontuacaoTopo(const EstadoJogo *jogo) {
  int linha = TABULEIRO_LINHA - 3;
  int esquerdaTabuleiro = TABULEIRO_COLUNA - 1;
  int larguraTabuleiro = COLUNAS + 2;
  int larguraTotal;
  int coluna;
  int pontosP1 = pontuacaoJogador(&jogo->p1, jogo->modo);
  const char *tipoP1 = textoPontuacao(jogo->modo, pontosP1);

  larguraTotal = larguraPontuacaoJogador(&jogo->p1, jogo->modo);

  if (jogo->jogadores == 2) {
    larguraTotal += 3 + larguraPontuacaoJogador(&jogo->p2, jogo->modo);
  }

  coluna = esquerdaTabuleiro + (larguraTabuleiro - larguraTotal) / 2;

  if (coluna < esquerdaTabuleiro) {
    coluna = esquerdaTabuleiro;
  }

  desenharTextoComCor(linha, coluna, "PR", COR_JOGADOR_1);
  coluna += 2;

  mvprintw(linha, coluna, " %d %s", pontosP1, tipoP1);
  coluna += 1 + contarDigitos(pontosP1) + 1 + (int)strlen(tipoP1);

  if (jogo->jogadores == 2) {
    int pontosP2 = pontuacaoJogador(&jogo->p2, jogo->modo);
    const char *tipoP2 = textoPontuacao(jogo->modo, pontosP2);

    mvprintw(linha, coluna, " | ");
    coluna += 3;

    desenharTextoComCor(linha, coluna, "PY", COR_JOGADOR_2);
    coluna += 2;

    mvprintw(linha, coluna, " %d %s", pontosP2, tipoP2);
  }
}

static void desenharCabecalho(const EstadoJogo *jogo) {
  desenharTitulo(0, 0, "PENGUIN FISHING GAME");
  mvprintw(1, 0, "Modo: %s", nomeModo(jogo->modo));
}

static void desenharPainelObjetivo(const EstadoJogo *jogo) {
  desenharCaixa(PAINEL_ESQUERDO_LINHA, PAINEL_ESQUERDO_COLUNA,
                PAINEL_ESQUERDO_ALTURA, PAINEL_ESQUERDO_LARGURA, "OBJETIVO");

  mvprintw(PAINEL_ESQUERDO_LINHA + 2, PAINEL_ESQUERDO_COLUNA + 2, "%s",
           objetivoCurto(jogo->modo));

  if (jogo->modo == MAIS_PEIXES) {
    mvprintw(PAINEL_ESQUERDO_LINHA + 3, PAINEL_ESQUERDO_COLUNA + 2,
             "mais peixes");
    desenharTitulo(PAINEL_ESQUERDO_LINHA + 5, PAINEL_ESQUERDO_COLUNA + 2,
                   "REGRA");
    mvprintw(PAINEL_ESQUERDO_LINHA + 6, PAINEL_ESQUERDO_COLUNA + 2,
             "cor propria +2");
    mvprintw(PAINEL_ESQUERDO_LINHA + 7, PAINEL_ESQUERDO_COLUNA + 2,
             "outros +1");
  } else if (jogo->modo == MAIS_PESO) {
    mvprintw(PAINEL_ESQUERDO_LINHA + 3, PAINEL_ESQUERDO_COLUNA + 2,
             "peso total");
    desenharTitulo(PAINEL_ESQUERDO_LINHA + 5, PAINEL_ESQUERDO_COLUNA + 2,
                   "REGRA");
    mvprintw(PAINEL_ESQUERDO_LINHA + 6, PAINEL_ESQUERDO_COLUNA + 2,
             "peso + bonus");
    mvprintw(PAINEL_ESQUERDO_LINHA + 7, PAINEL_ESQUERDO_COLUNA + 2,
             "cor propria +2");
  } else {
    mvprintw(PAINEL_ESQUERDO_LINHA + 3, PAINEL_ESQUERDO_COLUNA + 2,
             "maior pilha");
    desenharTitulo(PAINEL_ESQUERDO_LINHA + 5, PAINEL_ESQUERDO_COLUNA + 2,
                   "REGRA");
    mvprintw(PAINEL_ESQUERDO_LINHA + 6, PAINEL_ESQUERDO_COLUNA + 2,
             "cor propria +2");
    mvprintw(PAINEL_ESQUERDO_LINHA + 7, PAINEL_ESQUERDO_COLUNA + 2,
             "outros +1");
  }

  if (jogo->jogadores == 2) {
    mvprintw(PAINEL_ESQUERDO_LINHA + 8, PAINEL_ESQUERDO_COLUNA + 2,
             "bloqueio ativo");
  }
}

static void desenharPeixeAtualTopo(const EstadoJogo *jogo) {
  char detalhe[70];
  char simbolo[2];
  const char *prefixo = "PEIXE: ";
  int linha = TABULEIRO_LINHA - 2;
  int esquerdaTabuleiro = TABULEIRO_COLUNA - 1;
  int larguraTabuleiro = COLUNAS + 2;
  int ganhoP1;
  int ganhoP2;
  int larguraTotal;
  int coluna;

  // O peixe atual fica perto do tabuleiro, onde o jogador olha mais.
  if (!jogo->peixe.ativo) {
    const char *texto = "PEIXE: sem peixe";
    coluna = esquerdaTabuleiro + (larguraTabuleiro - (int)strlen(texto)) / 2;
    mvprintw(linha, coluna, "%s", texto);
    return;
  }

  simbolo[0] = jogo->peixe.simbolo;
  simbolo[1] = '\0';

  ganhoP1 = ganhoPeixeAtual(jogo, 1);

  if (jogo->jogadores == 2) {
    ganhoP2 = ganhoPeixeAtual(jogo, 2);

    if (jogo->modo == MAIS_PESO) {
      snprintf(detalhe, sizeof(detalhe), " peso %d | PR +%d | PY +%d",
               jogo->peixe.peso, ganhoP1, ganhoP2);
    } else if (jogo->modo == MAIS_PEIXES) {
      snprintf(detalhe, sizeof(detalhe), " | PR +%d | PY +%d", ganhoP1,
               ganhoP2);
    } else {
      snprintf(detalhe, sizeof(detalhe), " pilha | PR +%d | PY +%d", ganhoP1,
               ganhoP2);
    }
  } else {
    if (jogo->modo == MAIS_PESO) {
      snprintf(detalhe, sizeof(detalhe), " peso %d | PR +%d", jogo->peixe.peso,
               ganhoP1);
    } else if (jogo->modo == MAIS_PEIXES) {
      snprintf(detalhe, sizeof(detalhe), " | PR +%d", ganhoP1);
    } else {
      snprintf(detalhe, sizeof(detalhe), " pilha | PR +%d", ganhoP1);
    }
  }

  larguraTotal = (int)strlen(prefixo) + 1 + (int)strlen(detalhe);
  coluna = esquerdaTabuleiro + (larguraTabuleiro - larguraTotal) / 2;

  if (coluna < esquerdaTabuleiro) {
    coluna = esquerdaTabuleiro;
  }

  mvprintw(linha, coluna, "%s", prefixo);
  coluna += (int)strlen(prefixo);

  desenharTextoComCor(linha, coluna, simbolo,
                      corPeixeVisual(jogo->peixe.simbolo));
  coluna += 1;

  mvprintw(linha, coluna, "%s", detalhe);
}

static void desenharPainelDireito(const EstadoJogo *jogo) {
  desenharCaixa(PAINEL_DIREITO_LINHA, PAINEL_DIREITO_COLUNA,
                PAINEL_DIREITO_ALTURA, PAINEL_DIREITO_LARGURA, "INFO");

  desenharTitulo(PAINEL_DIREITO_LINHA + 2, PAINEL_DIREITO_COLUNA + 2,
                 "LEGENDA");

  desenharTextoComCor(PAINEL_DIREITO_LINHA + 4, PAINEL_DIREITO_COLUNA + 2, "R",
                      COR_PEIXE_VERMELHO);
  mvprintw(PAINEL_DIREITO_LINHA + 4, PAINEL_DIREITO_COLUNA + 6,
           "peixe vermelho");

  desenharTextoComCor(PAINEL_DIREITO_LINHA + 5, PAINEL_DIREITO_COLUNA + 2, "Y",
                      COR_PEIXE_AMARELO);
  mvprintw(PAINEL_DIREITO_LINHA + 5, PAINEL_DIREITO_COLUNA + 6,
           "peixe amarelo");

  desenharTextoComCor(PAINEL_DIREITO_LINHA + 6, PAINEL_DIREITO_COLUNA + 2, "F",
                      COR_PEIXE_NORMAL);
  mvprintw(PAINEL_DIREITO_LINHA + 6, PAINEL_DIREITO_COLUNA + 6, "peixe neutro");

  if (jogo->modo == EMPILHAR) {
    mvprintw(PAINEL_DIREITO_LINHA + 8, PAINEL_DIREITO_COLUNA + 2, "B/b  cesto");
  } else {
    mvprintw(PAINEL_DIREITO_LINHA + 8, PAINEL_DIREITO_COLUNA + 2, "H/h  anzol");
  }

  desenharTitulo(PAINEL_DIREITO_LINHA + 11, PAINEL_DIREITO_COLUNA + 2,
                 "CONTROLOS");

  desenharTextoComCor(PAINEL_DIREITO_LINHA + 13, PAINEL_DIREITO_COLUNA + 2,
                      "PR", COR_JOGADOR_1);
  mvprintw(PAINEL_DIREITO_LINHA + 13, PAINEL_DIREITO_COLUNA + 6, "WASD");

  if (jogo->jogadores == 2) {
    desenharTextoComCor(PAINEL_DIREITO_LINHA + 14, PAINEL_DIREITO_COLUNA + 2,
                        "PY", COR_JOGADOR_2);
    mvprintw(PAINEL_DIREITO_LINHA + 14, PAINEL_DIREITO_COLUNA + 6, "setas");
  }

  mvprintw(PAINEL_DIREITO_LINHA + 16, PAINEL_DIREITO_COLUNA + 2,
           "P pausa | Q sair");
}

static void desenharRodape(void) {
  mvprintw(TABULEIRO_LINHA + LINHAS + 4, TABULEIRO_COLUNA - 1,
           "P para pausar | Q para sair | ultimos 10 segundos em vermelho");
}

void desenharPausa(void) {
  int largura = 34;
  int altura = 5;
  int linha = TABULEIRO_LINHA + (LINHAS / 2) - 2;
  int coluna = TABULEIRO_COLUNA + (COLUNAS - largura) / 2;
  const char *texto1 = "JOGO PAUSADO";
  const char *texto2 = "P continuar | Q sair";

  // Caixa simples por cima do tabuleiro.
  desenharCaixa(linha, coluna, altura, largura, "PAUSA");

  if (coresAtivas) {
    attron(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }

  mvprintw(linha + 2, coluna + (largura - (int)strlen(texto1)) / 2, "%s",
           texto1);
  mvprintw(linha + 3, coluna + (largura - (int)strlen(texto2)) / 2, "%s",
           texto2);

  if (coresAtivas) {
    attroff(COLOR_PAIR(COR_BORDA) | A_BOLD);
  }

  refresh();
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

  desenharCabecalho(jogo);
  desenharPainelObjetivo(jogo);
  desenharPainelDireito(jogo);

  desenharPontuacaoTopo(jogo);
  desenharPeixeAtualTopo(jogo);
  desenharBordaTabuleiro();
  desenharTempo(jogo);

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

  desenharRodape();

  refresh();
}
