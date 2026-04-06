#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

/*
 * Test-Harness for seedcontrol_stm32
 * 
 * Pin mapping (Tester → Seedcontrol):
 *   PA0 → PA0  (Wheel sensor)
 *   PA1 → PA1  (Seeder 1)
 *   PA2 → PA2  (Seeder 2)
 *   PA3 → PA3  (Seeder 3)
 *   PA4 → PA4  (Seeder 4)
 *   PA5 → PA5  (Fan pulses / TIM2 ext clock)
 *
 * All outputs directly active low (accent) via pulldown.
 * Active low pulse of ~1ms.
 */

#define PIN_WHEEL   GPIO_ODR_OD0
#define PIN_SEEDER1 GPIO_ODR_OD1
#define PIN_SEEDER2 GPIO_ODR_OD2
#define PIN_SEEDER3 GPIO_ODR_OD3
#define PIN_SEEDER4 GPIO_ODR_OD4
#define PIN_FAN     GPIO_ODR_OD5
#define PIN_ALL_SEEDERS (PIN_SEEDER1 | PIN_SEEDER2 | PIN_SEEDER3 | PIN_SEEDER4)

static volatile uint32_t ticks;

void SysTick_Handler(void){
    ticks++;
}

static void systick_init(void){
    SysTick_Config(16000); // 1ms @ 16MHz HSI
    ticks = 0;
}

static void delay_ms(uint32_t ms){
    uint32_t start = ticks;
    while((ticks - start) < ms);
}

static void pulse_low(uint32_t pins, uint32_t duration_ms) __attribute__((unused));
static void pulse_low(uint32_t pins, uint32_t duration_ms){
    GPIOA->ODR &= ~pins;
    delay_ms(duration_ms);
    GPIOA->ODR |= pins;
    delay_ms(1);
}

static void gpio_init(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // PA0-PA5 as push-pull outputs
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 |
                       GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
    GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
                     GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0;
    // Start all high (seedcontrol inputs have pull-ups, falling edge triggers)
    GPIOA->ODR |= PIN_WHEEL | PIN_ALL_SEEDERS | PIN_FAN;
}

int main(void){
    gpio_init();
    systick_init();
    delay_ms(500); // Wait for seedcontrol to boot


    while(1){

        for(int i = 0; i < 65; i++){
            delay_ms(800);
            pulse_low(PIN_WHEEL, 80); // Simulate wheel rotation
            delay_ms(80);
            pulse_low(PIN_SEEDER1, 50); // Simulate seeder 1 active
            pulse_low(PIN_SEEDER2, 50); // Simulate seeder 2 active
            pulse_low(PIN_SEEDER3, 50); // Simulate seeder 3 active
            pulse_low(PIN_SEEDER4, 50); // Simulate seeder 4 active
            delay_ms(800);
            pulse_low(PIN_WHEEL, 80); // Simulate wheel rotation

            delay_ms(80);
        }
        for(int i = 0; i < 250; i++){
            pulse_low(PIN_FAN, 5); // Simulate fan rotation
            delay_ms(15);
        }
    }

    return 0;

}
