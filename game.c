#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "game.h"

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

    // Lê a opção como texto para evitar problemas com scanf
    fgets(linha, sizeof(linha), stdin);

    return atoi(linha);
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

static void prepararJogador(Jogador *jogador, const char *nome, char simbolo, int linha, int coluna) {
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
    // Preenche o tabuleiro com pontos
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            tabuleiro[i][j] = '.';
        }
    }
}

static int dentroDoTabuleiro(int linha, int coluna) {
    return linha >= 0 && linha < LINHAS && coluna >= 0 && coluna < COLUNAS;
}

static void colocarNoTabuleiro(char tabuleiro[LINHAS][COLUNAS], int linha, int coluna, char simbolo) {
    // Só coloca símbolos dentro dos limites
    if (dentroDoTabuleiro(linha, coluna)) {
        tabuleiro[linha][coluna] = simbolo;
    }
}

static void obterZonaCaptura(const Jogador *jogador, ModoJogo modo, int numeroJogador, int *linha, int *coluna) {
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
        colocarNoTabuleiro(tabuleiro, jogo->peixe.linha, jogo->peixe.coluna, jogo->peixe.simbolo);
    }

    // Coloca o jogador 1
    colocarNoTabuleiro(tabuleiro, jogo->p1.linha, jogo->p1.coluna, jogo->p1.simbolo);

    obterZonaCaptura(&jogo->p1, jogo->modo, 1, &linhaCaptura, &colunaCaptura);

    if (jogo->modo == EMPILHAR) {
        colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'B');
    } else {
        colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'H');
    }

    // Coloca o jogador 2 se existir
    if (jogo->jogadores == 2) {
        colocarNoTabuleiro(tabuleiro, jogo->p2.linha, jogo->p2.coluna, jogo->p2.simbolo);

        obterZonaCaptura(&jogo->p2, jogo->modo, 2, &linhaCaptura, &colunaCaptura);

        if (jogo->modo == EMPILHAR) {
            colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'b');
        } else {
            colocarNoTabuleiro(tabuleiro, linhaCaptura, colunaCaptura, 'h');
        }
    }

    clear();

    mvprintw(0, 0, "Penguin Fishing Game");
    mvprintw(1, 0, "Modo: %s", nomeModo(jogo->modo));

    if (jogo->tempo == 9999) {
        mvprintw(2, 0, "Tempo: pratica infinita");
    } else {
        mvprintw(2, 0, "Tempo: %d", jogo->tempo);
    }

    mvprintw(3, 0, "P1: WASD | P2: setas | Q: sair");

    mvprintw(5, 0, "P1 peixes=%d peso=%d empilhados=%d",
             jogo->p1.peixes,
             jogo->p1.peso,
             jogo->p1.empilhados);

    if (jogo->jogadores == 2) {
        mvprintw(6, 0, "P2 peixes=%d peso=%d empilhados=%d",
                 jogo->p2.peixes,
                 jogo->p2.peso,
                 jogo->p2.empilhados);
    }

    // Desenha a matriz do jogo
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mvaddch(i + 8, j, tabuleiro[i][j]);
        }
    }

    refresh();
}

static void moverJogador1(Jogador *jogador, int tecla) {
    // Movimento do jogador 1 com WASD
    if ((tecla == 'w' || tecla == 'W') && jogador->linha > 1) {
        jogador->linha--;
    } else if ((tecla == 's' || tecla == 'S') && jogador->linha < LINHAS - 1) {
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
    } else if (tecla == KEY_DOWN && jogador->linha < LINHAS - 1) {
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

static int jogadorCapturou(const Jogador *jogador, const Peixe *peixe, ModoJogo modo, int numeroJogador) {
    int linhaCaptura;
    int colunaCaptura;

    obterZonaCaptura(jogador, modo, numeroJogador, &linhaCaptura, &colunaCaptura);

    return peixe->ativo &&
           peixe->linha == linhaCaptura &&
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

        // O peixe mexe a cada alguns frames
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

        napms(80);
    }

    // Fecha o ncurses
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