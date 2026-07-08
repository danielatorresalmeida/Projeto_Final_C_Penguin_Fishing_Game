# LOG.md

## Sessão 1 – 18/05/2026

**Funcionalidades implementadas:**

- Criação da estrutura inicial do projeto com os ficheiros `main.c`, `game.c`, `game.h`, `README.md` e `LOG.md`.
- Implementação inicial do menu principal do jogo no terminal.
- Criação das opções de jogo para modo solo, dois jogadores e prática.
- Criação das estruturas principais do jogo, como `Jogador`, `Peixe` e `EstadoJogo`.
- Integração inicial da biblioteca ncurses no projeto.
- Configuração do ambiente MSYS2 UCRT64 para compilar o projeto com ncurses.
- Correção do problema de compilação relacionado com o ficheiro `ncurses.h`.
- Compilação do projeto com o comando adequado para o ambiente usado.
- Teste inicial do programa, confirmando que o menu aparece corretamente no terminal.

**Maior dificuldade:**

A maior dificuldade foi configurar corretamente a biblioteca ncurses no Windows com MSYS2 UCRT64.

**Como resolvi:**

Resolvi instalando os pacotes necessários no MSYS2 e ajustando o comando de compilação para incluir o caminho correto da biblioteca ncurses. Depois disso, o programa passou a compilar e o menu inicial apareceu corretamente no terminal.

**Próximo passo planeado:**

Melhorar a resposta do movimento das teclas e reduzir o tremor do ecrã durante o jogo. Para isso, pretendo testar o uso de `erase()`, `refresh()` e `flushinp()` dentro do ciclo principal do jogo.

**Linhas de código escritas (estimativa):**

350

---

## Sessão 2 – 20/05/2026

**Funcionalidades implementadas:**

- Melhoria da fluidez do jogo durante o ciclo principal.
- Redução do tremor do ecrã durante a execução do jogo.
- Correção do problema em que o jogador podia deixar de se mover corretamente.
- Melhoria da validação inicial do menu.
- Correção do problema em que carregar apenas ENTER no menu podia ser interpretado como opção 0.
- Formatação do código para melhorar a organização e a leitura.
- Melhoria da geração segura do peixe no tabuleiro.
- Adição da escolha inicial de idioma.
- Adição da opção para alterar o idioma durante o jogo.
- Tradução do menu principal e dos textos principais do jogo.
- Melhoria do resultado final apresentado ao jogador.
- Apresentação da pontuação final mesmo quando o jogo é interrompido.
- Atualização da documentação do projeto no `README.md`.
- Atualização do registo de desenvolvimento no `LOG.md`.

**Maior dificuldade:**

A maior dificuldade foi melhorar o ciclo principal do jogo sem prejudicar a leitura das teclas. O jogo precisava de ficar mais fluido, mas sem deixar de responder aos comandos do jogador.

**Como resolvi:**

Melhorei o ciclo de desenho e atualização do jogo, usando uma estrutura mais estável com limpeza do ecrã, redesenho do tabuleiro, atualização final com `refresh()` e uma pequena pausa com `napms()`. Também foram feitos ajustes para evitar que entradas vazias fossem tratadas como escolhas válidas no menu.

**Próximo passo planeado:**

Melhorar a validação dos inputs usando `strtol()` em vez de `atoi()`, para tratar melhor entradas inválidas como letras, símbolos, comandos incompletos ou ENTER vazio.

**Linhas de código escritas (estimativa):**

250

---

## Sessão 3 – 21/05/2026

**Funcionalidades implementadas:**

- Substituição de `atoi()` por `strtol()` na leitura das opções do menu.
- Correção do problema em que carregar apenas ENTER podia ser interpretado como opção 0.
- Validação de entradas inválidas como letras, símbolos e comandos incompletos.
- Adição de cores com ncurses.
- Jogador `PR` apresentado a vermelho no tabuleiro.
- Jogador `PY` apresentado a amarelo no tabuleiro.
- Peixes vermelhos, amarelos e normais desenhados com cores diferentes.
- Melhoria visual da legenda dos jogadores durante o jogo.
- Ajuste do desenho dos jogadores para permitir símbolos com dois caracteres.
- Ajuste da zona de captura para funcionar com os jogadores `PR` e `PY`.

**Maior dificuldade:**

A maior dificuldade foi adaptar o desenho dos jogadores para dois caracteres, `PR` e `PY`, porque o tabuleiro estava preparado para guardar apenas um carácter por posição.

**Como resolvi:**

Mantive o tabuleiro como uma matriz simples de caracteres e desenhei os jogadores por cima da matriz com `mvprintw()`. Assim foi possível mostrar `PR` e `PY` com cores próprias, sem complicar demasiado a estrutura principal do jogo.

Também foi criada uma função de validação com `strtol()` para confirmar se a opção escrita pelo utilizador é realmente um número válido antes de ser usada no menu.

**Próximo passo planeado:**

Melhorar ainda mais a UX visual para tornar mais claro quais peixes pertencem a cada jogador. Depois disso, preparar a funcionalidade de guardar e retomar jogo, tendo em atenção que a tecla `S` já é usada para mover o jogador 1 para baixo.

**Linhas de código escritas (estimativa):**

200