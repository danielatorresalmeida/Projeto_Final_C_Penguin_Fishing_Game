#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

typedef enum { IDIOMA_EN = 1, IDIOMA_PT = 2 } Idioma;

static Idioma idiomaAtual = IDIOMA_EN;
static int idiomaFoiEscolhido = 0;

static const char *texto(const char *en, const char *pt) {
  if (idiomaAtual == IDIOMA_PT) {
    return pt;
  }

  return en;
}

static void imprimirLinha(int largura) {
  for (int i = 0; i < largura; i++) {
    printf("=");
  }

  printf("\n");
}

static void imprimirTitulo(const char *titulo) {
  int tamanhoTitulo = strlen(titulo);
  int largura = tamanhoTitulo + 10;
  int espacos;

  if (largura < 36) {
    largura = 36;
  }

  espacos = (largura - tamanhoTitulo) / 2;

  imprimirLinha(largura);

  for (int i = 0; i < espacos; i++) {
    printf(" ");
  }

  printf("%s\n", titulo);

  imprimirLinha(largura);
}

static void escolherIdioma(void) {
  char linha[20];
  int opcao = -1;

  while (opcao != 1 && opcao != 2) {
    printf("\n");
    imprimirTitulo("LANGUAGE / IDIOMA");

    printf("1 - English\n");
    printf("2 - Portugues\n");
    printf("0 - Exit / Sair\n");
    printf("Option / Opcao: ");

    if (fgets(linha, sizeof(linha), stdin) == NULL) {
      idiomaAtual = IDIOMA_EN;
      idiomaFoiEscolhido = 1;
      return;
    }

    if (linha[0] == '\n') {
      printf("Invalid option / Opcao invalida.\n");
      continue;
    }

    opcao = atoi(linha);

    if (opcao == 0) {
      printf("\nExiting game / A sair do jogo...\n");
      exit(0);
    } else if (opcao == 1) {
      idiomaAtual = IDIOMA_EN;
    } else if (opcao == 2) {
      idiomaAtual = IDIOMA_PT;
    } else {
      printf("Invalid option / Opcao invalida.\n");
    }
  }

  idiomaFoiEscolhido = 1;
}

void mudarIdioma(void) {
  idiomaFoiEscolhido = 0;
  escolherIdioma();
}

void mostrarMenu(void) {
  if (!idiomaFoiEscolhido) {
    escolherIdioma();
  }

  printf("\n");
  imprimirTitulo(texto("PENGUIN FISHING GAME", "JOGO DE PESCA DOS PINGUINS"));

  printf("1 - %s\n", texto("Solo: most fish", "Solo: mais peixes"));
  printf("2 - %s\n", texto("Solo: most weight", "Solo: mais peso"));
  printf("3 - %s\n", texto("Solo: stack fish", "Solo: empilhar peixes"));
  printf("4 - %s\n", texto("2 players: most fish", "2 jogadores: mais peixes"));
  printf("5 - %s\n", texto("2 players: most weight", "2 jogadores: mais peso"));
  printf("6 - %s\n",
         texto("2 players: stack fish", "2 jogadores: empilhar peixes"));
  printf("7 - %s\n", texto("Practice: most fish", "Pratica: mais peixes"));
  printf("8 - %s\n", texto("Practice: most weight", "Pratica: mais peso"));
  printf("9 - %s\n", texto("Practice: stack fish", "Pratica: empilhar peixes"));
  printf("10 - %s\n", texto("Change language", "Mudar idioma"));
  printf("0 - %s\n", texto("Exit", "Sair"));

  printf("%s", texto("Choose an option: ", "Escolhe uma opcao: "));
}

int lerOpcao(void) {
  char linha[20];
  int opcao;

  while (1) {
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
      return 0;
    }

    if (linha[0] == '\n') {
      printf("%s", texto("Choose an option: ", "Escolhe uma opcao: "));
      continue;
    }

    opcao = atoi(linha);

    if (opcao >= 0 && opcao <= 10) {
      return opcao;
    }

    printf("%s", texto("Invalid option. Choose an option between 0 and 10: ",
                       "Opcao invalida. Escolhe uma opcao entre 0 e 10: "));
  }
}

const char *nomeModo(ModoJogo modo) {
  if (modo == MAIS_PEIXES) {
    return texto("Most Fish", "Mais Peixes");
  }

  if (modo == MAIS_PESO) {
    return texto("Most Weight", "Mais Peso");
  }

  if (modo == EMPILHAR) {
    return texto("Stack Fish", "Empilhar Peixes");
  }

  return texto("Menu", "Menu");
}

const char *objetivoModo(ModoJogo modo) {
  if (modo == MAIS_PEIXES) {
    return texto("Goal: catch the highest number of fish",
                 "Objetivo: apanhar o maior numero de peixes");
  }

  if (modo == MAIS_PESO) {
    return texto("Goal: catch fish with the highest total weight",
                 "Objetivo: apanhar peixes com maior peso total");
  }

  if (modo == EMPILHAR) {
    return texto("Goal: stack the highest number of fish",
                 "Objetivo: empilhar o maior numero de peixes");
  }

  return "";
}

static void gerarPeixeSeguro(EstadoJogo *jogo);

static void prepararJogador(Jogador *jogador, const char *nome, char simbolo,
                            int linha, int coluna) {
  // Prepara os dados iniciais de um jogador
  strcpy(jogador->nome, nome);
  jogador->simbolo = simbolo;
  jogador->linha = linha;
  jogador->coluna = coluna;
  jogador->peixes = 0;
  jogador->peso = 0;
  jogador->empilhados = 0;
}

static void gerarPeixe(Peixe *peixe) {
  // Gera um peixe numa posicao aleatoria
  peixe->linha = 2 + rand() % (LINHAS - 4);
  peixe->coluna = 2 + rand() % (COLUNAS - 4);
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

void iniciarJogo(EstadoJogo *jogo, int opcao) {
  jogo->ativo = 1;
  jogo->interrompidoPeloJogador = 0;

  // Define se o jogo tem 1 ou 2 jogadores
  if (opcao >= 1 && opcao <= 3) {
    jogo->jogadores = 1;
  } else if (opcao >= 7 && opcao <= 9) {
    jogo->jogadores = 1;
  } else {
    jogo->jogadores = 2;
  }

  // Define o modo de jogo
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

  // Modos de pratica têm tempo muito alto
  if (opcao >= 7 && opcao <= 9) {
    jogo->tempo = 9999;
  }

  prepararJogador(&jogo->p1, texto("Red Penguin", "Pinguim Vermelho"), 'P',
                  LINHAS - 2, 5);
  prepararJogador(&jogo->p2, texto("Yellow Penguin", "Pinguim Amarelo"), 'Y',
                  LINHAS - 2, COLUNAS - 6);

  gerarPeixeSeguro(jogo);
}

static void criarTabuleiro(char tabuleiro[LINHAS][COLUNAS]) {
  // Cria o tabuleiro com limites e água
  for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {

      // Bordas do tabuleiro
      if (i == 0 || i == LINHAS - 1 || j == 0 || j == COLUNAS - 1) {
        tabuleiro[i][j] = '#';
      } else {
        tabuleiro[i][j] = '~';
      }
    }
  }
}

static int dentroDoTabuleiro(int linha, int coluna) {
  return linha >= 0 && linha < LINHAS && coluna >= 0 && coluna < COLUNAS;
}

static void colocarNoTabuleiro(char tabuleiro[LINHAS][COLUNAS], int linha,
                               int coluna, char simbolo) {
  // Só coloca símbolos dentro dos limites
  if (dentroDoTabuleiro(linha, coluna)) {
    tabuleiro[linha][coluna] = simbolo;
  }
}

static void obterZonaCaptura(const Jogador *jogador, ModoJogo modo,
                             int numeroJogador, int *linha, int *coluna) {
  // No modo empilhar, a tigela fica em cima da cabeça
  if (modo == EMPILHAR) {
    *linha = jogador->linha - 1;
    *coluna = jogador->coluna;
    return;
  }

  // Nos outros modos, o anzol fica na mão
  *linha = jogador->linha;

  if (numeroJogador == 1) {
    *coluna = jogador->coluna + 1;
  } else {
    *coluna = jogador->coluna - 1;
  }
}

static int peixeEstaEmCimaDoJogador(const Peixe *peixe,
                                    const Jogador *jogador) {
  return peixe->linha == jogador->linha && peixe->coluna == jogador->coluna;
}

static int peixeEstaEmZonaCaptura(const Peixe *peixe, const Jogador *jogador,
                                  ModoJogo modo, int numeroJogador) {
  int linhaCaptura;
  int colunaCaptura;

  obterZonaCaptura(jogador, modo, numeroJogador, &linhaCaptura, &colunaCaptura);

  return peixe->linha == linhaCaptura && peixe->coluna == colunaCaptura;
}

static int posicaoPeixeValida(const EstadoJogo *jogo) {
  if (peixeEstaEmCimaDoJogador(&jogo->peixe, &jogo->p1)) {
    return 0;
  }

  if (peixeEstaEmZonaCaptura(&jogo->peixe, &jogo->p1, jogo->modo, 1)) {
    return 0;
  }

  if (jogo->jogadores == 2) {
    if (peixeEstaEmCimaDoJogador(&jogo->peixe, &jogo->p2)) {
      return 0;
    }

    if (peixeEstaEmZonaCaptura(&jogo->peixe, &jogo->p2, jogo->modo, 2)) {
      return 0;
    }
  }

  return 1;
}

static void gerarPeixeSeguro(EstadoJogo *jogo) {
  int tentativas = 0;

  do {
    gerarPeixe(&jogo->peixe);
    tentativas++;
  } while (!posicaoPeixeValida(jogo) && tentativas < 100);
}

static void desenharJogo(const EstadoJogo *jogo) {
  char tabuleiro[LINHAS][COLUNAS];
  int linhaCaptura;
  int colunaCaptura;

  criarTabuleiro(tabuleiro);

  // Coloca o peixe
  if (jogo->peixe.ativo) {
    colocarNoTabuleiro(tabuleiro, jogo->peixe.linha, jogo->peixe.coluna,
                       jogo->peixe.simbolo);
  }

  // Coloca o jogador 1
  colocarNoTabuleiro(tabuleiro, jogo->p1.linha, jogo->p1.coluna,
                     jogo->p1.simbolo);

  obterZonaCaptura(&jogo->p1, jogo->modo, 1, &linhaCaptura, &colunaCaptura);

  if (jogo->modo == EMPILHAR) {
    colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'B');
  } else {
    colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'H');
  }

  // Coloca o jogador 2 se existir
  if (jogo->jogadores == 2) {
    colocarNoTabuleiro(tabuleiro, jogo->p2.linha, jogo->p2.coluna,
                       jogo->p2.simbolo);

    obterZonaCaptura(&jogo->p2, jogo->modo, 2, &linhaCaptura, &colunaCaptura);

    if (jogo->modo == EMPILHAR) {
      colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'b');
    } else {
      colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'h');
    }
  }

  erase();

  mvprintw(0, 0, "%s",
           texto("Penguin Fishing Game", "Jogo de Pesca dos Pinguins"));
  mvprintw(1, 0, "%s: %s", texto("Mode", "Modo"), nomeModo(jogo->modo));
  mvprintw(2, 0, "%s", objetivoModo(jogo->modo));

  if (jogo->tempo == 9999) {
    mvprintw(3, 0, "%s",
             texto("Time: unlimited practice", "Tempo: pratica sem limite"));
  } else {
    mvprintw(3, 0, "%s: %d", texto("Time", "Tempo"), jogo->tempo);
  }

  if (jogo->jogadores == 2) {
    mvprintw(4, 0, "%s",
             texto("P1: WASD | P2: arrows | Q: quit",
                   "P1: WASD | P2: setas | Q: sair"));
  } else {
    mvprintw(4, 0, "%s", texto("P1: WASD | Q: quit", "P1: WASD | Q: sair"));
  }

  // Mostra apenas a legenda relevante para o modo escolhido
  if (jogo->modo == EMPILHAR) {
    if (jogo->jogadores == 2) {
      mvprintw(
          5, 0, "%s",
          texto(
              "Legend: P = player 1 | Y = player 2 | F/R/Y = fish | B/b = bowl",
              "Legenda: P = jogador 1 | Y = jogador 2 | F/R/Y = peixes | B/b = "
              "tigela"));
    } else {
      mvprintw(5, 0, "%s",
               texto("Legend: P = player | F/R/Y = fish | B = bowl",
                     "Legenda: P = jogador | F/R/Y = peixes | B = tigela"));
    }
  } else {
    if (jogo->jogadores == 2) {
      mvprintw(
          5, 0, "%s",
          texto(
              "Legend: P = player 1 | Y = player 2 | F/R/Y = fish | H/h = hook",
              "Legenda: P = jogador 1 | Y = jogador 2 | F/R/Y = peixes | H/h = "
              "anzol"));
    } else {
      mvprintw(5, 0, "%s",
               texto("Legend: P = player | F/R/Y = fish | H = hook",
                     "Legenda: P = jogador | F/R/Y = peixes | H = anzol"));
    }
  }

  // Mostra apenas a pontuacao importante para o modo escolhido
  if (jogo->modo == MAIS_PEIXES) {
    mvprintw(7, 0, texto("P1 fish: %d", "P1 peixes: %d"), jogo->p1.peixes);

    if (jogo->jogadores == 2) {
      mvprintw(8, 0, texto("P2 fish: %d", "P2 peixes: %d"), jogo->p2.peixes);
    }
  } else if (jogo->modo == MAIS_PESO) {
    mvprintw(7, 0, texto("P1 total weight: %d", "P1 peso total: %d"),
             jogo->p1.peso);

    if (jogo->jogadores == 2) {
      mvprintw(8, 0, texto("P2 total weight: %d", "P2 peso total: %d"),
               jogo->p2.peso);
    }
  } else if (jogo->modo == EMPILHAR) {
    mvprintw(7, 0, texto("P1 stacked fish: %d", "P1 peixes empilhados: %d"),
             jogo->p1.empilhados);

    if (jogo->jogadores == 2) {
      mvprintw(8, 0, texto("P2 stacked fish: %d", "P2 peixes empilhados: %d"),
               jogo->p2.empilhados);
    }
  }

  // Desenha a matriz do jogo
  for (int i = 0; i < LINHAS; i++) {
    for (int j = 0; j < COLUNAS; j++) {
      mvaddch(i + 10, j, tabuleiro[i][j]);
    }
  }
}

static void moverJogador1(Jogador *jogador, int tecla) {
  // Movimento do jogador 1 com WASD
  if ((tecla == 'w' || tecla == 'W') && jogador->linha > 1) {
    jogador->linha--;
  } else if ((tecla == 's' || tecla == 'S') && jogador->linha < LINHAS - 2) {
    jogador->linha++;
  } else if ((tecla == 'a' || tecla == 'A') && jogador->coluna > 1) {
    jogador->coluna--;
  } else if ((tecla == 'd' || tecla == 'D') && jogador->coluna < COLUNAS - 2) {
    jogador->coluna++;
  }
}

static void moverJogador2(Jogador *jogador, int tecla) {
  // Movimento do jogador 2 com setas
  if (tecla == KEY_UP && jogador->linha > 1) {
    jogador->linha--;
  } else if (tecla == KEY_DOWN && jogador->linha < LINHAS - 2) {
    jogador->linha++;
  } else if (tecla == KEY_LEFT && jogador->coluna > 1) {
    jogador->coluna--;
  } else if (tecla == KEY_RIGHT && jogador->coluna < COLUNAS - 2) {
    jogador->coluna++;
  }
}

static void moverPeixe(Peixe *peixe) {
  int direcao = rand() % 4;

  // Movimento simples e aleatorio do peixe
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

  // Modo 1: ganha quem apanhar mais peixes
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

  // Modo 2: ganha quem tiver mais peso
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

  // Modo 3: empilhar peixes
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
  // Verifica captura do jogador 1
  if (jogadorCapturou(&jogo->p1, &jogo->peixe, jogo->modo, 1)) {
    aplicarPontuacao(jogo, 1);
    gerarPeixeSeguro(jogo);
    return;
  }

  // Verifica captura do jogador 2
  if (jogo->jogadores == 2 &&
      jogadorCapturou(&jogo->p2, &jogo->peixe, jogo->modo, 2)) {
    aplicarPontuacao(jogo, 2);
    gerarPeixeSeguro(jogo);
  }
}

void cicloJogo(EstadoJogo *jogo) {
  int tecla;
  int contadorMovimentoPeixe = 0;
  time_t ultimoSegundo = time(NULL);

  // Inicializa o ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  while (jogo->ativo && jogo->tempo > 0) {
    // Le uma tecla por ciclo
    tecla = getch();

    if (tecla != ERR) {
      if (tecla == 'q' || tecla == 'Q') {
        jogo->interrompidoPeloJogador = 1;
        jogo->ativo = 0;
        break;
      }

      // Movimento do jogador 1
      moverJogador1(&jogo->p1, tecla);

      // Movimento do jogador 2, se existir
      if (jogo->jogadores == 2) {
        moverJogador2(&jogo->p2, tecla);
      }
    }

    // O peixe mexe a cada alguns ciclos
    contadorMovimentoPeixe++;

    if (contadorMovimentoPeixe >= 5) {
      moverPeixe(&jogo->peixe);
      contadorMovimentoPeixe = 0;
    }

    verificarCapturas(jogo);

    // Diminui o tempo uma vez por segundo
    if (jogo->tempo != 9999 && time(NULL) != ultimoSegundo) {
      jogo->tempo--;
      ultimoSegundo = time(NULL);
    }

    // Desenha o jogo no fim do ciclo
    desenharJogo(jogo);
    refresh();

    // Controla a velocidade do jogo
    napms(35);
  }

  // Fecha o ncurses
  endwin();

  jogo->ativo = 0;
}

void mostrarResultado(const EstadoJogo *jogo) {
  int valorP1 = 0;
  int valorP2 = 0;
  const char *tipoResultado = "";
  const char *unidade = "";

  printf("\n==============================\n");
  printf("          %s\n", texto("GAME OVER", "FIM DO JOGO"));
  printf("==============================\n");

if (jogo->interrompidoPeloJogador) {
  printf("%s\n\n", texto("Game interrupted by the player",
                         "Jogo interrompido pelo jogador"));
}

  // Define que valor vai ser usado para comparar os jogadores
  if (jogo->modo == MAIS_PEIXES) {
    valorP1 = jogo->p1.peixes;
    valorP2 = jogo->p2.peixes;
    tipoResultado = texto("Result by fish", "Resultado por peixes");
    unidade = texto("fish", "peixes");
  } else if (jogo->modo == MAIS_PESO) {
    valorP1 = jogo->p1.peso;
    valorP2 = jogo->p2.peso;
    tipoResultado = texto("Result by weight", "Resultado por peso");
    unidade = texto("weight", "peso");
  } else if (jogo->modo == EMPILHAR) {
    valorP1 = jogo->p1.empilhados;
    valorP2 = jogo->p2.empilhados;
    tipoResultado =
        texto("Result by stacked fish", "Resultado por peixes empilhados");
    unidade = texto("stacked fish", "peixes empilhados");
  }

  printf("%s:\n", tipoResultado);
  printf("P1: %d %s\n", valorP1, unidade);

  if (jogo->jogadores == 2) {
    printf("P2: %d %s\n", valorP2, unidade);
    printf("\n");

    if (valorP1 > valorP2) {
      printf("%s\n", texto("Winner: Player 1", "Vencedor: Jogador 1"));
    } else if (valorP2 > valorP1) {
      printf("%s\n", texto("Winner: Player 2", "Vencedor: Jogador 2"));
    } else {
      printf("%s\n", texto("Result: draw", "Resultado: empate"));
    }
  } else {
    printf("\n%s: %d %s\n",
           texto("Final player score", "Pontuacao final do jogador"), valorP1,
           unidade);
  }

  printf("==============================\n");
}