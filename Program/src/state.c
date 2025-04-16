#include "stm32f4xx.h"

#include "display.h"
#include "state.h"
#include "delay.h"
#include "persist.h"

#define N_ALARM_DELAY 2
#define MilliSECOND 16
#define N_FANROT 5
#define WHEEL_CIRCUMFERENCE 2.2
#define WORKING_WIDTH 3.0
#define T_RESET (2800 * MilliSECOND)

extern State state;
Measurement meas;

void EXTI0_IRQHandler(void){
    if(TIM3->CNT > 400*MilliSECOND || TIM3->CNT == 0){
        state.wheelRotating = true;
        meas.n_wheel_current++;
        for(int8_t i=0;i<N_ALARM_DELAY;i++){
            state.seederState1[i] = state.seederState1[i+1];
            state.seederState2[i] = state.seederState2[i+1];
            state.seederState3[i] = state.seederState3[i+1];
            state.seederState4[i] = state.seederState4[i+1];
        }
        state.seederState1[N_ALARM_DELAY] = false;
        state.seederState2[N_ALARM_DELAY] = false;
        state.seederState3[N_ALARM_DELAY] = false;
        state.seederState4[N_ALARM_DELAY] = false;
        meas.c_wheel = TIM3->CNT;
        TIM3->CNT = 0;
        TIM3->CR1 |= TIM_CR1_CEN;
    }    
    EXTI->PR |= EXTI_PR_PR0;
}
void EXTI1_IRQHandler(void){
    state.seederState1[2] = true;
    EXTI->PR |= EXTI_PR_PR1;
}
void EXTI2_IRQHandler(void){        //state.wheelRotating = !state.wheelRotating; //just for testing
    state.seederState2[2] = true;
    EXTI->PR |= EXTI_PR_PR2;
}
void EXTI3_IRQHandler(void){
    state.seederState3[2] = true;
    EXTI->PR |= EXTI_PR_PR3;
}
void EXTI4_IRQHandler(void){
    state.seederState4[2] = true;
    EXTI->PR |= EXTI_PR_PR4;
}

void TIM3_IRQHandler(void){
    if(TIM3->SR & TIM_SR_UIF){
        state.wheelRotating = false;
        state.seederState1[0] = false;
        state.seederState2[0] = false;
        state.seederState3[0] = false;
        state.seederState4[0] = false;
        meas.c_wheel = 0;
        TIM3->CR1 &= ~(TIM_CR1_CEN);
        TIM3->CNT = 0;
        TIM3->SR &= ~TIM_SR_UIF;
    }    
}

void TIM2_IRQHandler(void){
    if(TIM2->SR & TIM_SR_UIF){
        meas.c_fan = TIM4->CNT;
        state.fanRotating = true;
        TIM4->CNT = 0;
        TIM4->CR1 |= TIM_CR1_CEN;
        TIM2->SR &= ~TIM_SR_UIF;
    }
}

void TIM4_IRQHandler(void){
    if(TIM4->SR & TIM_SR_UIF){
        meas.c_fan = 0;
        TIM4->CR1 &= ~(TIM_CR1_CEN);
        TIM4->CNT = 0;
        state.fanRotating = false;
        TIM4->SR &= ~TIM_SR_UIF;
    }
}
//Timer to reset the states after wheel stopped rotating
/*TODO: Maybe set seederstates to false,true,true to turn display symbols on faster
Time to reset should be roughly 2800ms*/


void init_measuring(){
  
    //cli();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    //PORTA0-5 as input
    GPIOA->MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4);
    //Enable Pull-Down resistors
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0|GPIO_PUPDR_PUPDR1|GPIO_PUPDR_PUPDR2|GPIO_PUPDR_PUPDR3|GPIO_PUPDR_PUPDR4|GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0|GPIO_PUPDR_PUPDR1_0|GPIO_PUPDR_PUPDR2_0|GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR4_0;

    //Configure external interrupts
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0|SYSCFG_EXTICR1_EXTI1|SYSCFG_EXTICR1_EXTI2|SYSCFG_EXTICR1_EXTI3);
    SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI4);
    EXTI->IMR |= EXTI_IMR_MR0|EXTI_IMR_MR1|EXTI_IMR_MR2|EXTI_IMR_MR3|EXTI_IMR_MR4;
    EXTI->FTSR |= EXTI_RTSR_TR0|EXTI_RTSR_TR1|EXTI_RTSR_TR2|EXTI_RTSR_TR3|EXTI_RTSR_TR4;
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(EXTI4_IRQn);

    /*Timer TIM3 for wheel/speed measurement*/
    RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 999;
    TIM3->ARR = T_RESET;
    TIM3->CNT = 0;
    TIM3->DIER |= TIM_DIER_UIE;
    //TIM3->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM3_IRQn);

    /*Timer TIM2 for counting fan rotations*/
    GPIOA->MODER &= ~(GPIO_MODER_MODER5);
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5);
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 0;
    TIM2->ARR = N_FANROT;
    TIM2->SMCR |= TIM_SMCR_ECE;
    TIM2->CNT = 0;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);

    /*Timer TIM4 for fanspeed measurement*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    TIM4->PSC = 999;
    TIM4->ARR = T_RESET;
    TIM4->CNT = 0;
    TIM4->DIER |= TIM_DIER_UIE;
    //TIM4->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM4_IRQn);

    //Load persisted area from flash memory
    meas.n_wheel = flash_read_word();
}

void update_state(){
    if(state.wheelRotating && meas.c_wheel != 0){
        state.speed = (WHEEL_CIRCUMFERENCE * MilliSECOND * 3600) / meas.c_wheel;
    }
    else{
        state.speed = 0;
    }
    if(state.fanRotating && meas.c_fan != 0){
        state.fanSpeed = (N_FANROT * MilliSECOND * 60000) / meas.c_fan;
    }
    else{
        state.fanSpeed = 0;
    }
    state.currentArea = meas.n_wheel_current * WORKING_WIDTH * WHEEL_CIRCUMFERENCE / 10000;
    state.totalArea = meas.n_wheel * WORKING_WIDTH * WHEEL_CIRCUMFERENCE / 10000;
    if(meas.n_wheel_current % 50 == 0 && meas.n_wheel_current != 0){
        
        flash_write_word(meas.n_wheel + 49);
        meas.n_wheel += 49;
        meas.n_wheel_current++;
    }
}
