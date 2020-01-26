#ifndef DIRCONTROLLER
#define DIRCONTROLLER

#include <stdlib.h>
#include "sensors.h"
#include <driver/gpio.h>
void yawController(void *parameter);
static void setup(void);
bool checkDirInbox(int *action);

void direction();

#endif