#ifndef DIRSENSOR
#define DIRSENSOR

#include "sensors.h"

void dirController(void *parameter);
void getSensorValue(char *value, int length);
int convertValue(int value);
void sendValue(int value);
extern QueueHandle_t dirQueue;

#endif