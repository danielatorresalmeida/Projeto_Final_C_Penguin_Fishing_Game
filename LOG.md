## Sessão 1 – 18/05/2026

**Funcionalidades implementadas:**
- Criação da estrutura inicial do projeto com os ficheiros main.c, game.c, game.h, README.md e LOG.md.
- Implementação inicial do menu principal do jogo no terminal.
- Criação das opções de jogo para modo solo, dois jogadores e prática.
- Criação das estruturas principais do jogo, como Jogador, Peixe e EstadoJogo.
- Integração inicial da biblioteca ncurses no projeto.
- Configuração do ambiente MSYS2 UCRT64 para compilar o projeto com ncurses.
- Correção do problema de compilação relacionado com o ficheiro ncurses.h.
- Compilação do projeto com o comando adequado para o ambiente usado.
- Teste inicial do programa, confirmando que o menu aparece corretamente no terminal.

**Maior dificuldade:**
A maior dificuldade foi configurar corretamente a biblioteca ncurses no Windows com MSYS2 UCRT64.

**Como resolvi (ou não resolvi):**
Resolvi instalando os pacotes necessários no MSYS2 e ajustando o comando de compilação para incluir o caminho correto da biblioteca ncurses. O programa já compila e o menu inicial já aparece. Ainda falta melhorar a fluidez do movimento durante o jogo.

**Próximo passo planeado:**
Melhorar a resposta do movimento das teclas e reduzir o tremor do ecrã durante o jogo. Para isso, pretendo testar o uso de erase(), refresh() e flushinp() dentro do ciclo principal do jogo.

**Linhas de código escritas (estimativa):**
350

## Sessão 2 - 20/05/2026

**Funcionalidades implementadas:**
- Melhoria da fluidez do jogo no terminal.
- Substituição de `clear()` por `erase()` para reduzir o tremor do ecrã.
- Alteração do ciclo principal do jogo para desenhar o tabuleiro depois das atualizações.
- Remoção do `refresh()` de dentro da função `desenharJogo()`.
- Colocação do `refresh()` no fim do ciclo principal.
- Redução do tempo de pausa com `napms(35)` para melhorar a resposta do movimento.
- Correção do problema em que o jogador deixava de se mover após alterações no ciclo do jogo.
- Correção da leitura do menu para evitar que carregar apenas em ENTER fosse interpretado como opção 0.
- Melhoria visual do tabuleiro, com bordas e representação da água.
- Ajuste da pontuação apresentada no ecrã, mostrando apenas a pontuação relevante para cada modo de jogo.
- Ajuste da legenda, mostrando apenas os elementos importantes para o modo escolhido.
- Criação de um resultado final com indicação do vencedor nos modos de dois jogadores.
- Adição da escolha de idioma no início do programa.
- Implementação de suporte para inglês e português europeu nos menus, objetivos, legendas, pontuação e resultados.
- Adição da opção 10 no menu principal para mudar o idioma.
- Adição da opção 0 no menu de escolha de idioma para sair do jogo.
- Ajuste automático das linhas dos títulos para ficarem proporcionais ao texto apresentado.
- Implementação da indicação de jogo interrompido pelo jogador quando é usada a tecla `Q`.
- Ajuste para mostrar a pontuação final mesmo quando o jogo é interrompido pelo jogador.

**Maior dificuldade:**
A maior dificuldade foi reorganizar o ciclo principal do jogo sem quebrar o movimento dos jogadores. Também houve alguns erros com chavetas no código, que fizeram partes da função `cicloJogo()` ficarem fora da função. Além disso, foi necessário corrigir problemas de compilação causados pelo executável antigo ainda estar aberto.

**Como resolvi:**
Resolvi reorganizando o ciclo do jogo para seguir uma ordem mais clara: ler a tecla, atualizar o estado do jogo, mover o peixe, verificar capturas, atualizar o tempo, desenhar o ecrã e só depois fazer `refresh()`. Também corrigi as chavetas da função `cicloJogo()` e passei a apagar o executável antigo antes de compilar quando necessário. Para o menu, melhorei a validação da opção escolhida e adicionei suporte ao idioma selecionado.

**Próximo passo planeado:**
O próximo passo será adicionar a opção de guardar o jogo durante a partida usando a tecla `S`. Depois disso, pretendo criar uma opção no menu principal para retomar um jogo guardado.

**Linhas de código escritas ou alteradas (estimativa):**
180