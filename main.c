#include "game.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  EstadoJogo jogo;
  int opcao;
  Idioma idioma;

  srand((unsigned int)time(NULL));

  idioma = escolherIdioma();

  if (idioma == IDIOMA_SAIR) {
    printf("Obrigado por jogar! / Thanks for playing!\n");
    return 0;
  }

  do {
    mostrarMenu(idioma);
    opcao = lerOpcao();

    if (opcao == OPCAO_INVALIDA) {
      if (idioma == IDIOMA_EN) {
        printf("Invalid option. Try again.\n");
      } else {
        printf("Opcao invalida. Tenta novamente.\n");
      }

      continue;
    }

    if (opcao >= 1 && opcao <= 9) {
      iniciarJogo(&jogo, opcao);
      configurarTeclasRotacao(&jogo, idioma);
      cicloJogo(&jogo);
      mostrarResultado(&jogo);

      if (perguntarJogarNovamente(idioma, &jogo)) {
        continue;
      }

      if (confirmarSaida(idioma)) {
        opcao = 0;
      } else {
        continue;
      }

    } else if (opcao == 10) {
      idioma = escolherIdioma();

      if (idioma == IDIOMA_SAIR) {
        if (confirmarSaida(IDIOMA_PT)) {
          opcao = 0;
        } else {
          idioma = IDIOMA_PT;
        }
      }

    } else if (opcao == 11) {
      mostrarComoJogar(idioma);

    } else if (opcao == 0) {
      if (!confirmarSaida(idioma)) {
        opcao = OPCAO_INVALIDA;
      }

    } else {
      if (idioma == IDIOMA_EN) {
        printf("Invalid option. Try again.\n");
      } else {
        printf("Opcao invalida. Tenta novamente.\n");
      }
    }

  } while (opcao != 0);

  if (idioma == IDIOMA_EN) {
    printf("Thanks for playing!\n");
  } else {
    printf("Obrigado por jogar!\n");
  }

  return 0;
}