#include "stm32f4xx.h"

#include "delay.h"
#include "symbols.h"
#include "display.h"
#include "state.h"

struct State state;

int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (13*2));
    GPIOC->MODER |= GPIO_MODER_MODE13_0;//(1 << (13*2));
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

    DWT_Delay_init();

	init_display();
	init_measuring();
	while(1){
		//GPIOB->ODR &= ~GPIO_ODR_OD10;										//reset display
		for(int i=0;i<16000; i++);
		delay_ms(100);
		GPIOB->ODR |= GPIO_ODR_OD10;
		//init_display();
		GPIOC->ODR = (1 << 13);
		//delay();
        delay_ms(1000);
        GPIOC->ODR = ~(1 << 13);
		//delay();
		draw_fanSymbol(false);
		draw_Seeder1Symbol(state.seederState1[0]);
		draw_Seeder2Symbol(state.seederState2[0]);
		draw_Seeder3Symbol(state.seederState3[0]);
		draw_Seeder4Symbol(state.seederState4[0]);
		draw_wheel(state.wheelRotating);
        delay_ms(1000);

		draw_fanSymbol(true);
		draw_Seeder1Symbol(true);
		draw_Seeder2Symbol(true);
		draw_Seeder3Symbol(true);
		draw_Seeder4Symbol(true);
		draw_wheel(true);
		for(int i=0; i<1010; i++){
			draw_fanSpeed(i);
			delay_ms(10);
		}
		for(int i=0; i<1010; i++){
			draw_speed((float)(i/10.0));
			delay_ms(10);
		}	
		for(int i=0; i<1010; i++){
			draw_area((float)(i/10.0));
			delay_ms(10);
		}	
		for(int i=0; i<1010; i++){
			draw_areaTotal(i);
			delay_ms(10);
		}	
	}
/*
	DDRE = (1<<PE2);
	DDRA = (1<<PA6)|(1<<PA7);
	DDRD = 0;
	PORTD = (1<<PD3)|(1<<PD2);
	//Initialization of INT1
	MCUCR = (0<<ISC10)|(1<<ISC11);
	GICR =(1<<INT1);

	unsigned uint8_t alarmState = 0;

	init_display();
	init_measuring();

	while(1){
		measurement_update();
		
		if(!res.alarm_ignore && res.wheelRotating && (!res.seederStates[0] || !res.seederStates[1] || !res.seederStates[2] || !res.seederStates[3])){//|| !res.fanRotating)){
			if(alarmState == 0){
				alarmState = 1;
			}
		}
		else{
			alarmState = 0;
		}
		if(btn_press && (alarmState == 1)){
			alarmState = 2;
		}
		alarm(alarmState);
		if(btn_longpress){
			measurement_reset_counter();
			draw_all(&res);
			btn_longpress = false;
		}
		cli();
		lock_display(false);
		draw_all(&res);
		lock_display(true);
		delay_ms(2);
		sei();

		//TODO: optimize EEPROM functions to not always use the same memory cells (__EEPROM)

		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
	}
*/
	return 0;	 	
}