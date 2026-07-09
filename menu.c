#include "menu.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COR_VERMELHO "\033[1;31m"
#define COR_AMARELO "\033[1;33m"
#define COR_RESET "\033[0m"

static void limparRestoLinha(void) {
  int c;

  while ((c = getchar()) != '\n' && c != EOF) {
    // Remove caracteres que ficaram no buffer.
  }
}

static void limparEcraTexto(void) {
  // Limpa o terminal para a ajuda ficar mais legivel.
  printf("\033[2J\033[H");
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

Idioma escolherIdioma(void) {
  int opcao;

  do {
    printf("\n==============================\n");
    printf("       LANGUAGE / IDIOMA\n");
    printf("==============================\n");
    printf("1 - English\n");
    printf("2 - Portugues\n");
    printf("0 - Sair / Exit\n");
    printf("Escolhe uma opcao / Choose an option: ");

    opcao = lerOpcao();

    if (opcao == IDIOMA_EN) {
      return IDIOMA_EN;
    }

    if (opcao == IDIOMA_PT) {
      return IDIOMA_PT;
    }

    if (opcao == IDIOMA_SAIR) {
      return IDIOMA_SAIR;
    }

    printf("Opcao invalida / Invalid option.\n");

  } while (1);
}

void mostrarMenu(Idioma idioma) {
  printf("\n==============================\n");
  printf("     PENGUIN FISHING GAME\n");
  printf("==============================\n");

  if (idioma == IDIOMA_EN) {
    printf("1 - Solo: most fish\n");
    printf("2 - Solo: most weight\n");
    printf("3 - Solo: stack fish\n");
    printf("4 - 2 players: most fish\n");
    printf("5 - 2 players: most weight\n");
    printf("6 - 2 players: stack fish\n");
    printf("7 - Practice: most fish\n");
    printf("8 - Practice: most weight\n");
    printf("9 - Practice: stack fish\n");
    printf("10 - Change language\n");
    printf("11 - How to play\n");
    printf("0 - Exit\n");
    printf("Choose an option: ");
  } else {
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
    printf("11 - Como jogar\n");
    printf("0 - Sair\n");
    printf("Escolhe uma opcao: ");
  }
}

void mostrarComoJogar(Idioma idioma) {
  limparEcraTexto();

  if (idioma == IDIOMA_EN) {
    printf("\n");
    printf("==============================\n");
    printf("          HOW TO PLAY\n");
    printf("        PAGE 1 OF 2\n");
    printf("==============================\n\n");

    printf("GOAL\n");
    printf("- Catch fish on the board.\n");
    printf("- The winner depends on the selected mode.\n\n");

    printf("GAME MODES\n");
    printf("- Most fish   : player with more fish wins.\n");
    printf("- Most weight : player with more total weight wins.\n");
    printf("- Stack fish  : player with the biggest stack wins.\n\n");

    printf("FISH TYPES\n");
    printf("- F : neutral fish.\n");
    printf("- R : gives bonus to PR.\n");
    printf("- Y : gives bonus to PY.\n\n");

    printf("CONTROLS\n");
    printf("- PR : W A S D | default rotation Z/X\n");
    printf("- PY : arrow keys | default rotation N/M\n");
    printf("- Hooks rotate 90 degrees left or right.\n");
    printf("- Used keys cannot be chosen as rotation keys.\n");
    printf("- *  : caught fish animation\n");
    printf("- P  : pause / continue\n");
    printf("- Q  : quit and show result\n\n");

    printf("Press Enter to see scoring rules...");
    limparRestoLinha();

    limparEcraTexto();
    printf("\n");
    printf("==============================\n");
    printf("          HOW TO PLAY\n");
    printf("        PAGE 2 OF 2\n");
    printf("==============================\n\n");

    printf("SCORING\n\n");

    printf("Most fish\n");
    printf("- F = +1 fish.\n");
    printf("- R = +2 for PR, +1 for PY.\n");
    printf("- Y = +2 for PY, +1 for PR.\n\n");

    printf("Most weight\n");
    printf("- Each fish adds its weight.\n");
    printf("- Player-colored fish = +2 bonus.\n");
    printf("- Example: PR catches R with weight 3 = 5.\n");
    printf("- Example: PR catches Y with weight 3 = 3.\n\n");

    printf("Stack fish\n");
    printf("- Player-colored fish = +2.\n");
    printf("- Neutral or opponent fish = +1.\n\n");

    printf("2-PLAYER MODE\n");
    printf("- The player who catches the fish gets the score.\n");
    printf("- Fish color only decides the bonus.\n");
    printf("- Players cannot occupy the same position.\n");
    printf("- A player can block the other.\n\n");

    printf("Press Enter to return to the menu...");
  } else {
    printf("\n");
    printf("==============================\n");
    printf("          COMO JOGAR\n");
    printf("        PAGINA 1 DE 2\n");
    printf("==============================\n\n");

    printf("OBJETIVO\n");
    printf("- Apanhar peixes no tabuleiro.\n");
    printf("- O vencedor depende do modo escolhido.\n\n");

    printf("MODOS DE JOGO\n");
    printf("- Mais peixes  : ganha quem tiver mais peixes.\n");
    printf("- Mais peso    : ganha quem tiver mais peso.\n");
    printf("- Empilhar     : ganha quem tiver a maior pilha.\n\n");

    printf("TIPOS DE PEIXE\n");
    printf("- F : peixe neutro.\n");
    printf("- R : da bonus ao PR.\n");
    printf("- Y : da bonus ao PY.\n\n");

    printf("CONTROLOS\n");
    printf("- PR : W A S D | rotacao padrao Z/X\n");
    printf("- PY : setas | rotacao padrao N/M\n");
    printf("- O anzol roda 90 graus para a esquerda ou direita.\n");
    printf("- Teclas ja usadas nao podem ser escolhidas para rotacao.\n");
    printf("- *  : animacao de captura\n");
    printf("- P  : pausa / continua\n");
    printf("- Q  : sair e mostrar resultado\n\n");

    printf("Pressiona Enter para ver a pontuacao...");
    limparRestoLinha();

    limparEcraTexto();
    printf("\n");
    printf("==============================\n");
    printf("          COMO JOGAR\n");
    printf("        PAGINA 2 DE 2\n");
    printf("==============================\n\n");

    printf("PONTUACAO\n\n");

    printf("Mais peixes\n");
    printf("- F = +1 peixe.\n");
    printf("- R = +2 para PR, +1 para PY.\n");
    printf("- Y = +2 para PY, +1 para PR.\n\n");

    printf("Mais peso\n");
    printf("- Cada peixe soma o seu peso.\n");
    printf("- Peixe da cor do jogador = +2 bonus.\n");
    printf("- Ex.: PR apanha R de peso 3 = 5.\n");
    printf("- Ex.: PR apanha Y de peso 3 = 3.\n\n");

    printf("Empilhar peixes\n");
    printf("- Peixe da cor do jogador = +2.\n");
    printf("- Peixe neutro ou adversario = +1.\n\n");

    printf("MODO 2 JOGADORES\n");
    printf("- Quem apanha o peixe recebe os pontos.\n");
    printf("- A cor do peixe so decide o bonus.\n");
    printf("- Os jogadores nao podem ocupar a mesma posicao.\n");
    printf("- Um jogador pode bloquear o outro.\n\n");

    printf("Pressiona Enter para voltar ao menu...");
  }

  limparRestoLinha();
}


static int teclaParaMinuscula(int tecla) {
  if (tecla >= 'A' && tecla <= 'Z') {
    return tecla - 'A' + 'a';
  }

  return tecla;
}

static int teclaReservadaRotacao(int tecla) {
  tecla = teclaParaMinuscula(tecla);

  // Teclas já usadas no jogo não podem ser escolhidas para rotação.
  return tecla == 'w' || tecla == 'a' || tecla == 's' || tecla == 'd' ||
         tecla == 'p' || tecla == 'q';
}

static int teclaJaEscolhida(int tecla, const int teclasUsadas[], int total) {
  tecla = teclaParaMinuscula(tecla);

  for (int i = 0; i < total; i++) {
    if (tecla == teclaParaMinuscula(teclasUsadas[i])) {
      return 1;
    }
  }

  return 0;
}

static int lerTeclaRotacao(Idioma idioma, const char *mensagem,
                           int teclasUsadas[], int *totalUsadas) {
  char linha[64];
  int tecla;

  do {
    printf("%s", mensagem);

    if (fgets(linha, sizeof(linha), stdin) == NULL) {
      continue;
    }

    if (strchr(linha, '\n') == NULL) {
      limparRestoLinha();
    }

    tecla = (unsigned char)linha[0];
    tecla = teclaParaMinuscula(tecla);

    if (tecla == '\n' || tecla == '\0' || isspace((unsigned char)tecla)) {
      if (idioma == IDIOMA_EN) {
        printf("Choose one visible key.\n");
      } else {
        printf("Escolhe uma tecla visivel.\n");
      }
      continue;
    }

    if (teclaReservadaRotacao(tecla)) {
      if (idioma == IDIOMA_EN) {
        printf("This key is already used by the game. Choose another one.\n");
      } else {
        printf("Esta tecla ja tem funcao no jogo. Escolhe outra.\n");
      }
      continue;
    }

    if (teclaJaEscolhida(tecla, teclasUsadas, *totalUsadas)) {
      if (idioma == IDIOMA_EN) {
        printf("This rotation key is already being used. Choose another one.\n");
      } else {
        printf("Esta tecla de rotacao ja esta a ser usada. Escolhe outra.\n");
      }
      continue;
    }

    teclasUsadas[*totalUsadas] = tecla;
    (*totalUsadas)++;
    return tecla;

  } while (1);
}

void configurarTeclasRotacao(EstadoJogo *jogo, Idioma idioma) {
  int opcao;
  int teclasUsadas[4];
  int totalUsadas = 0;

  if (jogo->modo == EMPILHAR) {
    return;
  }

  if (idioma == IDIOMA_EN) {
    printf("\n==============================\n");
    printf("       HOOK ROTATION\n");
    printf("==============================\n");
    printf("Default keys:\n");
    printf("- PR: Z = left, X = right\n");
    if (jogo->jogadores == 2) {
      printf("- PY: N = left, M = right\n");
    }
    printf("\nNote: keys already used by the game cannot be chosen.\n");
    printf("Reserved: W A S D, arrow keys, P and Q.\n\n");
    printf("1 - Use default keys\n");
    printf("2 - Choose my own keys\n");
    printf("Choose an option: ");
  } else {
    printf("\n==============================\n");
    printf("       ROTACAO DO ANZOL\n");
    printf("==============================\n");
    printf("Teclas padrao:\n");
    printf("- PR: Z = esquerda, X = direita\n");
    if (jogo->jogadores == 2) {
      printf("- PY: N = esquerda, M = direita\n");
    }
    printf("\nNota: teclas que ja fazem alguma coisa no jogo nao podem ser usadas.\n");
    printf("Reservadas: W A S D, setas, P e Q.\n\n");
    printf("1 - Usar teclas padrao\n");
    printf("2 - Escolher as minhas teclas\n");
    printf("Escolhe uma opcao: ");
  }

  opcao = lerOpcao();

  if (opcao != 2) {
    return;
  }

  if (idioma == IDIOMA_EN) {
    printf("\nChoose two rotation keys for each active player.\n");
    printf("Do not use W A S D, arrow keys, P, Q or repeated keys.\n\n");

    jogo->p1.teclaRodarEsquerda = lerTeclaRotacao(
        idioma, "PR rotate left: ", teclasUsadas, &totalUsadas);
    jogo->p1.teclaRodarDireita = lerTeclaRotacao(
        idioma, "PR rotate right: ", teclasUsadas, &totalUsadas);

    if (jogo->jogadores == 2) {
      jogo->p2.teclaRodarEsquerda = lerTeclaRotacao(
          idioma, "PY rotate left: ", teclasUsadas, &totalUsadas);
      jogo->p2.teclaRodarDireita = lerTeclaRotacao(
          idioma, "PY rotate right: ", teclasUsadas, &totalUsadas);
    }
  } else {
    printf("\nEscolhe duas teclas de rotacao para cada jogador ativo.\n");
    printf("Nao uses W A S D, setas, P, Q ou teclas repetidas.\n\n");

    jogo->p1.teclaRodarEsquerda = lerTeclaRotacao(
        idioma, "PR rodar para a esquerda: ", teclasUsadas, &totalUsadas);
    jogo->p1.teclaRodarDireita = lerTeclaRotacao(
        idioma, "PR rodar para a direita: ", teclasUsadas, &totalUsadas);

    if (jogo->jogadores == 2) {
      jogo->p2.teclaRodarEsquerda = lerTeclaRotacao(
          idioma, "PY rodar para a esquerda: ", teclasUsadas, &totalUsadas);
      jogo->p2.teclaRodarDireita = lerTeclaRotacao(
          idioma, "PY rodar para a direita: ", teclasUsadas, &totalUsadas);
    }
  }
}

int perguntarJogarNovamente(Idioma idioma, const EstadoJogo *jogo) {
  int opcao;

  do {
    if (idioma == IDIOMA_EN) {
      printf("\nDo you want to play again?\n");
      printf("1 - Yes, return to the menu\n");
      printf("0 - No, exit\n");

      printf("Players: " COR_VERMELHO "PR" COR_RESET);

      if (jogo->jogadores == 2) {
        printf(" | " COR_AMARELO "PY" COR_RESET);
      }

      printf("\nChoose an option: ");
    } else {
      printf("\nQueres jogar novamente?\n");
      printf("1 - Sim, voltar ao menu\n");
      printf("0 - Nao, sair\n");

      printf("Jogadores: " COR_VERMELHO "PR" COR_RESET);

      if (jogo->jogadores == 2) {
        printf(" | " COR_AMARELO "PY" COR_RESET);
      }

      printf("\nEscolhe uma opcao: ");
    }

    opcao = lerOpcao();

    if (opcao == 1) {
      return 1;
    }

    if (opcao == 0) {
      return 0;
    }

    if (idioma == IDIOMA_EN) {
      printf("Invalid option. Try again.\n");
    } else {
      printf("Opcao invalida. Tenta novamente.\n");
    }

  } while (1);
}

int confirmarSaida(Idioma idioma) {
  int opcao;

  do {
    if (idioma == IDIOMA_EN) {
      printf("\nAre you sure you want to exit?\n");
      printf("1 - Yes, exit\n");
      printf("2 - No, return to the menu\n");
      printf("Choose an option: ");
    } else {
      printf("\nTens a certeza que queres sair?\n");
      printf("1 - Sim, sair\n");
      printf("2 - Nao, voltar ao menu\n");
      printf("Escolhe uma opcao: ");
    }

    opcao = lerOpcao();

    if (opcao == 1) {
      return 1;
    }

    if (opcao == 2) {
      return 0;
    }

    if (idioma == IDIOMA_EN) {
      printf("Invalid option. Try again.\n");
    } else {
      printf("Opcao invalida. Tenta novamente.\n");
    }

  } while (1);
}