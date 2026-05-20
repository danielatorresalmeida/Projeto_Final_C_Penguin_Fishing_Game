# Penguin Fishing Game

## Descrição do Projeto

O **Penguin Fishing Game** é um jogo desenvolvido em linguagem C como projeto final da unidade curricular. A ideia inicial surgiu a partir de um protótipo feito em Scratch, onde pinguins apanham peixes que saltam da água.

Nesta versão em C, o jogo foi adaptado para funcionar no terminal, usando a biblioteca **ncurses** para permitir movimento, desenho do tabuleiro e interação com o teclado.

Link do protótipo original em Scratch:

https://scratch.mit.edu/projects/1311165580/editor/

## Objetivo do Jogo

O objetivo do jogo é controlar um pinguim e apanhar peixes que aparecem no tabuleiro. Dependendo do modo escolhido, o jogador pode tentar apanhar mais peixes, acumular mais peso ou empilhar peixes.

O jogo ainda está em desenvolvimento, mas já contém uma estrutura inicial funcional com menu, tabuleiro, jogador, peixe, movimento e pontuação básica.

## Modos de Jogo

O jogo inclui os seguintes modos:

1. Solo: mais peixes  
2. Solo: mais peso  
3. Solo: empilhar peixes  
4. 2 jogadores: mais peixes  
5. 2 jogadores: mais peso  
6. 2 jogadores: empilhar peixes  
7. Prática: mais peixes  
8. Prática: mais peso  
9. Prática: empilhar peixes  

## Controlos

### Jogador 1

- `W`: mover para cima
- `A`: mover para a esquerda
- `S`: mover para baixo
- `D`: mover para a direita

### Jogador 2

- Setas do teclado: mover o segundo jogador

### Geral

- `Q`: sair do jogo

## Tecnologias Utilizadas

- Linguagem C
- Biblioteca ncurses
- MSYS2 UCRT64 no Windows
- Visual Studio Code
- Git e GitHub

## Estrutura do Projeto

```text
projeto-final-c/
│
├── main.c
├── game.c
├── game.h
├── README.md
├── LOG.md
├── .gitignore
└── .vscode/
    └── c_cpp_properties.json