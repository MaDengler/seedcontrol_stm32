#define ADDRESS_FLASH ((uint32_t)0x08020000)


void flash_unlock();
void flash_lock();
void flash_erase_sector5();
void flash_write_word(uint32_t);
uint32_t flash_read_word();