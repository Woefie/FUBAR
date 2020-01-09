#ifndef DIRSENSOR
#define DIRSENSOR

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
void dirController(void *parameter);
void getSensorValue(char* value, int length);
int convertValue(int value);
void sendValue(int value);
extern QueueHandle_t dirQueue;

#endif