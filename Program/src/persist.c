#include <stdbool.h>

#include "stm32f4xx.h"

#include "persist.h"

static uint32_t addr_next_entry;

typedef struct{
    uint32_t seq;
    uint32_t n_wheel;
    uint32_t n_wheel_current;
    uint32_t check;
} PersistEntry;

static PersistEntry entry_write_buffer = {0,0,0,0};
static PersistEntry entry_read_buffer;

static void flash_wait_ready(void){
    while(FLASH->SR & FLASH_SR_BSY);
}

static void flash_unlock(){
    if(FLASH->CR & FLASH_CR_LOCK){
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

static void flash_lock(){
    FLASH->CR |= FLASH_CR_LOCK;
}

static bool entry_is_erased(const PersistEntry *e){
    return e->seq == 0xFFFFFFFFu &&
           e->n_wheel == 0xFFFFFFFFu &&
           e->n_wheel_current == 0xFFFFFFFFu &&
           e->check == 0xFFFFFFFFu;
}

static bool entry_is_valid(const PersistEntry *e){
    return e->check == (e->seq ^ e->n_wheel ^ e->n_wheel_current ^ 0x5a5a5a5a);
}

static void write_buffer_update(uint32_t n_wheel, uint32_t n_wheel_current){
    entry_write_buffer.n_wheel = n_wheel;
    entry_write_buffer.n_wheel_current = n_wheel_current;
    entry_write_buffer.check = entry_write_buffer.seq ^ entry_write_buffer.n_wheel ^ entry_write_buffer.n_wheel_current ^ 0x5a5a5a5a;
}

static void persistEntry_write(uint32_t address){
    flash_unlock();
    flash_wait_ready();
    FLASH->CR &= ~FLASH_CR_PSIZE;
    FLASH->CR |= FLASH_CR_PSIZE_1;
    FLASH->CR |= FLASH_CR_PG;
    *(volatile uint32_t*) address = entry_write_buffer.seq;
    flash_wait_ready();
    *(volatile uint32_t*) (address + 4) = entry_write_buffer.n_wheel;
    flash_wait_ready();
    *(volatile uint32_t*) (address + 8) = entry_write_buffer.n_wheel_current;
    flash_wait_ready();
    *(volatile uint32_t*) (address + 12) = entry_write_buffer.check;
    flash_wait_ready();
    FLASH->CR &= ~FLASH_CR_PG;
    flash_lock();
}

static void persistEntry_read(uint32_t address){
    flash_wait_ready();
    entry_read_buffer.seq = *(volatile uint32_t*) address;
    entry_read_buffer.n_wheel = *(volatile uint32_t*) (address + 4);
    entry_read_buffer.n_wheel_current = *(volatile uint32_t*) (address + 8);
    entry_read_buffer.check = *(volatile uint32_t*) (address + 12);
}

static void flash_erase_sector5(){
    flash_unlock();
    flash_wait_ready();
    FLASH->CR &= ~FLASH_CR_SNB;
    FLASH->CR |= FLASH_CR_SER | FLASH_CR_SNB_0 | FLASH_CR_SNB_2;
    FLASH->CR |= FLASH_CR_STRT;
    flash_wait_ready();
    FLASH->CR &= ~FLASH_CR_SER; 
    flash_lock();
}

void persist_init(){
    for(uint32_t addr = ADDRESS_FLASH; addr < ADDRESS_FLASH + (128u * 1024u); addr += sizeof(PersistEntry)){
        persistEntry_read(addr);
        if(entry_read_buffer.seq > entry_write_buffer.seq && entry_is_valid(&entry_read_buffer)){
            entry_write_buffer = entry_read_buffer;
        }
        if(entry_is_erased(&entry_read_buffer)){
            addr_next_entry = addr;
            break;
        }
    }
}

void persist_append(uint32_t n_wheel, uint32_t n_wheel_current){
    entry_write_buffer.seq++;
    write_buffer_update(n_wheel, n_wheel_current);
    if(addr_next_entry >= (ADDRESS_FLASH + (128u * 1024u) - 4 * sizeof(PersistEntry))){
        flash_erase_sector5();
        addr_next_entry = ADDRESS_FLASH;
    }
    persistEntry_write(addr_next_entry);
    addr_next_entry += sizeof(PersistEntry);
}

uint32_t persist_read_total_count(){
    return entry_write_buffer.n_wheel;
}

uint32_t persist_read_current_count(){
    return entry_write_buffer.n_wheel_current;
}

void persist_clear_current_count(){
    entry_write_buffer.n_wheel_current = 0;
    persist_append(entry_write_buffer.n_wheel, entry_write_buffer.n_wheel_current);
}