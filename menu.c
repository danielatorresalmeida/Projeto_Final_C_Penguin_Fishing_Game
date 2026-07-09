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
  if (idioma == IDIOMA_EN) {
    printf("\n");
    printf("==============================\n");
    printf("          HOW TO PLAY\n");
    printf("==============================\n");
    printf("\n");
    printf("Goal:\n");
    printf("- Control your penguin and catch fish on the board.\n");
    printf("- The winner depends on the selected game mode.\n");
    printf("\n");
    printf("Game modes:\n");
    printf("- Most fish: win by catching the highest number of fish.\n");
    printf("- Most weight: win by collecting the highest total weight.\n");
    printf("- Stack fish: win by building the highest fish stack.\n");
    printf("\n");
    printf("Fish types:\n");
    printf("- R: red fish. Gives bonus to PR when PR catches it.\n");
    printf("- Y: yellow fish. Gives bonus to PY when PY catches it.\n");
    printf("- F: neutral fish. No color bonus.\n");
    printf("\n");
    printf("Scoring by mode:\n");
    printf("- Most fish:\n");
    printf("  F gives +1 fish to the player who catches it.\n");
    printf("  R gives +2 if PR catches it, or +1 if PY catches it.\n");
    printf("  Y gives +2 if PY catches it, or +1 if PR catches it.\n");
    printf("\n");
    printf("- Most weight:\n");
    printf("  Each fish adds its weight to the player who catches it.\n");
    printf("  A fish with the player's color adds a +2 bonus.\n");
    printf("  Example: PR catches R with weight 3 = 5 points.\n");
    printf("  Example: PR catches Y with weight 3 = 3 points.\n");
    printf("\n");
    printf("- Stack fish:\n");
    printf("  Every caught fish increases the catcher player's stack.\n");
    printf("  A fish with the player's color gives +2 to the stack.\n");
    printf("  Neutral or opponent-colored fish gives +1 to the stack.\n");
    printf("\n");
    printf("Two-player mode:\n");
    printf("- The player who catches the fish always receives the score.\n");
    printf("- The fish color only decides whether there is a bonus.\n");
    printf("- Players cannot occupy or cross the same position.\n");
    printf("- A player can block the other by standing on the neighbouring pixel.\n");
    printf("\n");
    printf("Controls:\n");
    printf("- PR: use W, A, S, D.\n");
    printf("- PY: use arrow keys.\n");
    printf("- P: pause or continue the game.\n");
    printf("- Q: stop the game and show the final result.\n");
    printf("\n");
    printf("Press Enter to return to the menu...");
  } else {
    printf("\n");
    printf("==============================\n");
    printf("          COMO JOGAR\n");
    printf("==============================\n");
    printf("\n");
    printf("Objetivo:\n");
    printf("- Controlar o pinguim e apanhar peixes no tabuleiro.\n");
    printf("- O vencedor depende do modo de jogo escolhido.\n");
    printf("\n");
    printf("Modos de jogo:\n");
    printf("- Mais peixes: ganha quem apanhar mais peixes.\n");
    printf("- Mais peso: ganha quem acumular maior peso total.\n");
    printf("- Empilhar peixes: ganha quem tiver a maior pilha.\n");
    printf("\n");
    printf("Tipos de peixe:\n");
    printf("- R: peixe vermelho. Da bonus ao PR quando o PR o apanha.\n");
    printf("- Y: peixe amarelo. Da bonus ao PY quando o PY o apanha.\n");
    printf("- F: peixe neutro. Nao da bonus de cor.\n");
    printf("\n");
    printf("Pontuacao por modo:\n");
    printf("- Mais peixes:\n");
    printf("  F vale +1 peixe para quem o apanha.\n");
    printf("  R vale +2 se o PR apanhar, ou +1 se o PY apanhar.\n");
    printf("  Y vale +2 se o PY apanhar, ou +1 se o PR apanhar.\n");
    printf("\n");
    printf("- Mais peso:\n");
    printf("  Cada peixe adiciona o seu peso a quem o apanha.\n");
    printf("  Peixe da cor do jogador adiciona +2 de bonus.\n");
    printf("  Exemplo: PR apanha R de peso 3 = 5 pontos.\n");
    printf("  Exemplo: PR apanha Y de peso 3 = 3 pontos.\n");
    printf("\n");
    printf("- Empilhar peixes:\n");
    printf("  Cada peixe apanhado aumenta a pilha de quem o apanha.\n");
    printf("  Peixe da cor do jogador vale +2 na pilha.\n");
    printf("  Peixe neutro ou adversario vale +1 na pilha.\n");
    printf("\n");
    printf("Modo de dois jogadores:\n");
    printf("- Quem apanha o peixe recebe sempre a pontuacao.\n");
    printf("- A cor do peixe apenas decide se existe bonus.\n");
    printf("- Os jogadores nao podem ocupar nem atravessar a mesma posicao.\n");
    printf("- Um jogador pode bloquear o outro ficando no pixel vizinho.\n");
    printf("\n");
    printf("Controlos:\n");
    printf("- PR: usa W, A, S, D.\n");
    printf("- PY: usa as setas do teclado.\n");
    printf("- P: pausa ou continua o jogo.\n");
    printf("- Q: interrompe o jogo e mostra o resultado final.\n");
    printf("\n");
    printf("Pressiona Enter para voltar ao menu...");
  }

  limparRestoLinha();
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