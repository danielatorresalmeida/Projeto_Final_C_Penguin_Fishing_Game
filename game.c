#include "game.h"
#include "logic.h"
#include "render.h"
#include <ncursesw/ncurses.h>
#include <time.h>

#define ATRASO_CICLO_MS 25
#define MAX_TECLAS_POR_CICLO 25

typedef struct {
  int jogador1;
  int jogador2;
  int pausar;
  int sair;
} EntradaJogo;

static int teclaSair(int tecla) { return tecla == 'q' || tecla == 'Q'; }

static int teclaPausa(int tecla) { return tecla == 'p' || tecla == 'P'; }

static int teclaDoJogador1(int tecla) {
  return tecla == 'w' || tecla == 'W' || tecla == 'a' || tecla == 'A' ||
         tecla == 's' || tecla == 'S' || tecla == 'd' || tecla == 'D';
}

static int teclaDoJogador2(int tecla) {
  return tecla == KEY_UP || tecla == KEY_DOWN || tecla == KEY_LEFT ||
         tecla == KEY_RIGHT;
}

static EntradaJogo criarEntradaVazia(void) {
  EntradaJogo entrada;

  entrada.jogador1 = ERR;
  entrada.jogador2 = ERR;
  entrada.pausar = 0;
  entrada.sair = 0;

  return entrada;
}

static EntradaJogo lerEntradaJogo(void) {
  int tecla;
  int teclasLidas = 0;
  EntradaJogo entrada = criarEntradaVazia();

  // Lê várias teclas acumuladas para reduzir atraso.
  // Guarda a tecla mais recente de cada jogador separadamente.
  while (teclasLidas < MAX_TECLAS_POR_CICLO) {
    tecla = getch();

    if (tecla == ERR) {
      break;
    }

    if (teclaSair(tecla)) {
      entrada.sair = 1;
      break;
    }

    if (teclaPausa(tecla)) {
      entrada.pausar = 1;
    } else if (teclaDoJogador1(tecla)) {
      entrada.jogador1 = tecla;
    } else if (teclaDoJogador2(tecla)) {
      entrada.jogador2 = tecla;
    }

    teclasLidas++;
  }

  return entrada;
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
  EntradaJogo entrada;
  int contadorMovimentoPeixe = 0;
  time_t ultimoSegundo = time(NULL);

  inicializarInterface();
  desenharJogo(jogo);

  while (jogo->ativo && jogo->tempo > 0) {
    entrada = lerEntradaJogo();

    if (entrada.sair) {
      jogo->ativo = 0;
      break;
    }

    if (entrada.pausar) {
      desenharPausa();

      if (!esperarDurantePausa()) {
        jogo->ativo = 0;
        break;
      }

      // Evita que o tempo avance pelo período em que ficou pausado.
      ultimoSegundo = time(NULL);
      contadorMovimentoPeixe = 0;
      desenharJogo(jogo);
    } else if (atualizarJogo(jogo, entrada.jogador1, entrada.jogador2,
                             &contadorMovimentoPeixe, &ultimoSegundo)) {
      desenharJogo(jogo);
    }

    napms(ATRASO_CICLO_MS);
  }

  finalizarInterface();
  jogo->ativo = 0;
}
