#include "stm32f4xx.h"

#include "delay.h"
#include "symbols.h"
#include "display.h"
#include "state.h"
#include "persist.h"

void alarm(bool alarm, bool alarm_silent){
	if(alarm){
		GPIOB->ODR &= ~(GPIO_ODR_OD4);
		GPIOB->ODR &= ~(GPIO_ODR_OD5);
		if(!alarm_silent){
			//GPIOB->ODR |= GPIO_ODR_OD6; //Activate alarm tone
			GPIOB->ODR &= ~(GPIO_ODR_OD4);

		}
		else{
			GPIOB->ODR &= ~(GPIO_ODR_OD6); //Deactivate alarm tone
								GPIOB->ODR |= GPIO_ODR_OD4;

		}
		delay_ms(500);
		GPIOB->ODR |= GPIO_ODR_OD5;
		delay_ms(500);
	}
	else{
		GPIOB->ODR &= ~(GPIO_ODR_OD6); //Deactivate alarm tone
		GPIOB->ODR |= GPIO_ODR_OD4;
		GPIOB->ODR &= ~(GPIO_ODR_OD5);
	}
}


int main(){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0;
	GPIOA->MODER |= GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3 | GPIO_AFRL_AFRL4); //Must be set to 0 for GPIO output mode
	GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
	GPIOB->MODER |= GPIO_MODER_MODER6_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0;
    DWT_Delay_init();
	persist_init();
	display_init();
	state_init();

	State state_buffer;

	while(1){
		update_state();
		state_buffer = state_get_state();
		draw_state(state_buffer);
		alarm(state_buffer.alarm, state_buffer.alarm_silent);

		if(!(GPIOA->IDR & GPIO_IDR_ID9) && !state_buffer.alarm){
			delay_ms(1000);
			if(!(GPIOA->IDR & GPIO_IDR_ID9) && !state_buffer.alarm){
				state_clear_current_count();
			}
		}

		if(!(GPIOA->IDR & GPIO_IDR_ID9) && state_buffer.alarm){
			delay_ms(200);
			if(!(GPIOA->IDR & GPIO_IDR_ID9) && state_buffer.alarm){
				state_set_alarm_mute(true);
			}
		}

		if(state_buffer.persist_needed){
			persist_append(state_buffer.n_wheel, state_buffer.n_wheel_current);
			state_reset_persist_needed();
		}


	}
	return 0;	 	
}
