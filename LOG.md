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

Melhorar a resposta do movimento das teclas e reduzir o tremor do ecrã durante o jogo.

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

A maior dificuldade foi melhorar o ciclo principal do jogo sem prejudicar a leitura das teclas.

**Como resolvi:**

Melhorei o ciclo de desenho e atualização do jogo, usando uma estrutura mais estável com limpeza do ecrã, redesenho do tabuleiro, atualização final com `refresh()` e uma pequena pausa com `napms()`. Também foram feitos ajustes para evitar que entradas vazias fossem tratadas como escolhas válidas no menu.

**Próximo passo planeado:**

Melhorar a validação dos inputs usando `strtol()` em vez de `atoi()`.

**Linhas de código escritas ou alteradas (estimativa):**

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

**Próximo passo planeado:**

Melhorar ainda mais a UX visual para tornar mais claro quais peixes pertencem a cada jogador.

**Linhas de código escritas ou alteradas (estimativa):**

200

---

## Sessão 4 – 08/07/2026

**Funcionalidades implementadas:**

- Organização do código em vários ficheiros.
- Separação do menu, lógica do jogo e desenho visual.
- Redução do tamanho e complexidade do ficheiro `game.c`.
- Manutenção da validação com `strtol()`.
- Manutenção das cores dos jogadores e dos peixes.
- Pontuação apresentada conforme o modo escolhido.
- Melhoria da legenda visual durante o jogo.
- Identificação visual dos peixes `R`, `Y` e `F`.
- Adição de bordas ao tabuleiro.

**Maior dificuldade:**

A maior dificuldade foi separar o código sem alterar o comportamento do jogo.

**Como resolvi:**

Dividi o projeto em ficheiros com responsabilidades diferentes: `menu.c`, `logic.c`, `render.c` e `game.c`. Assim, o código ficou mais organizado e mais fácil de manter.

**Próximo passo planeado:**

Melhorar o resultado final, mostrando o vencedor de forma mais clara nos modos de dois jogadores.


**Linhas de código escritas ou alteradas (estimativa):**

300
---

## Sessão 5 – 08/07/2026

**Funcionalidades implementadas:**

- Adição da escolha inicial de idioma antes do menu principal.
- Menu principal apresentado de acordo com o idioma escolhido.
- Adição da opção `11 - Como jogar / How to play`.
- Explicação dos modos de jogo, regras, tipos de peixe e controlos.
- Possibilidade de mudar o idioma através da opção `10`.
- Correção da organização das funções relacionadas com o menu.
- Adição de timer visível durante o jogo.
- Apresentação do tempo restante em segundos.
- Indicação especial para os modos de prática sem limite de tempo.
- Destaque visual nos últimos 10 segundos da partida.
- Melhoria do resultado final com pontuação do modo escolhido.
- Adição da pergunta para jogar novamente após o fim da partida.
- Adição de confirmação antes de sair do jogo.

**Maior dificuldade:**

A maior dificuldade foi adaptar o menu para trabalhar com dois idiomas sem criar conflito com funções já existentes, como `nomeModo()`.

**Como resolvi:**

Separei melhor as responsabilidades entre os ficheiros e mantive a função `nomeModo()` com a assinatura original. A escolha de idioma ficou ligada ao menu e às mensagens principais do utilizador.

**Próximo passo planeado:**

Melhorar a organização visual do ecrã do jogo durante a partida.


**Linhas de código escritas ou alteradas (estimativa):**

260
---

## Sessão 6 – 08/07/2026

**Funcionalidades implementadas:**

- Reorganização visual da interface durante o jogo.
- Criação de uma estrutura de ecrã com cabeçalho, objetivo, tabuleiro, informação lateral e rodapé.
- Agrupamento da legenda e dos controlos num painel lateral.
- Colocação do objetivo do modo num painel próprio.
- Integração do timer na borda superior do tabuleiro.
- Centralização do timer em relação ao tabuleiro.
- Colocação da pontuação por cima do timer, também centrada com o tabuleiro.
- Remoção da pontuação duplicada do painel lateral.
- Correção da pontuação no topo para suportar `PR` e `PY`.
- Correção do texto da pontuação para singular e plural, como `1 peixe` e `2 peixes`.
- Correção do resultado final para mostrar singular e plural corretamente.
- Manutenção das cores dos jogadores no resultado final e no menu de jogar novamente.

**Maior dificuldade:**

A maior dificuldade foi reorganizar a interface sem quebrar a lógica do jogo e sem repetir informação no ecrã.

**Como resolvi:**

Mantive a lógica da pontuação em `logic.c` e concentrei a apresentação visual em `render.c`. Também criei funções auxiliares para calcular a pontuação visível, escolher o texto correto no singular ou plural e centralizar a pontuação e o timer em relação ao tabuleiro.

**Próximo passo planeado:**

Testar todos os modos de jogo e considerar a criação de níveis de dificuldade ou uma opção de pausa antes de implementar guardar e retomar jogo.


**Linhas de código escritas ou alteradas (estimativa):**

240
---

## Sessão 7 – 09/07/2026

**Funcionalidades implementadas:**

- Melhoria da resposta das teclas durante o jogo.
- Redução do atraso do ciclo principal de jogo.
- Leitura de várias teclas acumuladas no buffer a cada ciclo.
- Utilização da tecla mais recente quando existem várias teclas pendentes.
- Redesenho do ecrã apenas quando existe alteração no estado do jogo.
- Alteração de `atualizarJogo()` para devolver se houve ou não alteração.
- Ajuste das funções de movimento para devolverem se o jogador ou o peixe se moveu.
- Ajuste da velocidade do peixe para manter o equilíbrio após a redução do tempo de pausa.
- Pequenas otimizações na interface ncurses com `leaveok()` e `set_escdelay()`.

**Maior dificuldade:**

A maior dificuldade foi melhorar a resposta das teclas sem fazer o peixe mover-se demasiado depressa e sem redesenhar o ecrã mais vezes do que necessário.

**Como resolvi:**

Passei a ler as teclas acumuladas em cada ciclo e usei apenas a tecla mais recente. Também reduzi a pausa do ciclo principal para tornar o jogo mais responsivo. Para compensar isso, aumentei o intervalo de movimento do peixe. Além disso, `atualizarJogo()` passou a indicar quando houve alteração, permitindo redesenhar o ecrã apenas quando necessário.

**Próximo passo planeado:**

Testar todos os modos de jogo e, depois, decidir entre adicionar uma opção de pausa, níveis de dificuldade ou guardar e retomar jogo.


**Linhas de código escritas ou alteradas (estimativa):**

180

---

## Sessão 8 – 09/07/2026

**Funcionalidades implementadas:**

- Adição da tecla `P` para pausar e continuar o jogo.
- Criação de uma mensagem visual de pausa no centro do tabuleiro.
- Bloqueio do movimento dos jogadores durante a pausa.
- Bloqueio do movimento do peixe durante a pausa.
- Pausa do timer enquanto o jogo está pausado.
- Possibilidade de sair da partida com `Q` durante a pausa.
- Atualização da legenda lateral e do rodapé com a nova tecla de pausa.
- Limpeza de teclas antigas com `flushinp()` para evitar comandos acumulados ao pausar ou continuar.

**Maior dificuldade:**

A maior dificuldade foi garantir que o timer não continuava a contar durante a pausa e que o jogo não saía da pausa imediatamente por causa de teclas acumuladas no buffer.

**Como resolvi:**

Criei uma função própria para esperar durante a pausa. Durante esse período, o jogo só aceita `P` para continuar ou `Q` para sair. Também usei `flushinp()` para limpar teclas antigas e reiniciei a referência do tempo ao voltar ao jogo.

**Próximo passo planeado:**

Testar a pausa em todos os modos de jogo e depois decidir se a próxima melhoria será níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

90

---

## Sessão 9 – 09/07/2026

**Funcionalidades implementadas:**

- Correção da jogabilidade do jogador amarelo `PY` nos modos de dois jogadores.
- Separação da leitura das teclas do jogador vermelho e do jogador amarelo.
- Criação de uma estrutura de entrada para guardar a última tecla de cada jogador no mesmo ciclo.
- Alteração de `atualizarJogo()` para receber uma tecla para `PR` e outra para `PY`.
- Melhoria do comportamento quando os dois jogadores pressionam teclas ao mesmo tempo ou quase ao mesmo tempo.
- Manutenção da leitura rápida do teclado sem prejudicar o jogador amarelo.

**Maior dificuldade:**

A maior dificuldade foi corrigir a resposta do jogador amarelo sem voltar ao problema anterior de atraso nas teclas. O sistema estava a usar apenas a última tecla lida no buffer, o que podia fazer com que a tecla de um jogador substituísse a tecla do outro.

**Como resolvi:**

Passei a guardar separadamente a última tecla do jogador vermelho e a última tecla do jogador amarelo em cada ciclo. Assim, quando existem teclas acumuladas, o jogo consegue atualizar os dois jogadores no mesmo ciclo, em vez de mover apenas aquele que pressionou a tecla mais recente.

**Próximo passo planeado:**

Testar os modos de dois jogadores para confirmar que `PR` e `PY` respondem corretamente. Depois disso, a próxima melhoria pode ser adicionar níveis de dificuldade.

**Linhas de código escritas ou alteradas (estimativa):**

110

---

## Sessão 10 – 09/07/2026

**Funcionalidades implementadas:**

- Atualização da opção `Como jogar / How to play` com explicação mais detalhada da pontuação.
- Alteração da regra de pontuação nos modos de dois jogadores.
- Correção para que quem apanha o peixe receba sempre a pontuação.
- Manutenção do bónus quando o jogador apanha um peixe da sua própria cor.
- Remoção da lógica em que um peixe da cor do adversário podia dar ponto ao adversário.
- Alteração do modo **Empilhar peixes** para usar bónus positivo em vez de penalizar o jogador.
- Implementação de bloqueio entre jogadores para impedir que `PR` e `PY` ocupem a mesma posição.
- Possibilidade de bloquear o adversário ficando lado a lado no tabuleiro.
- Atualização do `README.md` com as novas regras de pontuação e bloqueio.

**Maior dificuldade:**

A maior dificuldade foi alterar a pontuação sem tornar as regras injustas ou difíceis de explicar. Também foi necessário impedir a sobreposição entre jogadores sem quebrar o movimento normal no tabuleiro.

**Como resolvi:**

Passei a aplicar a pontuação sempre ao jogador que apanha o peixe. A cor do peixe agora serve apenas como bónus para o jogador correspondente. Para o bloqueio, criei validações antes de mover cada jogador, verificando se a nova posição iria ocupar uma célula usada pelo outro jogador.

**Próximo passo planeado:**

Testar as novas regras de pontuação e bloqueio em todos os modos de dois jogadores. Depois disso, a próxima melhoria pode ser adicionar níveis de dificuldade.

**Linhas de código escritas ou alteradas (estimativa):**

120

---

## Sessão 11 – 09/07/2026

**Funcionalidades implementadas:**

- Atualização do ecrã `Como jogar / How to play` para mostrar a explicação completa da pontuação.
- Adição da secção `Pontuação por modo` dentro da ajuda do jogo.
- Explicação detalhada dos valores dos peixes `R`, `Y` e `F` no modo **Mais peixes**.
- Adição de exemplos no modo **Mais peso** para mostrar como funciona o bónus de cor.
- Explicação do modo **Empilhar peixes** com bónus de cor e valor normal para peixe neutro ou adversário.
- Explicação clara de que quem apanha o peixe recebe sempre a pontuação.
- Explicação da regra de bloqueio entre jogadores no modo de dois jogadores.
- Atualização do `README.md` para refletir as mesmas regras.

**Maior dificuldade:**

A maior dificuldade foi deixar a explicação completa sem tornar o ecrã de ajuda demasiado confuso.

**Como resolvi:**

Organizei a ajuda por secções: objetivo, modos de jogo, tipos de peixe, pontuação por modo, modo de dois jogadores e controlos. Assim, as regras ficam mais fáceis de ler e o jogador consegue entender rapidamente como cada modo funciona.

**Próximo passo planeado:**

Testar a nova ajuda no menu em português e inglês, confirmar se as regras apresentadas correspondem à lógica implementada e depois avançar para níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

95

---

## Sessão 12 – 09/07/2026

**Funcionalidades implementadas:**

- Reorganização do ecrã `Como jogar / How to play` em duas páginas.
- Criação de uma primeira página com objetivo, modos de jogo, tipos de peixe e controlos.
- Criação de uma segunda página com pontuação por modo e regras de dois jogadores.
- Redução do texto apresentado em cada ecrã para facilitar a leitura no terminal.
- Adição de limpeza simples do terminal antes de mostrar cada página da ajuda.
- Manutenção das versões em português e inglês.

**Maior dificuldade:**

A maior dificuldade foi manter todas as regras importantes sem deixar o ecrã demasiado cheio de texto.

**Como resolvi:**

Dividi a ajuda em duas páginas. A primeira página explica rapidamente como jogar e quais são os controlos. A segunda página fica dedicada à pontuação e às regras de dois jogadores. Também usei textos mais curtos e alinhados para facilitar a leitura.

**Próximo passo planeado:**

Testar a ajuda nos dois idiomas e depois avançar para níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

70
