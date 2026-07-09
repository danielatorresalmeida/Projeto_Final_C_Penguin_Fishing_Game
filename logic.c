#include "logic.h"
#include "game.h"
#include <ctype.h>
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COR_VERMELHO "\033[1;31m"
#define COR_AMARELO "\033[1;33m"
#define COR_RESET "\033[0m"
#define ANIMACAO_DURACAO 10
#define MOVIMENTO_PEIXE_INTERVALO 14

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

static int teclasIguais(int tecla, int configurada) {
  if (tecla == ERR || configurada == ERR) {
    return 0;
  }

  if (tecla == configurada) {
    return 1;
  }

  if (tecla >= 0 && tecla <= 255 && configurada >= 0 && configurada <= 255) {
    return tolower((unsigned char)tecla) ==
           tolower((unsigned char)configurada);
  }

  return 0;
}

static Direcao rodarEsquerda(Direcao direcao) {
  return (Direcao)((direcao + 3) % 4);
}

static Direcao rodarDireita(Direcao direcao) {
  return (Direcao)((direcao + 1) % 4);
}

static void prepararJogador(Jogador *jogador, const char *nome, char simbolo,
                            int linha, int coluna, Direcao direcao,
                            int teclaRodarEsquerda,
                            int teclaRodarDireita) {
  strcpy(jogador->nome, nome);
  jogador->simbolo = simbolo;
  jogador->linha = linha;
  jogador->coluna = coluna;
  jogador->peixes = 0;
  jogador->peso = 0;
  jogador->empilhados = 0;
  jogador->direcao = direcao;
  jogador->teclaRodarEsquerda = teclaRodarEsquerda;
  jogador->teclaRodarDireita = teclaRodarDireita;
}

void obterCelulaAnzol(const Jogador *jogador, ModoJogo modo, int posicao,
                      int *linha, int *coluna) {
  int centroColuna = jogador->coluna + 1;

  if (modo == EMPILHAR) {
    *linha = jogador->linha - 1;
    *coluna = jogador->coluna;
    return;
  }

  if (posicao < 0) {
    posicao = 0;
  }

  if (posicao > 1) {
    posicao = 1;
  }

  if (jogador->direcao == DIRECAO_CIMA) {
    *linha = jogador->linha - 1 - posicao;
    *coluna = centroColuna;
  } else if (jogador->direcao == DIRECAO_DIREITA) {
    *linha = jogador->linha;
    *coluna = jogador->coluna + 2 + posicao;
  } else if (jogador->direcao == DIRECAO_BAIXO) {
    *linha = jogador->linha + 1 + posicao;
    *coluna = centroColuna;
  } else {
    *linha = jogador->linha;
    *coluna = jogador->coluna - 1 - posicao;
  }
}

void obterZonaCaptura(const Jogador *jogador, ModoJogo modo, int numeroJogador,
                      int *linha, int *coluna) {
  (void)numeroJogador;
  obterCelulaAnzol(jogador, modo, 0, linha, coluna);
}

static int celulaDentroDoTabuleiro(int linha, int coluna) {
  return linha >= 0 && linha < LINHAS && coluna >= 0 && coluna < COLUNAS;
}

static int jogadorOcupaCelula(const Jogador *jogador, int linha, int coluna) {
  return linha == jogador->linha &&
         (coluna == jogador->coluna || coluna == jogador->coluna + 1);
}

static int anzolOcupaCelula(const Jogador *jogador, ModoJogo modo, int linha,
                            int coluna) {
  int linhaAnzol;
  int colunaAnzol;
  int totalCelulas = modo == EMPILHAR ? 1 : 2;

  for (int i = 0; i < totalCelulas; i++) {
    obterCelulaAnzol(jogador, modo, i, &linhaAnzol, &colunaAnzol);

    if (linha == linhaAnzol && coluna == colunaAnzol) {
      return 1;
    }
  }

  return 0;
}

static int anzolDentroDoTabuleiro(const Jogador *jogador, ModoJogo modo) {
  int linhaAnzol;
  int colunaAnzol;
  int totalCelulas = modo == EMPILHAR ? 1 : 2;

  for (int i = 0; i < totalCelulas; i++) {
    obterCelulaAnzol(jogador, modo, i, &linhaAnzol, &colunaAnzol);

    if (!celulaDentroDoTabuleiro(linhaAnzol, colunaAnzol)) {
      return 0;
    }
  }

  return 1;
}

static int posicaoOcupada(const EstadoJogo *jogo, int linha, int coluna) {
  if (jogadorOcupaCelula(&jogo->p1, linha, coluna) ||
      anzolOcupaCelula(&jogo->p1, jogo->modo, linha, coluna)) {
    return 1;
  }

  if (jogo->jogadores == 2) {
    if (jogadorOcupaCelula(&jogo->p2, linha, coluna) ||
        anzolOcupaCelula(&jogo->p2, jogo->modo, linha, coluna)) {
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

  prepararJogador(&jogo->p1, "Pinguim Vermelho", 'P', LINHAS - 2, 5,
                  DIRECAO_DIREITA, 'z', 'x');
  prepararJogador(&jogo->p2, "Pinguim Amarelo", 'Y', LINHAS - 2,
                  COLUNAS - 6, DIRECAO_ESQUERDA, 'n', 'm');

  jogo->animacaoCaptura = 0;
  jogo->animacaoLinha = 0;
  jogo->animacaoColuna = 0;
  jogo->animacaoPontos = 0;
  jogo->animacaoJogador = 0;

  gerarPeixe(jogo);
}

static int movimentoDentroDoTabuleiro(const EstadoJogo *jogo, int numeroJogador,
                                      int linha, int coluna) {
  Jogador teste;

  if (linha < 1 || linha >= LINHAS) {
    return 0;
  }

  if (coluna < 1 || coluna > COLUNAS - 3) {
    return 0;
  }

  teste = numeroJogador == 1 ? jogo->p1 : jogo->p2;
  teste.linha = linha;
  teste.coluna = coluna;

  return anzolDentroDoTabuleiro(&teste, jogo->modo);
}

static int movimentoBloqueadoPeloOutroJogador(const EstadoJogo *jogo,
                                              int numeroJogador, int linha,
                                              int coluna) {
  const Jogador *outroJogador;

  if (jogo->jogadores != 2) {
    return 0;
  }

  outroJogador = numeroJogador == 1 ? &jogo->p2 : &jogo->p1;

  // Cada pinguim ocupa duas colunas: PR ou PY.
  // Se a nova posição tocar no outro, o movimento é bloqueado.
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

static int tentarRodarJogador(EstadoJogo *jogo, Jogador *jogador,
                              Direcao novaDirecao) {
  Direcao direcaoAnterior = jogador->direcao;

  jogador->direcao = novaDirecao;

  // A rotação só acontece se o anzol continuar dentro do tabuleiro.
  if (!anzolDentroDoTabuleiro(jogador, jogo->modo)) {
    jogador->direcao = direcaoAnterior;
    return 0;
  }

  return 1;
}

static int moverJogador1(EstadoJogo *jogo, int tecla) {
  Jogador *jogador = &jogo->p1;

  if (teclasIguais(tecla, jogador->teclaRodarEsquerda)) {
    return tentarRodarJogador(jogo, jogador, rodarEsquerda(jogador->direcao));
  }

  if (teclasIguais(tecla, jogador->teclaRodarDireita)) {
    return tentarRodarJogador(jogo, jogador, rodarDireita(jogador->direcao));
  }

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

  if (teclasIguais(tecla, jogador->teclaRodarEsquerda)) {
    return tentarRodarJogador(jogo, jogador, rodarEsquerda(jogador->direcao));
  }

  if (teclasIguais(tecla, jogador->teclaRodarDireita)) {
    return tentarRodarJogador(jogo, jogador, rodarDireita(jogador->direcao));
  }

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
  jogo->animacaoCaptura = ANIMACAO_DURACAO;
  jogo->animacaoLinha = linha;
  jogo->animacaoColuna = coluna;
  jogo->animacaoPontos = pontos;
  jogo->animacaoJogador = numeroJogador;
}

static int jogadorCapturou(const Jogador *jogador, const Peixe *peixe,
                           ModoJogo modo) {
  int linhaAnzol;
  int colunaAnzol;
  int totalCelulas = modo == EMPILHAR ? 1 : 2;

  if (!peixe->ativo) {
    return 0;
  }

  for (int i = 0; i < totalCelulas; i++) {
    obterCelulaAnzol(jogador, modo, i, &linhaAnzol, &colunaAnzol);

    if (peixe->linha == linhaAnzol && peixe->coluna == colunaAnzol) {
      return 1;
    }
  }

  return 0;
}

static int verificarCapturas(EstadoJogo *jogo) {
  int linhaPeixe = jogo->peixe.linha;
  int colunaPeixe = jogo->peixe.coluna;
  int pontos;

  if (jogadorCapturou(&jogo->p1, &jogo->peixe, jogo->modo)) {
    pontos = aplicarPontuacao(jogo, 1);
    iniciarAnimacaoCaptura(jogo, linhaPeixe, colunaPeixe, pontos, 1);
    gerarPeixe(jogo);
    return 1;
  }

  if (jogo->jogadores == 2 &&
      jogadorCapturou(&jogo->p2, &jogo->peixe, jogo->modo)) {
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
  if (*contadorMovimentoPeixe >= MOVIMENTO_PEIXE_INTERVALO) {
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
