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

---

## Sessão 13 – 09/07/2026

**Funcionalidades implementadas:**

- Melhoria da informação apresentada no ecrã principal do jogo.
- Adição de uma regra rápida no painel de objetivo, adaptada ao modo escolhido.
- Adição da secção `PEIXE ATUAL` no painel lateral.
- Apresentação do símbolo do peixe atual com a cor correspondente.
- Apresentação do peso do peixe atual no modo **Mais peso**.
- Apresentação do ganho previsto de `PR` e `PY` para o peixe atual.
- Indicação de `bloqueio ativo` nos modos de dois jogadores.
- Ajuste da posição do rodapé para evitar sobreposição com o painel lateral.

**Maior dificuldade:**

A maior dificuldade foi acrescentar informação útil sem deixar o ecrã demasiado cheio ou difícil de ler.

**Como resolvi:**

Mantive as regras completas no menu `Como jogar` e coloquei no ecrã principal apenas informação curta e prática: regra rápida do modo, peixe atual e ganho previsto para cada jogador. Assim, o jogador consegue decidir rapidamente se vale a pena apanhar o peixe sem interromper a partida.

**Próximo passo planeado:**

Testar se a nova informação cabe bem em todos os modos e depois avançar para níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

120

---

## Sessão 14 – 09/07/2026

**Funcionalidades implementadas:**

- Melhoria da posição da informação do peixe atual no ecrã principal.
- Remoção da secção `PEIXE ATUAL` do painel lateral para reduzir ruído visual.
- Colocação do peixe atual entre a pontuação e o timer.
- Apresentação do símbolo do peixe atual com cor no centro do ecrã.
- Apresentação do peso do peixe atual no modo **Mais peso** junto ao tabuleiro.
- Apresentação do ganho previsto de `PR` e `PY` numa linha mais visível.
- Redução da altura do painel lateral, deixando-o focado em legenda e controlos.
- Deslocação ligeira do tabuleiro para criar espaço para a nova linha informativa.

**Maior dificuldade:**

A maior dificuldade foi tornar a informação do peixe atual mais visível sem aproximar demasiado o texto da pontuação ou do timer.

**Como resolvi:**

Movi a informação do peixe atual para o topo do tabuleiro, onde o jogador olha durante a partida. A pontuação ficou na primeira linha, o peixe atual na segunda linha e o timer continua integrado na borda superior do tabuleiro. Também simplifiquei o painel lateral para mostrar apenas legenda e controlos.

**Próximo passo planeado:**

Testar a nova disposição visual nos três modos de jogo e em modo solo e dois jogadores. Depois disso, decidir se a próxima melhoria será níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

85

---

## Sessão 15 – 09/07/2026

**Funcionalidades implementadas:**

- Substituição dos símbolos antigos dos anzóis `H` e `h` por símbolos direcionais.
- Adição do anzol `-<` para o jogador vermelho `PR`.
- Adição do anzol `>-` para o jogador amarelo `PY`.
- Ajuste da zona de captura para considerar os dois caracteres do anzol.
- Adição de uma animação simples com `*` quando um peixe é capturado.
- Apresentação do ganho obtido, como `+1`, `+2` ou `+5`, junto ao local da captura.
- Atualização da legenda do jogo para explicar os novos anzóis e a animação de captura.
- Atualização do `README.md` com as novas melhorias visuais e de jogabilidade.

**Maior dificuldade:**

A maior dificuldade foi trocar os anzóis de um carácter para dois caracteres sem alterar demasiado a estrutura do tabuleiro, que continua a ser uma matriz simples de caracteres.

**Como resolvi:**

Mantive o tabuleiro como uma matriz de caracteres simples e desenhei os anzóis `-<` e `>-` por cima do tabuleiro, da mesma forma que os jogadores `PR` e `PY` já são desenhados. Também guardei no estado do jogo a posição da última captura, os pontos ganhos e o jogador que capturou o peixe para mostrar a animação com `*` e o feedback de pontuação durante alguns ciclos.

**Próximo passo planeado:**

Testar a nova zona de captura com os anzóis direcionais nos modos solo e dois jogadores. Depois disso, decidir se a próxima melhoria será níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

140


---

## Sessão 16 – 09/07/2026

**Funcionalidades implementadas:**

- Alteração da cor da animação de captura `*`.
- Criação de uma cor própria para a captura, usando magenta.
- Atualização da legenda para mostrar `*` com a nova cor.
- Manutenção do feedback de pontos junto da captura.

**Maior dificuldade:**

A maior dificuldade foi garantir que o símbolo `*` não fosse confundido com os peixes `R`, `Y` e `F`.

**Como resolvi:**

Adicionei uma nova cor no `render.c`, chamada `COR_CAPTURA`, e passei a usar essa cor sempre que o símbolo `*` é desenhado no tabuleiro ou na legenda.

**Próximo passo planeado:**

Testar a visibilidade da animação em todos os modos de jogo e ajustar a duração da animação se necessário.

**Linhas de código escritas ou alteradas (estimativa):**

25

---

## Sessão 17 – 09/07/2026

**Funcionalidades implementadas:**

- Adição da rotação dos anzóis dos jogadores.
- Possibilidade de rodar o anzol 90 graus para a esquerda ou para a direita.
- Suporte para anzol virado para cima, direita, baixo e esquerda.
- Ajuste da zona de captura para funcionar nas quatro direções do anzol.
- Adição de teclas padrão de rotação: `Z/X` para `PR` e `N/M` para `PY`.
- Adição de escolha personalizada das teclas de rotação antes da partida.
- Validação para impedir teclas repetidas ou teclas já usadas pelo jogo.
- Nota ao jogador de que `W`, `A`, `S`, `D`, setas, `P` e `Q` não podem ser usadas como teclas de rotação.
- Atualização do painel lateral para mostrar as teclas de rotação escolhidas.
- Atualização do menu `Como jogar / How to play` com a nova mecânica de rotação.
- Atualização do `README.md` com as novas regras e controlos.

**Maior dificuldade:**

A maior dificuldade foi fazer o anzol rodar sem transformar o tabuleiro numa estrutura mais complexa. O jogo continua a usar uma matriz simples de caracteres, mas o anzol agora pode ocupar duas posições diferentes dependendo da direção.

**Como resolvi:**

Adicionei uma enumeração `Direcao` e guardei em cada jogador a direção atual do anzol e as teclas de rotação escolhidas. A lógica calcula as duas células ocupadas pelo anzol conforme a direção. A rotação só é aceite se o anzol continuar dentro do tabuleiro. Também adicionei validação no menu para impedir teclas repetidas ou teclas já reservadas.

**Próximo passo planeado:**

Testar a rotação dos anzóis em todos os modos de pesca, confirmar se a captura funciona nas quatro direções e depois decidir se a próxima melhoria será níveis de dificuldade ou guardar e retomar jogo.

**Linhas de código escritas ou alteradas (estimativa):**

260

---

## Sessão 18 – 09/07/2026

**Funcionalidades implementadas:**

- Adição de som quando um peixe é capturado.
- Criação do campo `tocarSomCaptura` para sinalizar capturas entre a lógica e a interface.
- Criação do campo `somAtivo` para permitir ligar ou desligar o som.
- Implementação de `tocarSomCaptura()` com `beep()` do ncurses.
- Adição de fallback visual com `flash()` quando o terminal não reproduz o beep.
- Adição da opção `12` no menu principal para ligar ou desligar o som.
- Atualização do painel lateral para mostrar se o som está ligado ou desligado.
- Atualização do `Como jogar / How to play` com a indicação do som de captura.

**Maior dificuldade:**

A maior dificuldade foi adicionar som sem misturar a lógica do jogo com a parte visual.

**Como resolvi:**

A lógica apenas marca que houve uma captura, usando `tocarSomCaptura`. Depois, o ciclo principal verifica essa marca e chama a função de som em `render.c`, mantendo a separação entre lógica, interface e controlo do jogo.

**Próximo passo planeado:**

Testar o som em diferentes terminais do Windows e decidir se vale a pena adicionar mais feedback sonoro para pausa, fim do jogo ou erro de tecla.

**Linhas de código escritas ou alteradas (estimativa):**

95

---

## Sessão 19 – 09/07/2026

**Funcionalidades implementadas:**

- Adição de som curto de confirmação ao escolher uma opção válida no menu.
- Alteração do som de captura para um efeito mais parecido com moeda.
- Criação da função `tocarSomMenu()` para separar o som do menu do som de captura.
- Atualização de `tocarSomCaptura()` para tentar usar o comando `play` do SoX.
- Manutenção de fallback com beep simples e `flash()` quando o terminal não suporta som avançado.
- Atualização da documentação com nota sobre o SoX e o comando `play`.

**Maior dificuldade:**

A maior dificuldade foi adicionar sons diferentes sem tornar o código dependente de ficheiros de áudio externos ou de uma instalação obrigatória do SoX.

**Como resolvi:**

Criei funções pequenas e separadas para cada tipo de som. O jogo verifica se o comando `play` existe e usa o som avançado quando possível. Se não existir, continua a funcionar com um beep simples, sem quebrar a jogabilidade.

**Próximo passo planeado:**

Testar os sons no MSYS2 UCRT64 e, se necessário, ajustar o volume ou a duração do som de captura.

**Linhas de código escritas ou alteradas (estimativa):**

85
---

## Sessão 20 – 09/07/2026

**Funcionalidades implementadas:**

- Correção do som de confirmação no menu principal.
- Remoção da verificação com `command -v`, que podia falhar no Windows/MSYS2.
- Alteração dos comandos de som para usarem redirecionamento compatível com Windows e Linux.
- Adição de fallback com `Beep()` no Windows quando o SoX/play não está disponível.
- Manutenção do som de captura com efeito tipo moeda quando o SoX/play está instalado.

**Maior dificuldade:**

A maior dificuldade foi garantir que o som do menu funcionava antes da interface ncurses ser iniciada. O `beep()` do ncurses nem sempre funciona antes de `initscr()`, por isso era necessário um fallback externo ao ncurses.

**Como resolvi:**

Passei a tentar tocar o som com o comando `play` diretamente. Se esse comando falhar, o jogo usa `Beep()` no Windows ou o bell do terminal noutros sistemas. Assim, o menu continua a dar feedback sonoro mesmo quando o SoX não está instalado.

**Próximo passo planeado:**

Testar o som no MSYS2 UCRT64 e confirmar se o comando `play` está disponível no terminal usado para correr o jogo.

**Linhas de código escritas ou alteradas (estimativa):**

45


---

## Sessão 21 – 09/07/2026

**Funcionalidades implementadas:**

- Correção do som de confirmação nos submenus.
- Adição de som ao escolher o idioma.
- Adição de som ao confirmar saída.
- Adição de som ao escolher jogar novamente ou sair após uma partida.
- Adição de som ao escolher usar teclas padrão ou teclas personalizadas na rotação do anzol.
- Adição de som ao aceitar teclas personalizadas de rotação válidas.
- Atualização das assinaturas de funções do menu para receberem o estado do som.
- Manutenção do som desligado quando a opção de som está desativada.

**Maior dificuldade:**

A maior dificuldade foi garantir que o som tocava em todos os pontos de decisão do programa, e não apenas no menu principal ou antes de iniciar a partida.

**Como resolvi:**

Criei uma função auxiliar simples em `menu.c` para tocar o som apenas quando o som está ligado. Depois passei o estado do som para os submenus que aceitam opções do utilizador, como idioma, confirmação de saída, jogar novamente e configuração das teclas de rotação.

**Próximo passo planeado:**

Testar todos os menus com o som ligado e desligado, para confirmar que o feedback sonoro está consistente em todo o jogo.

**Linhas de código escritas ou alteradas (estimativa):**

70

---

## Sessão 22 – 09/07/2026

**Funcionalidades implementadas:**

- Adição do `Makefile` ao projeto.
- Criação de comandos para compilar, executar, limpar, reconstruir e formatar o projeto.
- Atualização do `.gitignore` para ignorar ficheiros gerados pela compilação, como `*.o`.
- Pequena limpeza no código, removendo uma linha repetida em `logic.c`.
- Atualização do `README.md` com instruções de uso do `Makefile`.

**Maior dificuldade:**

A maior dificuldade foi garantir que o projeto continuava simples de compilar no MSYS2 UCRT64, mas sem depender de comandos longos escritos manualmente todas as vezes.

**Como resolvi:**

Criei um `Makefile` com comandos curtos e claros. Assim, `make` compila o jogo, `make run` executa, `make clean` remove ficheiros gerados e `make rebuild` recompila tudo do zero.

**Próximo passo planeado:**

Fazer uma revisão final antes da entrega, testando todos os modos de jogo e garantindo que apenas ficheiros de código e documentação são enviados para o GitHub.

**Linhas de código escritas ou alteradas (estimativa):**

35
