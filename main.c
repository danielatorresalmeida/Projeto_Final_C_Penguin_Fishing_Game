#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include <ncursesw/ncurses.h>

int main(void) {
    EstadoJogo jogo;
    int opcao;

    // Garante valores aleatorios diferentes em cada execucao
    srand((unsigned int) time(NULL));

    do {
        mostrarMenu();
        opcao = lerOpcao();

        if (opcao >= 1 && opcao <= 9) {
            iniciarJogo(&jogo, opcao);
            cicloJogo(&jogo);
            mostrarResultado(&jogo);

            printf("\nPressiona Enter para voltar ao menu...");
            getchar();
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    printf("Obrigado por jogar!\n");
    return 0;
}