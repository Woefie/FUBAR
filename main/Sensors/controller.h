#ifndef CONTROLLER
#define CONTROLLER

#include "sensors.h"

#define WINDSPEED_PERIOD 50 /* Time between running of task (10 = 1 second )*/
#define WINDDIRECTION_PERIOD 20
#define MOVEPITCH_PERIOD 10 /* Time between running of task (10 = 1 second )*/
#define MOVEYAW_PERIOD 10
#define CONTROLLER_PERIOD 10

static void setup(void);
void controller(void *parameter);
int calculateYawSteps(int position);
void moveYaw(int value);
int calculateSpeedSteps(int angleDif);
void setDirectionPitch(int direction);
void clearScreen();
#endif