#ifndef SENSORS
#define SENSORS

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "speedSensor.h"
#include "dirSensor.h"

QueueHandle_t dirQueue, speedQueue;
/* Data struct, this is the message information. */
typedef struct
{
    char *sender;
    int value;
} Data;

void sendValue(int value);

#endif