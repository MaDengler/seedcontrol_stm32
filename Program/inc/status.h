#include <stdint.h>
#include <stdbool.h>

struct Status{
    float currentArea;
    uint16_t totalArea;
    float speed;
    uint16_t fanSpeed;
    bool seederStates[4];
	bool wheelRotating;
	bool fanRotating;
    bool alarm;
    bool alarm_ignore;
};