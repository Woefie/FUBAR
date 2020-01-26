#ifndef SENSORS
#define SENSORS

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "controller.h"
#include "speedSensor.h"
#include "dirSensor.h"
#include "yawController.h"

QueueHandle_t dirSensorQueue, yawControllerQueue, speedSensorQueue;

enum move {
    RIGHT,
    LEFT,
    STOP
};


/* Data struct, this is the message information. */
typedef struct
{
    char *sender;
    int value;
} Data;

void sendValue(int value, QueueHandle_t queue);

#endif