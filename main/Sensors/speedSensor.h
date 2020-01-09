#ifndef SPEEDSENSOR
#define SPEEDSENSOR

#include "sensors.h"

void speedController(void *parameter);
void getSensorValue(char *value, int length);
int convertValue(char *str);
void sendValue(int value);
extern QueueHandle_t speedQueue;

#endif