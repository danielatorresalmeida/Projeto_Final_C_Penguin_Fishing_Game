#include "game.h"
#include "logic.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void obterZonaCaptura(const Jogador *jogador, ModoJogo modo,
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

void atualizarJogo(EstadoJogo *jogo, int tecla, int *contadorMovimentoPeixe,
                   time_t *ultimoSegundo) {
  moverJogador1(&jogo->p1, tecla);

  if (jogo->jogadores == 2) {
    moverJogador2(&jogo->p2, tecla);
  }

  (*contadorMovimentoPeixe)++;

  if (*contadorMovimentoPeixe >= 5) {
    moverPeixe(&jogo->peixe);
    *contadorMovimentoPeixe = 0;
  }

  verificarCapturas(jogo);

  if (jogo->tempo != 9999 && time(NULL) != *ultimoSegundo) {
    jogo->tempo--;
    *ultimoSegundo = time(NULL);
  }
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
