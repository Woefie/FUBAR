#ifndef CONTROLLER
#define CONTROLLER

#include "sensors.h"
static void setup(void);
void controller(void *parameter);
void moveYaw(int value);
#endif