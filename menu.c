#include "game.h"
#include "menu.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mostrarMenu(void) {
  printf("\n==============================\n");
  printf("     PENGUIN FISHING GAME\n");
  printf("==============================\n");
  printf("1 - Solo: mais peixes\n");
  printf("2 - Solo: mais peso\n");
  printf("3 - Solo: empilhar peixes\n");
  printf("4 - 2 jogadores: mais peixes\n");
  printf("5 - 2 jogadores: mais peso\n");
  printf("6 - 2 jogadores: empilhar peixes\n");
  printf("7 - Pratica: mais peixes\n");
  printf("8 - Pratica: mais peso\n");
  printf("9 - Pratica: empilhar peixes\n");
  printf("10 - Mudar idioma\n");
  printf("0 - Sair\n");
  printf("Escolhe uma opcao: ");
}

static void limparRestoLinha(void) {
  int c;

  while ((c = getchar()) != '\n' && c != EOF) {
    // Remove caracteres que ficaram no buffer.
  }
}

int lerOpcao(void) {
  char linha[64];
  char *fim;
  long valor;

  if (fgets(linha, sizeof(linha), stdin) == NULL) {
    return OPCAO_INVALIDA;
  }

  if (strchr(linha, '\n') == NULL) {
    limparRestoLinha();
  }

  linha[strcspn(linha, "\n")] = '\0';

  errno = 0;
  valor = strtol(linha, &fim, 10);

  while (isspace((unsigned char)*fim)) {
    fim++;
  }

  if (fim == linha || *fim != '\0' || errno == ERANGE) {
    return OPCAO_INVALIDA;
  }

  if (valor < OPCAO_MINIMA || valor > OPCAO_MAXIMA) {
    return OPCAO_INVALIDA;
  }

  return (int)valor;
}
