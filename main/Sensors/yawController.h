#ifndef DIRCONTROLLER
#define DIRCONTROLLER

#include "sensors.h"
#include <driver/gpio.h>
void yawController(void *parameter);
static void setup(void);
void checkDirInbox(int *action);

void direction();

#endif