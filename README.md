# Penguin Fishing Game

## Descrição do Projeto

O **Penguin Fishing Game** é um jogo desenvolvido em linguagem C como projeto final da unidade curricular. A ideia inicial surgiu a partir de um protótipo feito em Scratch, onde pinguins apanham peixes que saltam da água.

Nesta versão em C, o jogo foi adaptado para funcionar no terminal, usando a biblioteca **ncurses** para permitir movimento, desenho do tabuleiro, cores e interação com o teclado.

Link do protótipo original em Scratch:

https://scratch.mit.edu/projects/1311165580/editor/

## Objetivo do Jogo

O objetivo do jogo é controlar um pinguim e apanhar peixes que aparecem no tabuleiro. Dependendo do modo escolhido, o jogador pode tentar apanhar mais peixes, acumular mais peso ou empilhar peixes.

O jogo já contém uma versão funcional com menu, tabuleiro, jogadores, peixe, movimento, pontuação, resultado final, cores, legendas visuais e organização do código em vários ficheiros.

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

### Menu principal

- `1` a `9`: escolher o modo de jogo
- `10`: mudar o idioma, funcionalidade reservada para desenvolvimento
- `0`: sair do jogo

### Durante o jogo

#### Jogador 1

- `W`: mover para cima
- `A`: mover para a esquerda
- `S`: mover para baixo
- `D`: mover para a direita

#### Jogador 2

- Setas do teclado: mover o segundo jogador

#### Geral

- `Q`: interromper o jogo e mostrar o resultado final

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
├── menu.c
├── menu.h
├── logic.c
├── logic.h
├── render.c
├── render.h
├── README.md
├── LOG.md
├── .gitignore
└── .vscode/
    └── c_cpp_properties.json

Organização dos Ficheiros
main.c: contém a entrada principal do programa e o ciclo do menu.
game.c: controla o ciclo principal da partida.
game.h: contém as estruturas, constantes, enumerações e protótipos principais.
menu.c: contém o menu principal e a validação das opções introduzidas pelo utilizador.
menu.h: contém os protótipos relacionados com o menu.
logic.c: contém a lógica do jogo, como movimento, geração do peixe, capturas e pontuação.
logic.h: contém os protótipos relacionados com a lógica do jogo.
render.c: contém a parte visual do jogo, incluindo tabuleiro, cores, bordas e legendas.
render.h: contém os protótipos relacionados com a apresentação visual.
Estado Atual do Projeto

Até ao momento, o projeto já inclui:

Menu inicial no terminal
Estrutura organizada por vários ficheiros
Enumeração para os modos de jogo
Estruturas para jogador, peixe e estado do jogo
Matriz para representar o tabuleiro
Integração da biblioteca ncurses
Movimento com WASD para o jogador 1
Movimento com setas para o jogador 2
Tabuleiro com bordas e representação da água
Peixe visível no tabuleiro
Movimento simples e aleatório do peixe
Geração segura do peixe, evitando posições ocupadas
Sistema de pontuação por modo de jogo
Pontuação apresentada de acordo com o modo escolhido
Legenda adaptada ao modo escolhido
Cores com ncurses para distinguir jogadores, peixes, água e bordas
Jogador PR apresentado a vermelho
Jogador PY apresentado a amarelo
Resultado final com pontuação do modo escolhido
Interrupção do jogo com a tecla Q
Pontuação final apresentada mesmo quando o jogo é interrompido
Melhor fluidez do jogo com erase(), refresh() e controlo do ciclo principal
Validação das opções do menu com strtol()
Tratamento de entradas inválidas, como letras, símbolos, comandos incompletos e ENTER vazio
Ficheiros README.md e LOG.md atualizados
Como Compilar

No ambiente MSYS2 UCRT64, dentro da pasta do projeto, usar o seguinte comando:

gcc main.c game.c menu.c logic.c render.c -o jogo -I/ucrt64/include/ncurses -L/ucrt64/lib -lncursesw
Como Executar

Depois de compilar, executar com:

./jogo

No Windows, o executável gerado pode aparecer como:

jogo.exe

Este ficheiro não deve ser enviado para o GitHub.

Formatar o Código

Para formatar os ficheiros principais, usar:

clang-format -i main.c game.c game.h menu.c menu.h logic.c logic.h render.c render.h
Problemas Encontrados

Durante o desenvolvimento foram encontrados alguns problemas, como:

Erro no #include <ncurses.h> no VS Code
Problemas de configuração do IntelliSense
Erro de compilação por estar na pasta errada
Erro Permission denied quando o executável estava aberto
Tremor no ecrã durante o jogo
Algum atraso na resposta das teclas
Problemas com chavetas {} ao alterar a função cicloJogo()
Erro ao executar uma versão antiga do jogo depois de uma compilação falhada
Dificuldade ao validar entradas inválidas no menu
Problemas com atoi(), porque entradas vazias podiam ser interpretadas como opção 0
Dificuldade inicial em desenhar jogadores com dois caracteres, como PR e PY
Necessidade de reorganizar o código quando o ficheiro game.c ficou demasiado longo

A maioria destes problemas já foi corrigida. O jogo está mais fluido, o menu está mais seguro, o tabuleiro está mais claro e o código está melhor organizado.

Próximos Passos

Os próximos objetivos do projeto são:

Melhorar o resultado final, mostrando o vencedor de forma mais clara nos modos de dois jogadores.
Melhorar as mensagens quando o jogo é interrompido pelo jogador.
Criar uma opção de pausa durante a partida.
Adicionar a opção de guardar o jogo durante a partida.
Resolver o conflito da tecla S, porque atualmente ela move o jogador 1 para baixo.
Criar uma opção no menu principal para retomar um jogo guardado.
Guardar o estado do jogo num ficheiro externo.
Melhorar a experiência do utilizador durante o jogo.
Tornar ainda mais claro quais são os peixes associados a cada jogador.
Testar todos os modos de jogo.
Atualizar o LOG.md no fim de cada sessão.
Preparar a versão final para entrega.
Autor

Projeto desenvolvido por:

Daniela Torres Almeida

Nota Final

Este projeto encontra-se em desenvolvimento. A versão atual representa uma fase funcional do jogo, com foco na estrutura base, menus, movimento, pontuação, modos de jogo, organização do código e melhoria da experiência no terminal.    