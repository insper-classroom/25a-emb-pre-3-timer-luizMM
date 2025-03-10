#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;
int piscando_r = 0;
int piscando_g = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = 1;
        else if (gpio == BTN_PIN_G)
            flag_g = 1;
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {

        if (flag_r) {
            flag_r = 0;
            piscando_r = !piscando_r;
            if (!piscando_r){
                gpio_put(LED_PIN_R, false);
            }
        }

        if (flag_g) {
            flag_g = 0;
            piscando_g = !piscando_g;
            if (!piscando_g){
                gpio_put(LED_PIN_G, false);
            }
        }

        if(piscando_r){
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
            sleep_ms(500);
        }

        if(piscando_g){
            gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G));
            sleep_ms(250);
        }

        if (!piscando_g & !piscando_r){
            sleep_ms(10);
        }
    }
}
