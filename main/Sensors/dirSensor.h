#ifndef DIRSENSOR
#define DIRSENSOR

#include "sensors.h"
#include <driver/adc.h>

void dirSensor(void *parameter);
int getDirSensorValue(void);
void convertDirValue(int *value);
static void setup();

#endif