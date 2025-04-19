#include "stm32f4xx.h"

#include "persist.h"

static uint32_t* ptr_tmp_counter;
static uint32_t* ptr_counter;

void flash_unlock(){
    if(FLASH->CR & FLASH_CR_LOCK){
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

void flash_lock(){
    FLASH->CR |= FLASH_CR_LOCK;
}

void flash_erase_sector5(){
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_SNB;
    FLASH->CR |= FLASH_CR_SER | FLASH_CR_SNB_0 | FLASH_CR_SNB_2;
    FLASH->CR |= FLASH_CR_STRT;
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_SER; 
}

void flash_erase_sector4(){
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_SNB;
    FLASH->CR |= FLASH_CR_SER | FLASH_CR_SNB_2;
    FLASH->CR |= FLASH_CR_STRT;
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_SER; 
}

void flash_write_word(uint32_t data){
    flash_unlock();
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_PSIZE;
    FLASH->CR |= FLASH_CR_PSIZE_1;
    FLASH->CR |= FLASH_CR_PG;
    *ptr_tmp_counter = data;
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_PG;
    flash_lock();
}
/*
uint32_t flash_read_word(){
    while(FLASH->SR & FLASH_SR_BSY);
    uint32_t data = *(volatile uint32_t*) ADDRESS_FLASH_START;
    if(data == 0xffffffff)
        data = 0;
    while(FLASH->SR & FLASH_SR_BSY);
    return data;
}
*/
void persistance_init(){
    ptr_tmp_counter = (uint32_t*) ADDRESS_TMP_COUNTER_START;
    while((uint32_t)ptr_tmp_counter < ADDRESS_TMP_COUNTER_END && *ptr_tmp_counter != 0xFFFFFFFF){
        ptr_tmp_counter++;
    }

    ptr_tmp_counter = (uint32_t*) ADDRESS_TMP_COUNTER_START;
    while((uint32_t)ptr_tmp_counter < ADDRESS_TMP_COUNTER_END && *ptr_tmp_counter != 0xFFFFFFFF){
        ptr_tmp_counter++;
    }
}

void persist_tmp_counter(uint32_t data){
    __disable_irq();
    if((uint32_t)ptr_tmp_counter == ADDRESS_TMP_COUNTER_END){
        flash_unlock();
        flash_erase_sector5();
        flash_lock();
        ptr_tmp_counter = (uint32_t*)ADDRESS_TMP_COUNTER_START;
    }
    flash_write_word(data);
    __enable_irq();
}

void persist_counter(uint32_t data){
    __disable_irq();
    if((uint32_t)ptr_tmp_counter == ADDRESS_TMP_COUNTER_END){
        flash_unlock();
        flash_erase_sector4();
        flash_lock();
        ptr_tmp_counter = (uint32_t*)ADDRESS_TMP_COUNTER_START;
    }
    flash_write_word(data);
    __enable_irq();
}

uint32_t get_tmp_counter(){
    return *(ptr_tmp_counter - 1);
}

uint32_t get_counter(){
    return *(ptr_counter - 1);
}