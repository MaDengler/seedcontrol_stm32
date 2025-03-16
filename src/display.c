//#include "stm32f4xx.h"
#include "stm32f401xc.h"
#include <stdbool.h>

#include "symbols.h"
#include "display.h"
#include "delay.h"


void draw_speed(float val){
	char d1;
	char d2;
	char d3;	
	int tmp_val=(int)(val*10);
	
	d3 = tmp_val%10;		
	tmp_val/=10;
	d2 = tmp_val%10;
	tmp_val/=10;	
	if(tmp_val==0)
		d1 = -1;	
	else
		d1 = tmp_val%10;

	set_position(PAGESTART_TOP,PAGEEND_TOP,COLSTART_SPEED);		
	draw_symbol(select_digit(d1));
	draw_symbol(select_digit(d2));
	draw_symbol(&symbolComma);
	draw_symbol(select_digit(d3));
	draw_symbol(&symbolKMH);
}

void draw_fanSpeed(int val){
	char d1;
	char d2;
	char d3;	
	char d4;
	int tmp_val=val;
	d4 = tmp_val%10;	
	tmp_val/=10;	
	d3 = tmp_val%10;
	if(tmp_val==0){
		d3=-1;
		d2=-1;
		d1=-1;
	}
	tmp_val/=10;
	d2 = tmp_val%10;
	if(tmp_val==0){
		d2=-1;
		d1=-1;
	}
	tmp_val/=10;
	d1 = tmp_val%10;
	if(tmp_val==0)
		d1=-1;	
	
	set_position(PAGESTART_TOP,PAGEEND_TOP,COLSTART_FANSPEED);
			
	draw_symbol(select_digit(d1));
	draw_symbol(select_digit(d2));
	draw_symbol(select_digit(d3));
	draw_symbol(select_digit(d4));
	draw_symbol(&symbolUmin);
}

void draw_area(float val){
	char d1;
	char d2;
	char d3;	
	int tmp_val=(int)(val*10);
	
	d3 = tmp_val%10;		
	tmp_val/=10;
	d2 = tmp_val%10;
	tmp_val/=10;	
	if(tmp_val==0)
		d1 = -1;	
	else
		d1 = tmp_val%10;

	set_position(PAGESTART_MID,PAGEEND_MID,COLSTART_AREA);		
	draw_symbol(select_digit(d1));
	draw_symbol(select_digit(d2));
	draw_symbol(&symbolComma);
	draw_symbol(select_digit(d3));
	draw_symbol(&symbolHa);
}

void draw_areaTotal(int val){
	char d1;
	char d2;
	char d3;	
	char d4;
	int tmp_val=val;
	d4 = tmp_val%10;	
	tmp_val/=10;	
	d3 = tmp_val%10;
	if(tmp_val==0){
		d3=-1;
		d2=-1;
		d1=-1;
	}
	tmp_val/=10;
	d2 = tmp_val%10;
	if(tmp_val==0){
		d2=-1;
		d1=-1;
	}
	tmp_val/=10;
	d1 = tmp_val%10;
	if(tmp_val==0)
		d1=-1;	
	
	set_position(PAGESTART_MID,PAGEEND_MID,COLSTART_AREATOTAL);
			
	draw_symbol(select_digit(d1));
	draw_symbol(select_digit(d2));
	draw_symbol(select_digit(d3));
	draw_symbol(select_digit(d4));
	draw_symbol(&symbolHaTot);
}

void draw_fanSymbol(bool isOk){
	set_position(PAGESTART_SYMBOLBAR,PAGEEND_SYMBOLBAR,COLSTART_FANSYMB);	

	if(isOk)
		draw_symbol(&symbolFan);
	else
		draw_symbol(&symbolClear72);
}
void draw_Seeder1Symbol(bool isOk){
	set_position(PAGESTART_SYMBOLBAR,PAGEEND_SYMBOLBAR,COLSTART_S1SYMB);	

	if(isOk)
		draw_symbol(&symbolSeeder1);
	else
		draw_symbol(&symbolClear60);
}
void draw_Seeder2Symbol(bool isOk){
	set_position(PAGESTART_SYMBOLBAR,PAGEEND_SYMBOLBAR,COLSTART_S2SYMB);	

	if(isOk)
		draw_symbol(&symbolSeeder2);
	else
		draw_symbol(&symbolClear60);
}
void draw_Seeder3Symbol(bool isOk){
	set_position(PAGESTART_SYMBOLBAR,PAGEEND_SYMBOLBAR,COLSTART_S3SYMB);	

	if(isOk)
		draw_symbol(&symbolSeeder3);
	else
		draw_symbol(&symbolClear60);
}
void draw_Seeder4Symbol(bool isOk){
	set_position(PAGESTART_SYMBOLBAR,PAGEEND_SYMBOLBAR,COLSTART_S4SYMB);	

	if(isOk)
		draw_symbol(&symbolSeeder4);
	else
		draw_symbol(&symbolClear60);
}
void draw_wheel(bool wheel){
	set_position(PAGESTART_SYMBOLBAR,PAGEEND_SYMBOLBAR,COLSTART_WHEELSYMB);	

	if(wheel)
		draw_symbol(&symbolWheel);
	else
		draw_symbol(&symbolClear72);

}

void write_data(const char data){
	GPIOB->ODR |= GPIO_ODR_OD2;					//Set PORTB11 high to transmit data
	SPI2->DR = data;
	delay_ms(1);
	//while(!(SPI2->SR & SPI_SR_TXE)){;}	
}

void write_cmd(char data){
	GPIOB->ODR &= ~GPIO_ODR_OD2;					//Set PORTB11 low to transmit data	
	SPI2->DR = data;
	delay_ms(1);
	//while(!(SPI2->SR & SPI_SR_TXE)){;}
}

void init_display(){
	/**********Initialization for SPI communication**********/
	//Configure PORTB Pin 10 (Reset) PORTB Pin11 (data/cmd-switch) as output
	GPIOB->MODER &= ~(GPIO_MODER_MODER10|GPIO_MODER_MODER2);
	GPIOB->MODER |= GPIO_MODER_MODER10_0|GPIO_MODER_MODER2_0;
	//Enable Clocks for SPI and GPIOB
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//Configure GPIO registers for SPI communication (SPI2)
	GPIOB->MODER &= ~(GPIO_MODER_MODER12|GPIO_MODER_MODER13|GPIO_MODER_MODER14|GPIO_MODER_MODER15);
	GPIOB->MODER |= GPIO_MODER_MODER12_1|GPIO_MODER_MODER13_1|GPIO_MODER_MODER14_1|GPIO_MODER_MODER15_1;		
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED12|GPIO_OSPEEDR_OSPEED13|GPIO_OSPEEDR_OSPEED14|GPIO_OSPEEDR_OSPEED15);		
	GPIOB->PUPDR &= ~(GPIO_OTYPER_OT12|GPIO_OTYPER_OT13|GPIO_OTYPER_OT14|GPIO_OTYPER_OT15);
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL12_0|GPIO_AFRH_AFSEL12_2)|(GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2)|(GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2)|(GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2);
	//Configure SPI2
	SPI2->CR1 = 0;
	SPI2->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_SPE;
	SPI2->CR2 |= SPI_CR2_SSOE;
	

	GPIOB->ODR &= ~GPIO_ODR_OD10;										//reset display
	delay_ms(100);
	GPIOB->ODR |= GPIO_ODR_OD10;

	write_cmd(0xa4);
	write_cmd(0xa6);
	write_cmd(0xaf);
	write_cmd(0xfd);
	write_cmd(0x12);		
	write_cmd(0x20);  //Addressing mode: 0x00=horizontal
	write_cmd(0x01);
	
	clear_display();
	
} 															

void clear_display(){
	write_cmd(0x21);
	write_cmd(0x00);
	write_cmd(0xff);
	write_cmd(0x22);
	write_cmd(0x00);		
	write_cmd(0x07);

	for(int i=0; i<8*128; i++){
		write_data(0x00);	
	}
}

void set_position(char PageStart, char PageEnd, char ColStart){
	write_cmd(0x21);
	write_cmd(ColStart);
	write_cmd(127);
	write_cmd(0x22);
	write_cmd(PageStart);
	write_cmd(PageEnd);
}

const struct Symbol* select_digit(char digit){
	switch(digit){
		case 0: return &digit0;break;
		case 1: return &digit1;break;
		case 2: return &digit2;break;
		case 3: return &digit3;break;
		case 4: return &digit4;break;
		case 5: return &digit5;break;
		case 6: return &digit6;break;
		case 7: return &digit7;break;
		case 8: return &digit8;break;
		case 9: return &digit9;break;
		default: return &symbolClearDigit; break;
	}
}

void draw_symbol(const struct Symbol* symbol){
	for(int i=0; i<symbol->size; i++)//
		write_data(symbol->pixels[i]);
}

void lock_display(bool locked){
	write_cmd(0xfd);
	if(locked){
		write_cmd(0x16);
	}else{
		write_cmd(0x12);
	}
	
}

	
