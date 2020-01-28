#ifndef DIRSENSOR
#define DIRSENSOR

#include "sensors.h"
#include <driver/adc.h>


void dirSensor(void *parameter);
static void setup(void);
int getDirSensorValue(void);
void convertDirValue(int *value);


#endif