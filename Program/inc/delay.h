#include "stm32f4xx.h"
#include <stdint.h>

void DWT_Delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);