#include "game.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void esperarEnter(void) {
  int c;

  printf("\nPressiona Enter para voltar ao menu...");

  while ((c = getchar()) != '\n' && c != EOF) {
    // Ignora tudo até o utilizador carregar em Enter.
  }
}

int main(void) {
  EstadoJogo jogo;
  int opcao;

  // Garante valores aleatorios diferentes em cada execucao.
  srand((unsigned int)time(NULL));

  do {
    mostrarMenu();
    opcao = lerOpcao();

    if (opcao == OPCAO_INVALIDA) {
      printf("Opcao invalida. Tenta novamente.\n");
      continue;
    }

    if (opcao >= 1 && opcao <= 9) {
      iniciarJogo(&jogo, opcao);
      cicloJogo(&jogo);
      mostrarResultado(&jogo);
      esperarEnter();
    } else if (opcao == 10) {
      printf("A opcao de mudar idioma ainda nao esta ativa nesta versao.\n");
    } else if (opcao != 0) {
      printf("Opcao invalida. Tenta novamente.\n");
    }

  } while (opcao != 0);

  printf("Obrigado por jogar!\n");
  return 0;
}
