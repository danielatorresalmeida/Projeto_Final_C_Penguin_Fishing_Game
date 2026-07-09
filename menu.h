#ifndef MENU_H
#define MENU_H

#include "game.h"

int lerOpcao(void);
Idioma escolherIdioma(void);
void mostrarMenu(Idioma idioma);
void mostrarComoJogar(Idioma idioma);
void configurarTeclasRotacao(EstadoJogo *jogo, Idioma idioma);

int perguntarJogarNovamente(Idioma idioma, const EstadoJogo *jogo);
int confirmarSaida(Idioma idioma);

#endif