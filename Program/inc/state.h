#include <stdint.h>
#include <stdbool.h>

struct State{
    float currentArea;
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
};

void init_measuring();