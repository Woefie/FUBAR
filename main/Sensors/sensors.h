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
#include "pitchController.h"

QueueHandle_t dirSensorQueue, yawControllerQueue, speedSensorQueue, pitchControllerQueue;

enum move {
    RIGHT,
    LEFT,
    STOP
};


/* Data struct, this is the message information. */
typedef struct
{
    char *sender;
    double value;
} Data;

void sendValue(double value, QueueHandle_t queue);

#endif