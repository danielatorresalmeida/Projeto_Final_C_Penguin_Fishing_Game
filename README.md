# Penguin Fishing Game

## Descrição do Projeto

O **Penguin Fishing Game** é um jogo desenvolvido em linguagem C como projeto final da unidade curricular. A ideia inicial surgiu a partir de um protótipo feito em Scratch, onde pinguins apanham peixes que saltam da água.

Nesta versão em C, o jogo foi adaptado para funcionar no terminal, usando a biblioteca **ncurses** para permitir movimento, desenho do tabuleiro e interação com o teclado.

Link do protótipo original em Scratch:

https://scratch.mit.edu/projects/1311165580/editor/

## Objetivo do Jogo

O objetivo do jogo é controlar um pinguim e apanhar peixes que aparecem no tabuleiro. Dependendo do modo escolhido, o jogador pode tentar apanhar mais peixes, acumular mais peso ou empilhar peixes.

O jogo ainda está em desenvolvimento, mas já contém uma versão funcional com menu, escolha de idioma, tabuleiro, jogadores, peixe, movimento, pontuação, resultado final e melhorias na fluidez do jogo.

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
- `10`: mudar o idioma
- `0`: sair do jogo

### Menu de idioma

- `1`: English
- `2`: Português
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
├── README.md
├── LOG.md
├── .gitignore
└── .vscode/
    └── c_cpp_properties.json
```

## Estado Atual do Projeto

Até ao momento, o projeto já inclui:

- Menu inicial no terminal
- Escolha inicial de idioma entre inglês e português
- Opção para mudar o idioma a partir do menu principal
- Opção para sair a partir do menu de idioma
- Estrutura organizada por ficheiros
- Enumeração para os modos de jogo
- Estruturas para jogador, peixe e estado do jogo
- Matriz para representar o tabuleiro
- Integração da biblioteca ncurses
- Movimento com WASD para o jogador 1
- Movimento com setas para o jogador 2
- Tabuleiro com bordas e representação da água
- Peixe visível no tabuleiro
- Movimento simples e aleatório do peixe
- Geração segura do peixe, evitando posições ocupadas
- Sistema de pontuação por modo de jogo
- Pontuação apresentada de acordo com o modo escolhido
- Legenda adaptada ao modo escolhido
- Resultado final com indicação do vencedor nos modos de dois jogadores
- Mensagem quando o jogo é interrompido pelo jogador
- Pontuação final apresentada mesmo quando o jogo é interrompido
- Melhoria da fluidez do jogo com `erase()`, `refresh()` e controlo do ciclo principal
- Ajuste automático das linhas dos títulos nos menus
- Ficheiros README.md e LOG.md atualizados

## Como Compilar

No ambiente **MSYS2 UCRT64**, dentro da pasta do projeto, usar o seguinte comando:

```bash
gcc main.c game.c -o jogo -I/ucrt64/include/ncurses -L/ucrt64/lib -lncursesw
```

## Como Executar

Depois de compilar, executar com:

```bash
./jogo
```

No Windows, o executável gerado pode aparecer como:

```text
jogo.exe
```

Este ficheiro não deve ser enviado para o GitHub.

## Problemas Encontrados

Durante o desenvolvimento foram encontrados alguns problemas, como:

- Erro no `#include <ncurses.h>` no VS Code
- Problemas de configuração do IntelliSense
- Erro de compilação por estar na pasta errada
- Erro `Permission denied` quando o executável estava aberto
- Tremor no ecrã durante o jogo
- Algum atraso na resposta das teclas
- Problemas com chavetas `{}` ao alterar a função `cicloJogo()`
- Erro ao executar uma versão antiga do jogo depois de uma compilação falhada
- Dificuldade inicial em manter os textos coerentes entre inglês e português

A maioria destes problemas já foi corrigida. O jogo está mais fluido, o menu está mais completo e os textos principais já respeitam o idioma escolhido.

## Próximos Passos

Os próximos objetivos do projeto são:

- Adicionar a opção de guardar o jogo durante a partida.
- Usar a tecla `S` para guardar, ajustando os controlos se necessário para evitar conflito com o movimento para baixo.
- Criar uma opção no menu principal para retomar um jogo guardado.
- Guardar o estado do jogo num ficheiro externo.
- Melhorar a experiência do utilizador durante o jogo.
- Tornar mais claro quais são os peixes associados a cada jogador.
- Adicionar cores com ncurses para distinguir jogadores, peixes, água, bordas, anzóis e tigelas.
- Melhorar as legendas visuais para facilitar a compreensão dos modos de jogo.
- Melhorar a organização do código, se necessário, separando melhor lógica, interface e ficheiros.
- Testar todos os modos de jogo.
- Atualizar o LOG.md no fim de cada sessão.
- Preparar a versão final para entrega.
- Melhorar a validação das opções introduzidas pelo utilizador.
- Substituir o uso de `atoi()` por uma alternativa mais segura, como `strtol()`, para validar melhor os dados introduzidos.
- Evitar que entradas inválidas, como letras, símbolos ou comandos incompletos, sejam interpretadas como opções válidas.
- Aplicar validação tanto no menu principal como no menu de escolha de idioma.
- Apresentar mensagens de erro mais claras quando o utilizador escrever uma opção inválida.

## Autor

Projeto desenvolvido por:

**Daniela Torres Almeida**

## Nota Final

Este projeto encontra-se em desenvolvimento. A versão atual representa uma fase funcional do jogo, com foco na estrutura base, menus, escolha de idioma, movimento, pontuação, modos de jogo e melhoria da experiência no terminal.