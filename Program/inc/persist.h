#define ADDRESS_FLASH_START ((uint32_t)0x08020000)
#define ADDRESS_TMP_COUNTER_START ((uint32_t)0x08020000)
#define ADDRESS_COUNTER_START ((uint32_t)0x08010000)
#define ADDRESS_TMP_COUNTER_END ((uint32_t)0x0802FFFF)
#define ADDRESS_COUNTER_END ((uint32_t)0x0801FFFF)

void flash_unlock();
void flash_lock();
void flash_erase_sector5();
void flash_write_word(uint32_t);
//uint32_t flash_read_word();

void persistance_init();
void persist_tmp_counter(uint32_t);
void persist_counter(uint32_t);
uint32_t get_tmp_counter();
uint32_t get_counter();