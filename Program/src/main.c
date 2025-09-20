#include "stm32f4xx.h"

#include "delay.h"
#include "symbols.h"
#include "display.h"
#include "state.h"
#include "persist.h"

State state = {0,0,0,0,{false,true,true},{false,true,true},{false,true,true},{false,true,true},false,false,false,false};

void HardFault_Handler(void){
	while(1){
		GPIOC->ODR = ~(1 << 13);
        delay_ms(1000);
        GPIOC->ODR = (1 << 13);
		delay_ms(1000);
	}
}

void alarm(bool alarm){
	if(alarm){
		GPIOA->ODR &= ~(GPIO_ODR_OD7);
		GPIOA->ODR &= ~(GPIO_ODR_OD6);
		delay_ms(500);
		GPIOA->ODR |= GPIO_ODR_OD6;
		delay_ms(500);
	}
	else{
		GPIOA->ODR |= GPIO_ODR_OD7;
		GPIOA->ODR &= ~(GPIO_ODR_OD6);

	}
}




int main(){
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    //GPIOC->MODER &= ~(3 << (13*2));
    //GPIOC->MODER |= GPIO_MODER_MODE13_0;//(1 << (13*2));
    //GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
    //GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0;
    DWT_Delay_init();
	init_display();
	init_measuring();
	//flash_unlock();
    //flash_erase_sector5();

	while(1){
		alarm(state.wheelRotating && !(state.seederState1[0]&&state.seederState2[0]&&state.seederState3[0]&&state.seederState4[0]));

		update_state();
		draw_state();
	}
	return 0;	 	
}
