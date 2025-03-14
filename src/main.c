#include "stm32f4xx.h"

void delay_ms(uint32_t ms){
    for(uint32_t i=0; i < ms * 100; i++){
        __asm__("nop");
    }
}

void main(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (13*2));
    GPIOC->MODER |= GPIO_MODER_MODE13_0;//(1 << (13*2));
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
    while(1){
        GPIOC->ODR = (1 << 13);
        delay_ms(10000);
        GPIOC->ODR = ~(1 << 13);
        delay_ms(1000);
    }
}