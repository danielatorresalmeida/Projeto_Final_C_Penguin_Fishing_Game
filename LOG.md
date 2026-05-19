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