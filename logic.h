#ifndef LOGIC_H
#define LOGIC_H

#include "game.h"
#include <time.h>

void iniciarJogo(EstadoJogo *jogo, int opcao);

int atualizarJogo(EstadoJogo *jogo, int teclaJogador1, int teclaJogador2,
                  int *contadorMovimentoPeixe, time_t *ultimoSegundo);

void mostrarResultado(const EstadoJogo *jogo);

void obterZonaCaptura(const Jogador *jogador, ModoJogo modo, int numeroJogador,
                      int *linha, int *coluna);
void obterCelulaAnzol(const Jogador *jogador, ModoJogo modo, int posicao,
                      int *linha, int *coluna);

#endif
