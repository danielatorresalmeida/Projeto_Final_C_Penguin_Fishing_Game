#include "game.h"
#include "logic.h"
#include "render.h"
#include <ncurses.h>
#include <time.h>

void cicloJogo(EstadoJogo *jogo) {
  int tecla;
  int contadorMovimentoPeixe = 0;
  time_t ultimoSegundo = time(NULL);

  inicializarInterface();

  while (jogo->ativo && jogo->tempo > 0) {
    desenharJogo(jogo);

    tecla = getch();

    if (tecla == 'q' || tecla == 'Q') {
      jogo->ativo = 0;
      break;
    }

    atualizarJogo(jogo, tecla, &contadorMovimentoPeixe, &ultimoSegundo);
    napms(80);
  }

  finalizarInterface();
  jogo->ativo = 0;
}
