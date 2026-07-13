#include "game.h"
#include "menu.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void tocarSomOpcaoMenu(int opcao, int somAtivo) {
  if (!somAtivo || opcao == OPCAO_INVALIDA) {
    return;
  }

  tocarSomMenu();
}

int main(void) {
  EstadoJogo jogo;
  int opcao;
  Idioma idioma;
  int somAtivo = 1;

  srand((unsigned int)time(NULL));

  idioma = escolherIdioma(somAtivo);

  if (idioma == IDIOMA_SAIR) {
    printf("Obrigado por jogar! / Thanks for playing!\n");
    return 0;
  }

  do {
    mostrarMenu(idioma, somAtivo);
    opcao = lerOpcao();

    if (opcao == OPCAO_INVALIDA) {
      if (idioma == IDIOMA_EN) {
        printf("Invalid option. Try again.\n");
      } else {
        printf("Opcao invalida. Tenta novamente.\n");
      }

      continue;
    }

    if (opcao == 12) {
      somAtivo = !somAtivo;

      // Quando o som e ligado, toca uma confirmacao curta.
      tocarSomOpcaoMenu(opcao, somAtivo);

      if (idioma == IDIOMA_EN) {
        printf("Sound is now %s.\n", somAtivo ? "on" : "off");
      } else {
        printf("Som agora esta %s.\n", somAtivo ? "ligado" : "desligado");
      }

    } else if (opcao >= 1 && opcao <= 9) {
      tocarSomOpcaoMenu(opcao, somAtivo);

      iniciarJogo(&jogo, opcao);
      jogo.somAtivo = somAtivo;
      configurarTeclasRotacao(&jogo, idioma, somAtivo);
      cicloJogo(&jogo);
      mostrarResultado(&jogo);

      if (perguntarJogarNovamente(idioma, &jogo, somAtivo)) {
        continue;
      }

      if (confirmarSaida(idioma, somAtivo)) {
        opcao = 0;
      } else {
        continue;
      }

    } else if (opcao == 10) {
      tocarSomOpcaoMenu(opcao, somAtivo);
      idioma = escolherIdioma(somAtivo);

      if (idioma == IDIOMA_SAIR) {
        if (confirmarSaida(IDIOMA_PT, somAtivo)) {
          opcao = 0;
        } else {
          idioma = IDIOMA_PT;
        }
      }

    } else if (opcao == 11) {
      tocarSomOpcaoMenu(opcao, somAtivo);
      mostrarComoJogar(idioma);

    } else if (opcao == 0) {
      tocarSomOpcaoMenu(opcao, somAtivo);

      if (!confirmarSaida(idioma, somAtivo)) {
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
