#ifndef SPEEDSENSOR
#define SPEEDSENSOR

#include <driver/adc.h>
#include "sensors.h"

void speedSensor(void *parameter);
void getSpeedSensorValue(char *value, int length);
void convertSpeedValue(int value);

#endif