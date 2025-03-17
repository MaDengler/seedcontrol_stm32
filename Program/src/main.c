#include "stm32f4xx.h"

#include "delay.h"

#include "symbols.h"
#include "display.h"

//#include "counter.h"
//#include "measurement.h"
//#include "UART_debug.h"
/*
#define ONE_SEC 7812

bool btn_press = false;
bool btn_longpress = false;

ISR(INT1_vect){
	static bool falling_edge = true;
	static unsigned int t1 = 0; 
	delay_ms(80);

	if(falling_edge){	
		MCUCR |= (1<<ISC10);
		falling_edge = false;
		t1 = TCNT1;
		GIFR = 0b10000000;

	}
	else{
		unsigned int delta_t;
		unsigned int t2 = TCNT1;
		MCUCR &= ~(1<<ISC10);
		falling_edge = true;
		if(t1 < t2){
        	delta_t = t2 - t1;
    	}
    	else{
    	    delta_t = t1 - t2;
    	}

		if(delta_t >= ONE_SEC * 3){
			btn_longpress = true;
		}
		else if((delta_t > ONE_SEC / 4) && (delta_t < ONE_SEC *3)){
			if(btn_press){
				btn_press = false;
			}
			else{
				btn_press = true;
			}
		}		
	}
}

struct Results res;

void alarm(unsigned uint8_t state){
	switch(state){
	case 0: 	
		PORTE &= ~(1<<PE2);
		PORTA &= ~(1<<PA7);
		PORTA |= (1<<PA6);
		break;
	case 1: 	
		PORTE |= (1<<PE2);
		PORTA |= (1<<PA7);
		PORTA &= ~(1<<PA6);
		break;

	case 2: 	
		PORTE &= ~(1<<PE2);
		PORTA |= (1<<PA7);
		PORTA &= ~(1<<PA6);
		break;
	default:
		break;
	}
}

void draw_all(struct Results* res){
	static struct Results prevRes = {1,1,1,0xff};

	if(res->speed != prevRes.speed)
		draw_speed(res->speed);
	if(res->currentArea != prevRes.currentArea)
		draw_area(res->currentArea);
	if(res->totalArea != prevRes.totalArea)
		draw_areaTotal(res->totalArea);
	if(res->fanSpeed != prevRes.fanSpeed)
		draw_fanSpeed(res->fanSpeed);

	if(res->fanRotating != prevRes.fanRotating)
		draw_fanSymbol(res->fanRotating);
	if(res->wheelRotating != prevRes.wheelRotating)
		draw_wheel(res->wheelRotating);
	if(res->seederStates[0] != prevRes.seederStates[0])
		draw_Seeder1Symbol(res->seederStates[0]);
	if(res->seederStates[1] != prevRes.seederStates[1])
		draw_Seeder2Symbol(res->seederStates[1]);
	if(res->seederStates[2] != prevRes.seederStates[2])
		draw_Seeder3Symbol(res->seederStates[2]);
	if(res->seederStates[3] != prevRes.seederStates[3])
		draw_Seeder4Symbol(res->seederStates[3]);

	prevRes = *res;
}
*/
void delay(){
	for(int i=0;i<160000; i++);
}

int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (13*2));
    GPIOC->MODER |= GPIO_MODER_MODE13_0;//(1 << (13*2));
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;
	__disable_irq();
    DWT_Delay_init();
	//delay_ms(100);
	init_display();
	//delay_ms(100);
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
        delay_ms(1000);

		draw_fanSymbol(true);
		delay_ms(100);	
		draw_area(25);
		delay_ms(1000);
		draw_fanSymbol(false);
		delay_ms(1000);	
		draw_area(1);
		delay_ms(100);		
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