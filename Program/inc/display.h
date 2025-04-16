#include <stdbool.h>
#include <stdint.h>

#include "state.h"

#define PAGESTART_TOP 0
#define PAGEEND_TOP 1
#define COLSTART_SPEED 4
#define COLSTART_FANSPEED 64

#define PAGESTART_MID 3
#define PAGEEND_MID 4
#define COLSTART_AREA 4
#define COLSTART_AREATOTAL  64

#define PAGESTART_SYMBOLBAR 5
#define PAGEEND_SYMBOLBAR 7
#define COLSTART_WHEELSYMB 0
#define COLSTART_S1SYMB 24
#define COLSTART_S2SYMB 44
#define COLSTART_S3SYMB 64
#define COLSTART_S4SYMB 84
#define COLSTART_FANSYMB 104


void write_data(const uint8_t);
void write_cmd(uint8_t);

void init_display();
void draw_state();
void clear_display();
void lock_display(bool);

const struct Symbol* select_digit(uint8_t);
void draw_symbol(const struct Symbol*);
void set_position(uint8_t, uint8_t,uint8_t);

void draw_speed(float);
void draw_fanSpeed(uint32_t);
void draw_area(float);
void draw_areaTotal(uint32_t);
void draw_fanSymbol(bool);
void draw_Seeder1Symbol(bool);
void draw_Seeder2Symbol(bool);
void draw_Seeder3Symbol(bool);
void draw_Seeder4Symbol(bool);
void draw_wheel(bool);
