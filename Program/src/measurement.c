#include "stm32f4xx.h"
#include "measurement.h"
#include "display.h"


void EXTI0_IRQHandler(void){
    clear_display();
    EXTI->PR |= EXTI_PR_PR0;
}
void EXTI1_IRQHandler(void){
    clear_display();
    EXTI->PR |= EXTI_PR_PR0;
}
void EXTI2_IRQHandler(void){
  clear_display();
  EXTI->PR |= EXTI_PR_PR0;
}
void EXTI3_IRQHandler(void){
  clear_display();
  EXTI->PR |= EXTI_PR_PR0;
}
void EXTI4_IRQHandler(void){
  clear_display();
  EXTI->PR |= EXTI_PR_PR0;
}


void init_measuring(){
  
    //cli();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    //PORTA0-5 as input
    GPIOA->MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5);
    //Enable Pull-Down resistors
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0|GPIO_PUPDR_PUPDR1|GPIO_PUPDR_PUPDR2|GPIO_PUPDR_PUPDR3|GPIO_PUPDR_PUPDR4|GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1|GPIO_PUPDR_PUPDR1_1|GPIO_PUPDR_PUPDR2_1|GPIO_PUPDR_PUPDR3_1|GPIO_PUPDR_PUPDR4_1|GPIO_PUPDR_PUPDR5_1;

    //Configure external interrupts
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0|SYSCFG_EXTICR1_EXTI1|SYSCFG_EXTICR1_EXTI2|SYSCFG_EXTICR1_EXTI3);
    SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI4);
    EXTI->IMR |= EXTI_IMR_MR0|EXTI_IMR_MR1|EXTI_IMR_MR2|EXTI_IMR_MR3|EXTI_IMR_MR4;
    EXTI->RTSR |= EXTI_RTSR_TR0|EXTI_RTSR_TR1|EXTI_RTSR_TR2|EXTI_RTSR_TR3|EXTI_RTSR_TR4;
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(EXTI4_IRQn);
  /*  
    
    DDRA |= (1<<PA0)|(1<<PA1)|(1<<PA2)|(1<<PA3);

    DDRE &= ~(1<<PE0);
    PORTE |= (1<<PE0);

    DDRB &= ~(1<<PB0);
    PORTB |= (1<<PB0); 
    counter_get_total(&c_wheel);
    counter_get_current(&c_wheel_current);

    res.alarm_ignore = true;
    flag_wheel_reset = true;


    GICR |= (1<<INT2)|(1<<INT0); //Enable INT0 und INT2
    EMCUCR |= (0<<ISC2); //INT2: ISC2: 1=rising/0=falling edge
    MCUCR |= (1<<ISC01)|(1<<ISC00); //INT0 on rising edge

    TCCR0 |= (1<<CS02)|(1<<CS01)|(0<<CS00); //Timer0 external clock rising edge
    TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10); //Timer1 internal clock prescaler 1024
    OCR0 = N_FANROT; //Output compare Timer0 is N_FANROT
    TIMSK |= (1<<OCIE0)|(1<<OCIE1A)|(1<<OCIE1B);//|(1<<OCIE1A)|(1<<OCIE1B); //Enable output compare for Timer0 Timer1(A and B)
    
    GIFR = (1<<INTF0)|(1<<INTF1)|(1<<INTF2);
    sei();*/
}