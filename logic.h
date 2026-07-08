#ifndef LOGIC_H
#define LOGIC_H

#include "game.h"
#include <time.h>

void atualizarJogo(EstadoJogo *jogo, int tecla, int *contadorMovimentoPeixe,
                   time_t *ultimoSegundo);
void obterZonaCaptura(const Jogador *jogador, ModoJogo modo,
                      int numeroJogador, int *linha, int *coluna);

#endif
