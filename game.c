#include "game.h"
#include "logic.h"
#include "render.h"
#include <ncursesw/ncurses.h>
#include <time.h>

#define ATRASO_CICLO_MS 25
#define MAX_TECLAS_POR_CICLO 20

static int teclaSair(int tecla) { return tecla == 'q' || tecla == 'Q'; }

static int teclaPausa(int tecla) { return tecla == 'p' || tecla == 'P'; }

static int lerUltimaTecla(int *sair) {
  int tecla;
  int ultimaTecla = ERR;
  int teclasLidas = 0;

  // Lê as teclas acumuladas e usa só a mais recente.
  while (teclasLidas < MAX_TECLAS_POR_CICLO) {
    tecla = getch();

    if (tecla == ERR) {
      break;
    }

    if (teclaSair(tecla)) {
      *sair = 1;
      return tecla;
    }

    ultimaTecla = tecla;
    teclasLidas++;
  }

  return ultimaTecla;
}

static int esperarDurantePausa(void) {
  int tecla;

  // Limpa teclas antigas para evitar sair da pausa sem querer.
  flushinp();

  while (1) {
    tecla = getch();

    if (teclaSair(tecla)) {
      return 0;
    }

    if (teclaPausa(tecla)) {
      flushinp();
      return 1;
    }

    napms(50);
  }
}

void cicloJogo(EstadoJogo *jogo) {
  int tecla;
  int sair = 0;
  int contadorMovimentoPeixe = 0;
  time_t ultimoSegundo = time(NULL);

  inicializarInterface();
  desenharJogo(jogo);

  while (jogo->ativo && jogo->tempo > 0 && !sair) {
    tecla = lerUltimaTecla(&sair);

    if (sair) {
      jogo->ativo = 0;
      break;
    }

    if (teclaPausa(tecla)) {
      desenharPausa();

      if (!esperarDurantePausa()) {
        jogo->ativo = 0;
        break;
      }

      // Evita que o tempo avance pelo período em que ficou pausado.
      ultimoSegundo = time(NULL);
      contadorMovimentoPeixe = 0;
      desenharJogo(jogo);
    } else if (atualizarJogo(jogo, tecla, &contadorMovimentoPeixe,
                             &ultimoSegundo)) {
      desenharJogo(jogo);
    }

    napms(ATRASO_CICLO_MS);
  }

  finalizarInterface();
  jogo->ativo = 0;
}
