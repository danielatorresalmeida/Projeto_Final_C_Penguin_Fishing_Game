# Penguin Fishing Game

## Descrição do Projeto

O **Penguin Fishing Game** é um jogo desenvolvido em linguagem C como projeto final da unidade curricular. A ideia inicial surgiu a partir de um protótipo feito em Scratch, onde pinguins apanham peixes que saltam da água.

Nesta versão em C, o jogo foi adaptado para funcionar no terminal, usando a biblioteca **ncurses** para permitir movimento, desenho do tabuleiro, cores, menus e interação com o teclado.

Link do protótipo original em Scratch:

https://scratch.mit.edu/projects/1311165580/editor/

## Objetivo do Jogo

O objetivo do jogo é controlar um pinguim e apanhar peixes que aparecem no tabuleiro. Dependendo do modo escolhido, o jogador pode tentar apanhar mais peixes, acumular mais peso ou empilhar peixes.

O jogo já contém uma versão funcional com escolha inicial de idioma, menu principal, opção de ajuda, tabuleiro com cores, movimento, pontuação por modo, timer visível, tecla de pausa, melhor resposta das teclas nos dois jogadores, animação simples de captura, rotação de anzóis, teclas de rotação personalizáveis, resultado final e opção para jogar novamente.

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

## Pontuação e Regras de Dois Jogadores

A pontuação foi ajustada para ficar mais simples e justa: quem apanha o peixe recebe sempre a pontuação. A cor do peixe serve apenas para decidir se existe bónus.

### Modo Mais Peixes

- `F`: peixe neutro, vale `+1` peixe para quem o apanha.
- `R`: peixe vermelho, vale `+2` se for apanhado pelo `PR` e `+1` se for apanhado pelo `PY`.
- `Y`: peixe amarelo, vale `+2` se for apanhado pelo `PY` e `+1` se for apanhado pelo `PR`.

### Modo Mais Peso

- Cada peixe adiciona o seu peso ao jogador que o apanha.
- Se o peixe for da cor do jogador, recebe `+2` de bónus no peso.
- Exemplo: `PR` apanha `R` de peso `3`, recebe `5` pontos.
- Exemplo: `PR` apanha `Y` de peso `3`, recebe `3` pontos.
- Exemplo: `PY` apanha `Y` de peso `5`, recebe `7` pontos.

### Modo Empilhar Peixes

- Cada peixe apanhado aumenta a pilha do jogador que o apanha.
- Peixe da cor do jogador vale `+2` na pilha.
- Peixe neutro ou da cor do adversário vale `+1` na pilha.

### Bloqueio entre jogadores

Nos modos de dois jogadores, `PR` e `PY` não podem ocupar nem atravessar a mesma posição no tabuleiro. Como cada jogador ocupa dois caracteres, o caminho fica bloqueado quando um jogador tenta passar por cima do outro ou fica encostado a ele. Isto permite bloquear o adversário de forma estratégica sem adicionar penalizações automáticas.


## Informação apresentada durante o jogo

Durante a partida, o ecrã mostra informação rápida para ajudar o jogador a tomar decisões sem abrir o menu de ajuda:

- Painel esquerdo com objetivo do modo e regra rápida de pontuação.
- Painel direito com legenda e controlos.
- Linha `PEIXE`, por cima do tabuleiro, mostrando o símbolo do peixe, peso quando aplicável e quanto cada jogador ganha se o apanhar.
- Anzóis direcionais que podem rodar 90 graus para a esquerda ou para a direita.
- Animação simples com `*` quando um peixe é capturado.
- O `*` da captura usa uma cor própria, magenta, para se destacar dos peixes.
- Teclas de rotação mostradas no painel lateral durante a partida.
- Feedback visual do ganho, como `+1`, `+2` ou `+5`, junto ao local da captura.
- Indicação de `bloqueio ativo` nos modos de dois jogadores.

Exemplos de informação dinâmica:

```text
PEIXE: R peso 3 | PR +5 | PY +3
```

```text
PEIXE: Y pilha | PR +1 | PY +2
```

## Controlos

### Menu de idioma

- `1`: English
- `2`: Português
- `0`: Sair / Exit

### Menu principal

- `1` a `9`: escolher o modo de jogo
- `10`: mudar idioma
- `11`: Como jogar / How to play
- `0`: sair do jogo

### Durante o jogo

#### Jogador 1

- `W`: mover para cima
- `A`: mover para a esquerda
- `S`: mover para baixo
- `D`: mover para a direita
- `Z`: rodar o anzol 90 graus para a esquerda, por padrão
- `X`: rodar o anzol 90 graus para a direita, por padrão
- As teclas de rotação podem ser alteradas antes da partida

#### Jogador 2

- Setas do teclado: mover o segundo jogador
- `N`: rodar o anzol 90 graus para a esquerda, por padrão
- `M`: rodar o anzol 90 graus para a direita, por padrão
- As teclas de rotação podem ser alteradas antes da partida
- Nos modos de dois jogadores, as teclas de `PR` e `PY` são processadas separadamente para melhorar a jogabilidade

#### Geral

- Antes da partida, o jogador pode escolher teclas personalizadas para rodar o anzol
- Teclas já usadas pelo jogo não podem ser escolhidas para rotação, como `W`, `A`, `S`, `D`, setas, `P` e `Q`
- `*`: aparece brevemente quando um peixe é capturado
- `P`: pausar ou continuar o jogo
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
```

## Organização dos Ficheiros

- `main.c`: contém a entrada principal do programa e o ciclo do menu.
- `game.c`: controla o ciclo principal da partida.
- `game.h`: contém estruturas, constantes, enumerações e protótipos principais.
- `menu.c`: contém o menu principal, escolha de idioma, ajuda e validação das opções.
- `menu.h`: contém os protótipos relacionados com o menu.
- `logic.c`: contém a lógica do jogo, como movimento, geração do peixe, capturas, pontuação e resultado final.
- `logic.h`: contém os protótipos relacionados com a lógica do jogo.
- `render.c`: contém a parte visual do jogo, incluindo tabuleiro, cores, bordas, timer, painéis e legendas.
- `render.h`: contém os protótipos relacionados com a apresentação visual.

## Estado Atual do Projeto

Até ao momento, o projeto já inclui:

- Escolha inicial de idioma antes do menu principal
- Menu principal em português ou inglês
- Opção para mudar idioma durante a execução
- Opção `11 - Como jogar / How to play`
- Ajuda `Como jogar / How to play` reorganizada em duas páginas para melhorar a leitura
- Página 1 da ajuda com objetivo, modos, tipos de peixe e controlos
- Página 2 da ajuda com pontuação e regras de dois jogadores
- Estrutura organizada por vários ficheiros
- Enumeração para os modos de jogo
- Estruturas para jogador, peixe e estado do jogo
- Matriz para representar o tabuleiro
- Integração da biblioteca ncurses
- Movimento com WASD para o jogador 1
- Movimento com setas para o jogador 2
- Leitura separada das teclas de `PR` e `PY` nos modos de dois jogadores
- Melhor jogabilidade do jogador amarelo `PY` quando existem várias teclas acumuladas
- Bloqueio entre jogadores para impedir que `PR` e `PY` ocupem a mesma posição
- Possibilidade de bloquear o caminho do adversário ficando lado a lado
- Tabuleiro com bordas e representação da água
- Peixe visível no tabuleiro
- Movimento simples e aleatório do peixe
- Geração segura do peixe, evitando posições ocupadas
- Sistema de pontuação por modo de jogo
- Pontuação apresentada de acordo com o modo escolhido
- Regra de pontuação em que quem apanha o peixe recebe sempre a pontuação
- Bónus de cor própria nos modos de pontuação
- Peixe da cor do adversário conta para quem o apanha, mas sem bónus
- Pontuação colocada por cima do tabuleiro para maior visibilidade
- Correção de singular/plural na pontuação, por exemplo `1 peixe` e `2 peixes`
- Timer centrado e integrado na borda superior do tabuleiro
- Destaque visual nos últimos 10 segundos
- Legenda e controlos agrupados num painel lateral
- Objetivo do modo apresentado num painel próprio
- Regra rápida do modo apresentada no painel de objetivo
- Informação dinâmica do peixe atual durante a partida
- Peixe atual mostrado por cima do tabuleiro, entre a pontuação e o timer
- Ganho previsto de `PR` e `PY` mostrado perto do tabuleiro
- Peso do peixe atual mostrado no modo **Mais peso**
- Anzóis direcionais nos modos de pesca
- Rotação dos anzóis 90 graus para a esquerda ou para a direita
- Possibilidade de escolher teclas personalizadas para rodar o anzol antes da partida
- Validação para impedir teclas repetidas ou teclas já usadas pelo jogo
- Painel lateral mostra as teclas de rotação escolhidas
- Ajuste da zona de captura para considerar as duas posições do anzol em qualquer direção
- Animação de captura com `*` no local onde o peixe foi apanhado
- Feedback visual do ganho junto à animação de captura
- Cores com ncurses para distinguir jogadores, peixes, água e bordas
- Jogador `PR` apresentado a vermelho
- Jogador `PY` apresentado a amarelo
- Resultado final com pontuação do modo escolhido
- Resultado final com indicação de vencedor ou empate em modos de dois jogadores
- Opção para jogar novamente depois do fim da partida
- Confirmação antes de sair do jogo
- Interrupção do jogo com a tecla `Q`
- Pontuação final apresentada mesmo quando o jogo é interrompido
- Ciclo principal mais responsivo, com menor atraso entre leituras do teclado
- Pausa durante a partida com a tecla `P`
- Timer parado durante a pausa
- Mensagem visual de pausa no centro do tabuleiro
- Leitura de várias teclas acumuladas por ciclo
- Utilização da tecla mais recente de cada jogador, em vez de uma única tecla global
- Redesenho do ecrã apenas quando o estado do jogo muda
- Velocidade do peixe ajustada para manter o equilíbrio após a redução do atraso do ciclo
- Melhor fluidez do jogo com `erase()`, `refresh()` e controlo do ciclo principal
- Validação das opções do menu com `strtol()`
- Tratamento de entradas inválidas, como letras, símbolos, comandos incompletos e ENTER vazio

## Como Compilar

No ambiente **MSYS2 UCRT64**, dentro da pasta do projeto, usar o seguinte comando:

```bash
gcc main.c game.c menu.c logic.c render.c -o jogo -I/ucrt64/include -L/ucrt64/lib -lncursesw
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

## Formatar o Código

Para formatar os ficheiros principais, usar:

```bash
clang-format -i main.c game.c game.h menu.c menu.h logic.c logic.h render.c render.h
```

## Problemas Encontrados

Durante o desenvolvimento foram encontrados alguns problemas, como:

- Erro no `#include <ncurses.h>` no VS Code
- Problemas de configuração do IntelliSense
- Erro de compilação por estar na pasta errada
- Erro `Permission denied` quando o executável estava aberto
- Tremor no ecrã durante o jogo
- Algum atraso na resposta das teclas quando o jogador pressionava teclas continuamente
- Problemas com chavetas `{}` ao alterar a função `cicloJogo()`
- Erro ao executar uma versão antiga do jogo depois de uma compilação falhada
- Dificuldade ao validar entradas inválidas no menu
- Problemas com `atoi()`, porque entradas vazias podiam ser interpretadas como opção `0`
- Dificuldade inicial em desenhar jogadores com dois caracteres, como `PR` e `PY`
- Necessidade de reorganizar o código quando o ficheiro `game.c` ficou demasiado longo
- Ajustes necessários para evitar pontuação duplicada no ecrã
- Correção de singular/plural no resultado final e na pontuação visual
- Ajustes necessários para impedir que o timer continuasse durante a pausa
- Correção da lógica antiga em que alguns peixes davam pontos ao adversário
- Necessidade de impedir sobreposição entre `PR` e `PY`
- Ajuste visual dos anzóis para substituir `H` e `h` por símbolos mais claros
- Necessidade de mostrar feedback quando um peixe era capturado
- Necessidade de permitir que o jogador apanhasse peixes que ficavam atrás dele

A maioria destes problemas já foi corrigida. O jogo está mais fluido, o menu está mais seguro, o tabuleiro está mais claro, a resposta das teclas está mais rápida, a pausa está implementada, a jogabilidade do jogador amarelo foi corrigida, os anzóis estão mais claros, existe animação de captura, os anzóis podem rodar e o código está melhor organizado.

## Próximos Passos

Os próximos objetivos do projeto são:

- Testar todos os modos de jogo de forma completa.
- Testar a rotação dos anzóis nas quatro direções.
- Testar teclas de rotação padrão e personalizadas.
- Testar a animação de captura `*` em todos os modos.
- Testar a nova pontuação nos modos solo e dois jogadores.
- Testar o bloqueio entre jogadores em todos os modos de dois jogadores.
- Testar a pausa em todos os modos de jogo.
- Melhorar as mensagens quando o jogo é interrompido pelo jogador.
- Adicionar a opção de guardar o jogo durante a partida.
- Resolver o conflito da tecla `S`, porque atualmente ela move o jogador 1 para baixo.
- Criar uma opção no menu principal para retomar um jogo guardado.
- Guardar o estado do jogo num ficheiro externo.
- Considerar níveis de dificuldade.
- Atualizar o `LOG.md` no fim de cada sessão.
- Preparar a versão final para entrega.

## Autor

Projeto desenvolvido por:

**Daniela Torres Almeida**

## Nota Final

Este projeto encontra-se em desenvolvimento. A versão atual representa uma fase funcional do jogo, com foco na estrutura base, menus, movimento, pontuação, modos de jogo, organização do código e melhoria da experiência no terminal.
