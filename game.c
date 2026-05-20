#include "game.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  printf("0 - Sair\n");
  printf("Escolhe uma opcao: ");
}

int lerOpcao(void) {
  char linha[20];
  int opcao;

  while (1) {
    // Lê a opção como texto
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
      return 0;
    }

    // Se o utilizador só carregar ENTER, pede outra vez
    if (linha[0] == '\n') {
      printf("Escolhe uma opcao: ");
      continue;
    }

    opcao = atoi(linha);

    // Só aceita opções entre 0 e 9
    if (opcao >= 0 && opcao <= 9) {
      return opcao;
    }

    printf("Opcao invalida. Escolhe uma opcao entre 0 e 9: ");
  }
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

const char *objetivoModo(ModoJogo modo) {
  if (modo == MAIS_PEIXES) {
    return "Objetivo: apanhar o maior numero de peixes";
  }

  if (modo == MAIS_PESO) {
    return "Objetivo: apanhar peixes com maior peso";
  }

  if (modo == EMPILHAR) {
    return "Objetivo: empilhar o maior numero de peixes";
  }

  return "";
}

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

  prepararJogador(&jogo->p1, "Pinguim Vermelho", 'P', LINHAS - 2, 5);
  prepararJogador(&jogo->p2, "Pinguim Amarelo", 'Y', LINHAS - 2, COLUNAS - 6);

  gerarPeixe(&jogo->peixe);
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

  mvprintw(0, 0, "Penguin Fishing Game");
  mvprintw(1, 0, "Modo: %s", nomeModo(jogo->modo));
  mvprintw(2, 0, "%s", objetivoModo(jogo->modo));

  if (jogo->tempo == 9999) {
    mvprintw(3, 0, "Tempo: pratica infinita");
  } else {
    mvprintw(3, 0, "Tempo: %d", jogo->tempo);
  }

  mvprintw(4, 0, "P1: WASD | P2: setas | Q: sair");

  // Mostra apenas a legenda relevante para o modo escolhido
  if (jogo->modo == EMPILHAR) {
    if (jogo->jogadores == 2) {
      mvprintw(5, 0,
               "Legenda: P = jogador 1 | Y = jogador 2 | F/R/Y = peixes | B/b "
               "= tigela");
    } else {
      mvprintw(5, 0, "Legenda: P = jogador | F/R/Y = peixes | B = tigela");
    }
  } else {
    if (jogo->jogadores == 2) {
      mvprintw(5, 0,
               "Legenda: P = jogador 1 | Y = jogador 2 | F/R/Y = peixes | H/h "
               "= anzol");
    } else {
      mvprintw(5, 0, "Legenda: P = jogador | F/R/Y = peixes | H = anzol");
    }
  }

  // Mostra apenas a pontuação importante para o modo escolhido
  if (jogo->modo == MAIS_PEIXES) {
    mvprintw(7, 0, "P1 peixes: %d", jogo->p1.peixes);

    if (jogo->jogadores == 2) {
      mvprintw(8, 0, "P2 peixes: %d", jogo->p2.peixes);
    }
  } else if (jogo->modo == MAIS_PESO) {
    mvprintw(7, 0, "P1 peso total: %d", jogo->p1.peso);

    if (jogo->jogadores == 2) {
      mvprintw(8, 0, "P2 peso total: %d", jogo->p2.peso);
    }
  } else if (jogo->modo == EMPILHAR) {
    mvprintw(7, 0, "P1 peixes empilhados: %d", jogo->p1.empilhados);

    if (jogo->jogadores == 2) {
      mvprintw(8, 0, "P2 peixes empilhados: %d", jogo->p2.empilhados);
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
    gerarPeixe(&jogo->peixe);
    return;
  }

  // Verifica captura do jogador 2
  if (jogo->jogadores == 2 &&
      jogadorCapturou(&jogo->p2, &jogo->peixe, jogo->modo, 2)) {
    aplicarPontuacao(jogo, 2);
    gerarPeixe(&jogo->peixe);
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
    // Lê uma tecla por ciclo
    tecla = getch();

    if (tecla != ERR) {
      if (tecla == 'q' || tecla == 'Q') {
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
  printf("          FIM DO JOGO\n");
  printf("==============================\n");

  // Define que valor vai ser usado para comparar os jogadores
  if (jogo->modo == MAIS_PEIXES) {
    valorP1 = jogo->p1.peixes;
    valorP2 = jogo->p2.peixes;
    tipoResultado = "Resultado por peixes";
    unidade = "peixes";
  } else if (jogo->modo == MAIS_PESO) {
    valorP1 = jogo->p1.peso;
    valorP2 = jogo->p2.peso;
    tipoResultado = "Resultado por peso";
    unidade = "peso";
  } else if (jogo->modo == EMPILHAR) {
    valorP1 = jogo->p1.empilhados;
    valorP2 = jogo->p2.empilhados;
    tipoResultado = "Resultado por peixes empilhados";
    unidade = "empilhados";
  }

  printf("%s:\n", tipoResultado);
  printf("P1: %d %s\n", valorP1, unidade);

  if (jogo->jogadores == 2) {
    printf("P2: %d %s\n", valorP2, unidade);

    printf("\n");

    if (valorP1 > valorP2) {
      printf("Vencedor: Jogador 1\n");
    } else if (valorP2 > valorP1) {
      printf("Vencedor: Jogador 2\n");
    } else {
      printf("Resultado: empate\n");
    }
  } else {
    printf("\nPontuacao final do jogador: %d %s\n", valorP1, unidade);
  }

  printf("==============================\n");
}