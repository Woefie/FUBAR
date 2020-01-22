#ifndef DIRSENSOR
#define DIRSENSOR

#include "sensors.h"
#include <driver/adc.h>

void dirSensor(void *parameter);
void getDirSensorValue(char *value, int length);
void convertDirValue(int value);

#endif