#ifndef PITCHCONTROLLER
#define PITCHCONTROLLER

#include <stdlib.h>
#include "sensors.h"
#include "PID.h"
#include <driver/gpio.h>

void pitchController(void *parameter);
static void setup(void);
bool checkPitchInbox(int *action);
int calculateSpeedSteps(int angleDif);
void setAnglePitch(int newAngle, int currentAngle);
void setDirectionPitch(int direction);
int getRotorSpeedValue(void);
int convertPotSpeedValue(int value);
void clearScreen();

#endif