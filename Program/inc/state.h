#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    float tmpArea;
    uint16_t totalArea;
    float speed;
    uint16_t fanSpeed;
    bool seederState1[3];
    bool seederState2[3];
    bool seederState3[3];
    bool seederState4[3];
    bool wheelRotating;
	bool fanRotating;
    bool alarm;
    bool alarm_ignore;
} State;

typedef struct{
    uint32_t c_wheel;
    uint32_t c_fan;
    uint32_t n_wheel;
    uint32_t n_wheel_tmp;
} Measurement;

void init_measuring();
void update_state();


#endif