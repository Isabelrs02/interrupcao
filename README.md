Projeto - Controle de Matriz de LEDs 5x5 com Botões e LEDs RGB
DESENVOLVEDORA : Isabel Rosa de Santana

Descrição

Este projeto controla uma matriz de LEDs 5x5 utilizando um microcontrolador RP2040 (ou similar), com a capacidade de exibir números de 0 a 9. O controle da matriz é feito através de dois botões, A e B, que aumentam e diminuem o número exibido, respectivamente. Além disso, um LED vermelho pisca continuamente como indicador de atividade.

Funcionalidades

Botões A e B:

O botão A aumenta o número exibido na matriz de LEDs de 0 a 9.

O botão B diminui o número exibido na matriz de LEDs de 0 a 9.


LEDs RGB:

Um LED vermelho pisca constantemente para indicar que o sistema está funcionando corretamente.

O código está preparado para controlar LEDs verde e azul, mas no momento não há lógica específica para eles.


Matriz de LEDs 5x5:

Exibe números de 0 a 9, onde cada número é representado por um padrão de LEDs acesos ou apagados.



Requisitos

Hardware:

Microcontrolador RP2040 (ou outro compatível).

Matriz de LEDs 5x5.

2 botões (A e B).

LEDs RGB (vermelho, verde e azul).


Software:

SDK do Raspberry Pi Pico.

Ferramentas de desenvolvimento C (por exemplo, gcc, make).



Conexões

LEDs:

LED vermelho: GPIO 13.

LED verde: GPIO 11.

LED azul: GPIO 12.


Botões:

Botão A: GPIO 5.

Botão B: GPIO 6.


Matriz de LEDs:

GPIO 7 (conexão com pino de dados da matriz WS2812).



Funcionamento

1. Configuração do GPIO:

Os LEDs e botões são configurados como saídas e entradas, respectivamente. Botões A e B têm resistores de pull-up internos ativados.



2. Interrupções:

O código utiliza interrupções para os botões A e B. Quando pressionados, eles alteram o número exibido na matriz de LEDs, com debounce para evitar múltiplas leituras indesejadas.



3. Controle da Matriz de LEDs:

O número exibido na matriz é alterado sempre que um botão é pressionado. O número exibido é representado por um padrão de LEDs verdes acesos ou apagados.



4. Piscar o LED Vermelho:

Um LED vermelho é piscado a cada 200 ms, utilizando uma função de temporizador repetitivo para manter o LED visível durante toda a execução do programa.




Como Usar

1. Compilação:

Compile o código utilizando o SDK do Raspberry Pi Pico ou outra ferramenta de sua preferência.



2. Carregar no Microcontrolador:

Envie o código compilado para o microcontrolador RP2040 (ou equivalente).



3. Interação:

Pressione o botão A para aumentar o número exibido na matriz de LEDs.

Pressione o botão B para diminuir o número exibido na matriz de LEDs.



4. Visualização:

O número atual será exibido na matriz de LEDs 5x5.

O LED vermelho pisca a cada 200 ms, indicando que o sistema está ativo.




Estrutura do Código

Funções Principais:

setup_gpio(): Configura os pinos GPIO para LEDs e botões.

blink_led(): Função para piscar o LED vermelho.

button_A_isr() e button_B_isr(): Funções de interrupção para os botões A e B, que alteram o número exibido.

update_matrix(): Atualiza a matriz de LEDs para exibir o número atual.


Função de Interrupção:

Os botões A e B utilizam interrupções para realizar ações com debounce, garantindo uma resposta rápida e eficiente.


Controle da Matriz WS2812:

A matriz de LEDs 5x5 é controlada com o protocolo WS2812, onde cada número é representado por um padrão de LEDs acesos ou apagados.

DEMONSTRAÇÃO 
Assista ao vídeo da placa :



