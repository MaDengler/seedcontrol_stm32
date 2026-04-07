#include "stm32f4xx.h"

#include "display.h"
#include "persist.h"
#include "state.h"
#include "delay.h"

#define MilliSECOND 16
#define N_FANROT 5
#define WHEEL_CIRCUMFERENCE 2.2
#define WORKING_WIDTH 3.0
#define T_RESET (2800 * MilliSECOND)

State state;

void EXTI0_IRQHandler(void){
    if(TIM3->CNT > 40*MilliSECOND || TIM3->CNT == 0){
        state.wheelRotating = true;
        state.n_wheel++;
        state.n_wheel_current++;
        if(state.n_wheel_current % 50 == 0 && state.n_wheel_current != 0){ 
            state.persist_needed = true;
        }
        for(int8_t i=0;i<2;i++){
            state.seederState1[i] = state.seederState1[i+1];
            state.seederState2[i] = state.seederState2[i+1];
            state.seederState3[i] = state.seederState3[i+1];
            state.seederState4[i] = state.seederState4[i+1];
        }
        state.seederState1[2] = false;
        state.seederState2[2] = false;
        state.seederState3[2] = false;
        state.seederState4[2] = false;
        if(state.alarm_prevent != 0){
            state.alarm_prevent--;
        }
        state.c_wheel = TIM3->CNT;
        TIM3->CNT = 0;
        TIM3->CR1 |= TIM_CR1_CEN;
    }    
    EXTI->PR |= EXTI_PR_PR0;
}
void EXTI1_IRQHandler(void){
    state.seederState1[2] = true;
    EXTI->PR |= EXTI_PR_PR1;
}
void EXTI2_IRQHandler(void){
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
        state.seederState1[0] = false; state.seederState1[1] = false; state.seederState1[2] = false;
        state.seederState2[0] = false; state.seederState2[1] = false; state.seederState2[2] = false;
        state.seederState3[0] = false; state.seederState3[1] = false; state.seederState3[2] = false;
        state.seederState4[0] = false; state.seederState4[1] = false; state.seederState4[2] = false;
        state.c_wheel = 0;
        state.alarm_prevent = 3;
        TIM3->CR1 &= ~(TIM_CR1_CEN);
        TIM3->CNT = 0;
        TIM3->SR &= ~TIM_SR_UIF;
    }    
}

void TIM2_IRQHandler(void){
    if(TIM2->SR & TIM_SR_UIF){
        state.c_fan = TIM4->CNT;
        state.fanRotating = true;
        TIM4->CNT = 0;
        TIM4->CR1 |= TIM_CR1_CEN;
        TIM2->SR &= ~TIM_SR_UIF;
    }
}

void TIM4_IRQHandler(void){
    if(TIM4->SR & TIM_SR_UIF){
        state.c_fan = 0;
        TIM4->CR1 &= ~(TIM_CR1_CEN);
        TIM4->CNT = 0;
        state.fanRotating = false;
        TIM4->SR &= ~TIM_SR_UIF;
    }
}
//Timer to reset the states after wheel stopped rotating
//Time to reset should be roughly 2800ms


void state_init(){
  
    //cli();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    //PORTA0-5 as input
    GPIOA->MODER &= ~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4);
    //Enable Pull-Up resistors
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
    state = (State){
        .n_wheel = persist_read_total_count(),
        .n_wheel_current = persist_read_current_count(),
        .alarm_prevent = 3
    };
}

void update_state(){
    if(state.wheelRotating && state.c_wheel != 0){
        state.speed = (WHEEL_CIRCUMFERENCE * MilliSECOND * 3600) / state.c_wheel;
    }
    else{
        state.speed = 0;
    }
    if(state.fanRotating && state.c_fan != 0){
        state.fanSpeed = (N_FANROT * MilliSECOND * 60000) / state.c_fan;
    }
    else{
        state.fanSpeed = 0;
    }
    state.currentArea = state.n_wheel_current * WORKING_WIDTH * WHEEL_CIRCUMFERENCE / 10000;
    state.totalArea = state.n_wheel * WORKING_WIDTH * WHEEL_CIRCUMFERENCE / 10000;

    bool seeder1_ok = state.seederState1[0] || state.seederState1[1] || state.seederState1[2];
    bool seeder2_ok = state.seederState2[0] || state.seederState2[1] || state.seederState2[2];
    bool seeder3_ok = state.seederState3[0] || state.seederState3[1] || state.seederState3[2];
    bool seeder4_ok = state.seederState4[0] || state.seederState4[1] || state.seederState4[2];
    bool all_seeders_ok = seeder1_ok && seeder2_ok && seeder3_ok && seeder4_ok;
    if(state.wheelRotating && !all_seeders_ok && !state.alarm_prevent){
        state.alarm = true;
    }
    if(state.wheelRotating && all_seeders_ok){
        state.alarm = false;
    }

    if(!state.alarm){
        state.alarm_silent = false;
    }
}

State state_get_state(){
    __disable_irq();
    State temp = state;
    __enable_irq();
    return temp;
}

void state_reset_persist_needed(){
    __disable_irq();
    state.persist_needed = false;
    __enable_irq();
}

void state_clear_current_count(){
    __disable_irq();
    state.n_wheel_current = 0;
    state.persist_needed = true;
    __enable_irq();
}

void state_set_alarm_mute(bool mute){
    __disable_irq();
    state.alarm_silent = mute;
    __enable_irq();
}


