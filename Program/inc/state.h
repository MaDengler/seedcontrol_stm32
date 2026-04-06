#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    float currentArea;
    uint32_t totalArea;
    float speed;
    uint32_t fanSpeed;
    bool seederState1[3];
    bool seederState2[3];
    bool seederState3[3];
    bool seederState4[3];
    bool wheelRotating;
	bool fanRotating;
    bool persist_needed;
    bool alarm;
    bool alarm_silent;
    uint32_t alarm_prevent;


    uint32_t c_wheel;
    uint32_t c_fan;
    uint32_t n_wheel;
    uint32_t n_wheel_current;
} State;


void state_init();
void update_state();

void state_reset_persist_needed();
State state_get_state();
void state_clear_current_count();
void state_set_alarm_mute(bool);


#endif