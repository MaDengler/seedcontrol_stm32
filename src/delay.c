#include "delay.h"

void DWT_Delay_init(void){
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us){
    uint32_t start = DWT->CYCCNT;
    uint32_t Cycles = (SystemCoreClock / 1000000) * us;
    while((DWT->CYCCNT - start) < Cycles);
}

void delay_ms(uint32_t ms){
    while(ms > 0){
        delay_us(1000);
        ms--;
    }
}