#ifndef SENSORS
#define SENSORS

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "dirSensor.h"

/* Data struct, this is the message information. */
typedef struct
{
    char* sender;
    int value;
} Data;

#endif