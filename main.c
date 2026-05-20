#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

int main(void) {
  EstadoJogo jogo;
  int opcao;

  srand(time(NULL));

  do {
    mostrarMenu();
    opcao = lerOpcao();

    if (opcao == 0) {
      printf("A sair do jogo...\n");
      break;
    }

    if (opcao == 10) {
      mudarIdioma();
      continue;
    }

    iniciarJogo(&jogo, opcao);
    cicloJogo(&jogo);
    mostrarResultado(&jogo);
    printf("\nPressiona ENTER para voltar ao menu...");
    getchar();

  } while (opcao != 0);

  return 0;
}