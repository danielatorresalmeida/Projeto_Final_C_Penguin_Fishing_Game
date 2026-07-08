#ifndef GAME_H
#define GAME_H

#define OPCAO_INVALIDA -1
#define OPCAO_MINIMA 0
#define OPCAO_MAXIMA 10

#define LINHAS 15
#define COLUNAS 40

typedef enum {
  MENU = 0,
  MAIS_PEIXES = 1,
  MAIS_PESO = 2,
  EMPILHAR = 3
} ModoJogo;

typedef struct {
  char nome[30];
  char simbolo;
  int linha;
  int coluna;
  int peixes;
  int peso;
  int empilhados;
} Jogador;

typedef struct {
  char simbolo;
  int linha;
  int coluna;
  int cor;
  int tipo;
  int peso;
  int ativo;
} Peixe;

typedef struct {
  int jogadores;
  ModoJogo modo;
  int tempo;
  int ativo;
  Jogador p1;
  Jogador p2;
  Peixe peixe;
} EstadoJogo;

void mostrarMenu(void);
int lerOpcao(void);
void iniciarJogo(EstadoJogo *jogo, int opcao);
void cicloJogo(EstadoJogo *jogo);
void mostrarResultado(const EstadoJogo *jogo);
const char *nomeModo(ModoJogo modo);

#endif