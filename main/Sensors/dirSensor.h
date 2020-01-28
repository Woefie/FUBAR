#ifndef DIRSENSOR
#define DIRSENSOR

#include "sensors.h"
#include <driver/adc.h>

/* Schedule setting */


/* Calibration defines */
#define DEGREE 11.7      /* What is one degree, sensor can only measure from 0 to 350 degree */
#define MEASUREMENTS 5   /* Number of measurements per average */


void dirSensor(void *parameter);
static void setup(void);
int getDirSensorValue(void);
void convertDirValue(int *value);


#endif