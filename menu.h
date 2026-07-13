#ifndef MENU_H
#define MENU_H

#include "game.h"

int lerOpcao(void);
Idioma escolherIdioma(int somAtivo);
void mostrarMenu(Idioma idioma, int somAtivo);
void mostrarComoJogar(Idioma idioma);
void configurarTeclasRotacao(EstadoJogo *jogo, Idioma idioma, int somAtivo);

int perguntarJogarNovamente(Idioma idioma, const EstadoJogo *jogo, int somAtivo);
int confirmarSaida(Idioma idioma, int somAtivo);

#endif