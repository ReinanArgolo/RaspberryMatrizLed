#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "build/generated/ws2812.pio.h"

#define NUM_LEDS 25
#define WS2812_PIN 7
#define IS_RGBW true

void set_led_color(uint32_t color, uint led_index, uint32_t *leds) {
    leds[led_index] = color;
}

int main() {
    stdio_init_all();

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    uint32_t leds[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0x00000000; // Apaga todos os LEDs
    }

    while (true) {
        for (int i = 0; i < NUM_LEDS; i++) {
            set_led_color(0x00FF0000, i, leds); // Define a cor vermelha para o LED i
            for (int j = 0; j < NUM_LEDS; j++) {
                pio_sm_put_blocking(pio, sm, leds[j] << 8u);
            }
            sleep_ms(100);
            set_led_color(0x00000000, i, leds); // Apaga o LED i
        }
    }

    return 0;
}