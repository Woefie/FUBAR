#ifndef PITCHCONTROLLER
#define PITCHCONTROLLER

#include <stdlib.h>
#include "sensors.h"
#include "PID.h"
#include <driver/gpio.h>

void pitchController(void *parameter);
static void setup(void);
bool checkPitchInbox(int *action);
int calculatePitchSteps(int angleDif);
void setPitchAngle(int newAngle, int currentAngle);
int setDirectionPitch(int newAngle, int currentAngle);
#endif