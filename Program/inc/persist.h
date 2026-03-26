#define ADDRESS_FLASH ((uint32_t)0x08020000)

void persist_init();
uint32_t persist_read_total_count();
uint32_t persist_read_current_count();
void persist_append(uint32_t, uint32_t);