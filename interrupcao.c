#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "ws2812.pio.h"

// Definição dos pinos
#define LED_R 13
#define LED_G 11
#define LED_B 12
#define BUTTON_A 5
#define BUTTON_B 6
#define MATRIX_GPIO 7

// Controle do LED RGB
volatile bool led_state = false;

// Controle da Matriz 5x5
volatile int number_displayed = 0;

// Debounce para os botões
volatile uint32_t last_press_A = 0;
volatile uint32_t last_press_B = 0;
const uint32_t debounce_time = 200; // 200ms

// Padrões de números 0-9
uint32_t digit_patterns[10][25] = {
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},  // 0
    {0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},  // 1
    {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},  // 2
    {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0},  // 3
    {1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},  // 4
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1},  // 5
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}   // 9
};

// Função para piscar o LED vermelho
bool blink_led(struct repeating_timer *t) {
    led_state = !led_state;
    gpio_put(LED_R, led_state);
    return true;
}

// Função de interrupção para o botão A
void button_A_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_press_A > debounce_time) {
        number_displayed = (number_displayed + 1) % 10;
        last_press_A = current_time;
        printf("Botão A pressionado. Número exibido: %d\n", number_displayed);
    }
}

// Função de interrupção para o botão B
void button_B_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_press_B > debounce_time) {
        number_displayed = (number_displayed - 1 + 10) % 10;
        last_press_B = current_time;
        printf("Botão B pressionado. Número exibido: %d\n", number_displayed);
    }
}

// Função para configurar GPIOs
void setup_gpio() {
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A); // Pull-up interno
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, button_A_isr); // Interrupção para botão A

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B); // Pull-up interno
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, button_B_isr); // Interrupção para botão B
}

// Função para atualizar a matriz de LEDs
void update_matrix(int num, PIO pio, uint sm) {
    uint32_t colors[25] = {0};

    for (int i = 0; i < 25; i++) {
        colors[i] = digit_patterns[num][i] ? 0x00FF00 : 0x000000; // Verde para o número
    }

    for (int i = 0; i < 25; i++) {
        uint32_t color = colors[i];
        for (int j = 0; j < 24; j++) {
            bool bit = (color >> (23 - j)) & 1;
            pio_sm_put_blocking(pio, sm, bit ? 0xFFFFFFFF : 0x000000);
        }
    }
}

int main() {
    stdio_init_all();
    setup_gpio();

    PIO pio = pio0;
    uint sm = 0;
    ws2812_program_init(pio, sm, pio_add_program(pio, &ws2812_program), MATRIX_GPIO, 800000);

    struct repeating_timer timer;
    add_repeating_timer_ms(-200, blink_led, NULL, &timer);  // Manter o LED piscando

    while (1) {
        update_matrix(number_displayed, pio, sm);
        sleep_ms(100);
    }
}
