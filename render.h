#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void inicializarInterface(void);
void finalizarInterface(void);
void desenharJogo(const EstadoJogo *jogo);
void desenharPausa(void);
void tocarSomMenu(void);
void tocarSomCaptura(void);

#endif
