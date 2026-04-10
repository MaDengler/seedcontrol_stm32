#include "stm32f4xx.h"

#include "delay.h"
#include "symbols.h"
#include "display.h"
#include "state.h"
#include "persist.h"

void alarm(bool alarm, bool alarm_silent, bool alarm_silent_whole_cycle){
	static uint32_t loops_to_toggle = 0;
	if(alarm){
		GPIOB->ODR &= ~(GPIO_ODR_OD4); //Deactivate green LED

		if(alarm_silent || alarm_silent_whole_cycle){
			GPIOB->ODR &= ~(GPIO_ODR_OD6);//Deactivate alarm tone
		}
		else{
			GPIOB->ODR |= GPIO_ODR_OD6; //Activate alarm tone
		}
		
		loops_to_toggle++;
		if(loops_to_toggle == 120){
			GPIOB->ODR ^= GPIO_ODR_OD5; //Toggle red LED
			loops_to_toggle = 0;
		}

	}
	else{
		GPIOB->ODR &= ~(GPIO_ODR_OD6); //Deactivate alarm tone
		GPIOB->ODR |= GPIO_ODR_OD4; //Activate green LED
		GPIOB->ODR &= ~(GPIO_ODR_OD5); //Deactivate red LED
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

	uint32_t loops_button_pressed = 0;
	bool button_pressed = false;
	bool button_clear_done = false;
	bool button_mute_done = false;
	bool button_mute_whole_cycle_done = false;

	while(1){
		update_state();
		state_buffer = state_get_state();
		draw_state(state_buffer);
		alarm(state_buffer.alarm, state_buffer.alarm_silent, state_buffer.alarm_silent_whole_cycle);

		if(state_buffer.persist_needed){
			persist_append(state_buffer.n_wheel, state_buffer.n_wheel_current);
			state_reset_persist_needed();
		}

		button_pressed = !(GPIOA->IDR & GPIO_IDR_ID9);
		if(button_pressed){
			loops_button_pressed++;	
			if(loops_button_pressed == 25 && state_buffer.alarm && !button_mute_done){
				state_set_alarm_silent();
				button_mute_done = true;
			}
			if(loops_button_pressed == 1500 && state_buffer.alarm && !button_mute_whole_cycle_done){
				state_set_alarm_silent_whole_cycle();
				button_mute_whole_cycle_done = true;
				
				GPIOB->ODR ^= GPIO_ODR_OD4; //Toggle green LED
				GPIOB->ODR ^= GPIO_ODR_OD5; //Toggle red LED
				delay_ms(500);
				GPIOB->ODR ^= GPIO_ODR_OD4; //Toggle green LED
				GPIOB->ODR ^= GPIO_ODR_OD5; //Toggle red LED
				delay_ms(500);
				GPIOB->ODR ^= GPIO_ODR_OD4; //Toggle green LED
				GPIOB->ODR ^= GPIO_ODR_OD5; //Toggle red LED
				delay_ms(500);				
				GPIOB->ODR ^= GPIO_ODR_OD4; //Toggle green LED
				GPIOB->ODR ^= GPIO_ODR_OD5; //Toggle red LED
				delay_ms(500);
			}
			if(loops_button_pressed == 500 && !button_clear_done){
				state_clear_current_count();
				button_clear_done = true;
			}
		}else{
			loops_button_pressed = 0;
			button_clear_done = false;
			button_mute_done = false;
			button_mute_whole_cycle_done = false;
		}
	}
	return 0;	 	
}
