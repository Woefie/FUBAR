#ifndef ROTORSPEED
#define ROTORSPEED

#include <stdlib.h>
#include "sensors.h"
#include <driver/gpio.h>

void rotorSpeed(void *parameter);
static void setup();
int getRotorSpeedValue(void);

#endif