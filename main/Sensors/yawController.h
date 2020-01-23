#ifndef DIRCONTROLLER
#define DIRCONTROLLER

#include "sensors.h"
#include <driver/gpio.h>
void dirController(void *parameter);

void checkDirInbox(int *action);
void direction();

#endif