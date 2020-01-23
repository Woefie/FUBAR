#ifndef SPEEDSENSOR
#define SPEEDSENSOR

#include <driver/adc.h>
#include "sensors.h"

void speedSensor(void *parameter);
static void setup(void);
double convertSpeedValue(int pulses);

#endif