#include "logic.h"
#include "game.h"
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COR_VERMELHO "\033[1;31m"
#define COR_AMARELO "\033[1;33m"
#define COR_RESET "\033[0m"

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

  // Evita gerar um peixe em cima da animação de captura.
  if (jogo->animacaoCaptura > 0 && linha == jogo->animacaoLinha &&
      coluna == jogo->animacaoColuna) {
    return 1;
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

  jogo->animacaoCaptura = 0;
  jogo->animacaoLinha = 0;
  jogo->animacaoColuna = 0;
  jogo->animacaoPontos = 0;
  jogo->animacaoJogador = 0;

  gerarPeixe(jogo);
}

void obterZonaCaptura(const Jogador *jogador, ModoJogo modo, int numeroJogador,
                      int *linha, int *coluna) {
  if (modo == EMPILHAR) {
    *linha = jogador->linha - 1;
    *coluna = jogador->coluna;
    return;
  }

  *linha = jogador->linha;

  if (numeroJogador == 1) {
    // O anzol do PR ocupa as posições -< à direita do jogador.
    *coluna = jogador->coluna + 2;
  } else {
    // O anzol do PY ocupa as posições >- à esquerda do jogador.
    *coluna = jogador->coluna - 2;
  }
}

static int jogadorOcupaCelula(const Jogador *jogador, int linha, int coluna) {
  return linha == jogador->linha &&
         (coluna == jogador->coluna || coluna == jogador->coluna + 1);
}

static int movimentoDentroDoTabuleiro(const EstadoJogo *jogo, int numeroJogador,
                                      int linha, int coluna) {
  if (linha < 1 || linha >= LINHAS) {
    return 0;
  }

  if (jogo->modo == EMPILHAR) {
    return coluna >= 1 && coluna < COLUNAS - 2;
  }

  // Nos modos com anzol, deixa espaço para desenhar -< e >-.
  if (numeroJogador == 1) {
    return coluna >= 1 && coluna <= COLUNAS - 4;
  }

  return coluna >= 2 && coluna < COLUNAS - 2;
}

static int movimentoBloqueadoPeloOutroJogador(const EstadoJogo *jogo,
                                              int numeroJogador, int linha,
                                              int coluna) {
  const Jogador *outroJogador;

  if (jogo->jogadores != 2) {
    return 0;
  }

  if (numeroJogador == 1) {
    outroJogador = &jogo->p2;
  } else {
    outroJogador = &jogo->p1;
  }

  // Cada pinguim ocupa duas colunas: PR ou PY.
  // Se a nova posição tocar numa célula ocupada pelo outro, o movimento é bloqueado.
  return jogadorOcupaCelula(outroJogador, linha, coluna) ||
         jogadorOcupaCelula(outroJogador, linha, coluna + 1);
}

static int tentarMoverJogador(EstadoJogo *jogo, Jogador *jogador,
                              int numeroJogador, int novaLinha,
                              int novaColuna) {
  if (!movimentoDentroDoTabuleiro(jogo, numeroJogador, novaLinha, novaColuna)) {
    return 0;
  }

  if (movimentoBloqueadoPeloOutroJogador(jogo, numeroJogador, novaLinha,
                                         novaColuna)) {
    return 0;
  }

  jogador->linha = novaLinha;
  jogador->coluna = novaColuna;
  return 1;
}

static int moverJogador1(EstadoJogo *jogo, int tecla) {
  Jogador *jogador = &jogo->p1;

  if (tecla == 'w' || tecla == 'W') {
    return tentarMoverJogador(jogo, jogador, 1, jogador->linha - 1,
                              jogador->coluna);
  }

  if (tecla == 's' || tecla == 'S') {
    return tentarMoverJogador(jogo, jogador, 1, jogador->linha + 1,
                              jogador->coluna);
  }

  if (tecla == 'a' || tecla == 'A') {
    return tentarMoverJogador(jogo, jogador, 1, jogador->linha,
                              jogador->coluna - 1);
  }

  if (tecla == 'd' || tecla == 'D') {
    return tentarMoverJogador(jogo, jogador, 1, jogador->linha,
                              jogador->coluna + 1);
  }

  return 0;
}

static int moverJogador2(EstadoJogo *jogo, int tecla) {
  Jogador *jogador = &jogo->p2;

  if (tecla == KEY_UP) {
    return tentarMoverJogador(jogo, jogador, 2, jogador->linha - 1,
                              jogador->coluna);
  }

  if (tecla == KEY_DOWN) {
    return tentarMoverJogador(jogo, jogador, 2, jogador->linha + 1,
                              jogador->coluna);
  }

  if (tecla == KEY_LEFT) {
    return tentarMoverJogador(jogo, jogador, 2, jogador->linha,
                              jogador->coluna - 1);
  }

  if (tecla == KEY_RIGHT) {
    return tentarMoverJogador(jogo, jogador, 2, jogador->linha,
                              jogador->coluna + 1);
  }

  return 0;
}

static int moverPeixe(Peixe *peixe) {
  int direcao = rand() % 4;

  if (direcao == 0 && peixe->linha > 1) {
    peixe->linha--;
    return 1;
  }

  if (direcao == 1 && peixe->linha < LINHAS - 2) {
    peixe->linha++;
    return 1;
  }

  if (direcao == 2 && peixe->coluna > 1) {
    peixe->coluna--;
    return 1;
  }

  if (direcao == 3 && peixe->coluna < COLUNAS - 2) {
    peixe->coluna++;
    return 1;
  }

  return 0;
}

static int peixeDaCorDoJogador(const Peixe *peixe, int numeroJogador) {
  return (numeroJogador == 1 && peixe->cor == 1) ||
         (numeroJogador == 2 && peixe->cor == 2);
}

static Jogador *obterJogador(EstadoJogo *jogo, int numeroJogador) {
  if (numeroJogador == 1) {
    return &jogo->p1;
  }

  return &jogo->p2;
}

static int aplicarPontuacao(EstadoJogo *jogo, int numeroJogador) {
  Peixe *peixe = &jogo->peixe;
  Jogador *jogador = obterJogador(jogo, numeroJogador);
  int bonusCor = peixeDaCorDoJogador(peixe, numeroJogador);
  int ganho = 0;

  // Quem apanha o peixe recebe sempre a pontuação.
  // A cor própria dá bónus, mas a cor do adversário não transfere pontos.
  if (jogo->modo == MAIS_PEIXES) {
    ganho = bonusCor ? 2 : 1;
    jogador->peixes += ganho;
    return ganho;
  }

  if (jogo->modo == MAIS_PESO) {
    ganho = peixe->peso;

    if (bonusCor) {
      ganho += 2;
    }

    jogador->peso += ganho;
    return ganho;
  }

  if (jogo->modo == EMPILHAR) {
    ganho = bonusCor ? 2 : 1;
    jogador->empilhados += ganho;
    return ganho;
  }

  return 0;
}

static void iniciarAnimacaoCaptura(EstadoJogo *jogo, int linha, int coluna,
                                   int pontos, int numeroJogador) {
  // Mostra um * e o ganho durante alguns ciclos.
  jogo->animacaoCaptura = 10;
  jogo->animacaoLinha = linha;
  jogo->animacaoColuna = coluna;
  jogo->animacaoPontos = pontos;
  jogo->animacaoJogador = numeroJogador;
}

static int jogadorCapturou(const Jogador *jogador, const Peixe *peixe,
                           ModoJogo modo, int numeroJogador) {
  int linhaCaptura;
  int colunaCaptura;

  if (!peixe->ativo) {
    return 0;
  }

  obterZonaCaptura(jogador, modo, numeroJogador, &linhaCaptura, &colunaCaptura);

  if (peixe->linha != linhaCaptura) {
    return 0;
  }

  if (modo == EMPILHAR) {
    return peixe->coluna == colunaCaptura;
  }

  // O anzol tem dois caracteres: -< para PR e >- para PY.
  return peixe->coluna == colunaCaptura || peixe->coluna == colunaCaptura + 1;
}

static int verificarCapturas(EstadoJogo *jogo) {
  int linhaPeixe = jogo->peixe.linha;
  int colunaPeixe = jogo->peixe.coluna;
  int pontos;

  if (jogadorCapturou(&jogo->p1, &jogo->peixe, jogo->modo, 1)) {
    pontos = aplicarPontuacao(jogo, 1);
    iniciarAnimacaoCaptura(jogo, linhaPeixe, colunaPeixe, pontos, 1);
    gerarPeixe(jogo);
    return 1;
  }

  if (jogo->jogadores == 2 &&
      jogadorCapturou(&jogo->p2, &jogo->peixe, jogo->modo, 2)) {
    pontos = aplicarPontuacao(jogo, 2);
    iniciarAnimacaoCaptura(jogo, linhaPeixe, colunaPeixe, pontos, 2);
    gerarPeixe(jogo);
    return 1;
  }

  return 0;
}

int atualizarJogo(EstadoJogo *jogo, int teclaJogador1, int teclaJogador2,
                  int *contadorMovimentoPeixe, time_t *ultimoSegundo) {
  int houveAlteracao = 0;

  if (jogo->animacaoCaptura > 0) {
    jogo->animacaoCaptura--;
    houveAlteracao = 1;
  }

  // Cada jogador tem uma tecla própria para permitir melhor jogabilidade
  // nos modos de dois jogadores.
  if (teclaJogador1 != ERR && moverJogador1(jogo, teclaJogador1)) {
    houveAlteracao = 1;
  }

  if (jogo->jogadores == 2 && teclaJogador2 != ERR &&
      moverJogador2(jogo, teclaJogador2)) {
    houveAlteracao = 1;
  }

  (*contadorMovimentoPeixe)++;

  // Mantém o peixe numa velocidade parecida mesmo com o ciclo mais rápido.
  if (*contadorMovimentoPeixe >= 14) {
    if (moverPeixe(&jogo->peixe)) {
      houveAlteracao = 1;
    }
    *contadorMovimentoPeixe = 0;
  }

  if (verificarCapturas(jogo)) {
    houveAlteracao = 1;
  }

  if (jogo->tempo != 9999 && time(NULL) != *ultimoSegundo) {
    jogo->tempo--;
    *ultimoSegundo = time(NULL);
    houveAlteracao = 1;
  }

  return houveAlteracao;
}

static int obterPontuacaoFinal(const Jogador *jogador, ModoJogo modo) {
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

static const char *nomePontuacao(ModoJogo modo, int valor) {
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

void mostrarResultado(const EstadoJogo *jogo) {
  int pontosP1 = obterPontuacaoFinal(&jogo->p1, jogo->modo);
  int pontosP2 = obterPontuacaoFinal(&jogo->p2, jogo->modo);
  const char *tipoPontuacaoP1 = nomePontuacao(jogo->modo, pontosP1);
  const char *tipoPontuacaoP2 = nomePontuacao(jogo->modo, pontosP2);

  printf("\n==============================\n");
  printf("          FIM DO JOGO\n");
  printf("==============================\n\n");

  printf("Modo: %s\n\n", nomeModo(jogo->modo));

  printf("Pontuacao final:\n");
  printf(COR_VERMELHO "PR" COR_RESET ": %d %s\n", pontosP1, tipoPontuacaoP1);

  if (jogo->jogadores == 2) {
    printf(COR_AMARELO "PY" COR_RESET ": %d %s\n", pontosP2, tipoPontuacaoP2);

    if (pontosP1 > pontosP2) {
      printf("\nVencedor: " COR_VERMELHO "PR" COR_RESET "\n");
    } else if (pontosP2 > pontosP1) {
      printf("\nVencedor: " COR_AMARELO "PY" COR_RESET "\n");
    } else {
      printf("\nResultado: empate\n");
    }
  }
}
