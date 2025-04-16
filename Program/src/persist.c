#include "stm32f4xx.h"

#include "persist.h"

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

void flash_write_word(uint32_t data){
    flash_unlock();
    flash_erase_sector5();
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_PSIZE;
    FLASH->CR |= FLASH_CR_PSIZE_1;
    FLASH->CR |= FLASH_CR_PG;
    *(volatile uint32_t*) ADDRESS_FLASH = data;
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR &= ~FLASH_CR_PG;
    flash_lock();
}

uint32_t flash_read_word(){
    while(FLASH->SR & FLASH_SR_BSY);
    uint32_t value = *(volatile uint32_t*) ADDRESS_FLASH;
    if(value == 0xffffffff)
        value = 0;
    while(FLASH->SR & FLASH_SR_BSY);

    return value;
}