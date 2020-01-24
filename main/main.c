/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <driver/adc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "Sensors/sensors.h"

#define PERFECTYAWLOW 140
#define PERFECTYAWHIGH 150

#define SCHEDULE_TIME 10 /* Time between running of task (10 = 1 second )*/

static void setup();
void turnYaw(int value);

/* Create tasks */
static void setup()
{
    dirSensorQueue = xQueueCreate(1, sizeof(Data));
    if (dirSensorQueue == NULL)
    {
        printf("Error creating the dir queue!\n");
    }
    xTaskCreate(
        dirSensor,   /*Task function */
        "DIRSENSOR", /* Name of task. */
        10000,       /* Stack size of task */
        NULL,        /* paramter of the task */
        1,           /* priority of the task */
        NULL         /* Task handle to keep track of created task */
    );

    yawControllerQueue = xQueueCreate(1, sizeof(Data));
    if (yawControllerQueue == NULL)
    {
        printf("Error creating the yaw queue!\n");
    }
    xTaskCreate(
        yawController,   /*Task function */
        "YAWCONTROLLER", /* Name of task. */
        10000,           /* Stack size of task */
        NULL,            /* paramter of the task */
        1,               /* priority of the task */
        NULL             /* Task handle to keep track of created task */
    );

    speedSensorQueue = xQueueCreate(1, sizeof(Data));
    if (speedSensorQueue == NULL)
    {
        printf("Error creating the speed queue!\n");
    }
    xTaskCreate(
        speedSensor,   /*Task function */
        "SPEEDSENSOR", /* Name of task. */
        10000,         /* Stack size of task */
        NULL,          /* paramter of the task */
        1,             /* priority of the task */
        NULL           /* Task handle to keep track of created task */
    );
}

void app_main(void)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * SCHEDULE_TIME; // Time when the function needs to run.

    Data dirSensor, speedSensor;
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);            // To make the task periodic a delayUntil is needed.
        if (uxQueueMessagesWaiting(dirSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(dirSensorQueue, &dirSensor, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %d\n", dirSensor.sender, dirSensor.value);
            turnYaw(dirSensor.value);
            //sendValue(dirSensor.value, yawControllerQueue);
        }

        if (uxQueueMessagesWaiting(speedSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(speedSensorQueue, &speedSensor, portMAX_DELAY);
            //printf("Recieved from: %s, Value is: %d\n", speedSensor.sender, speedSensor.value);
        }
    }
}

void turnYaw(int degree) {
    if(degree < PERFECTYAWLOW) {
        sendValue(LEFT, yawControllerQueue);
    }
    else if(degree > PERFECTYAWHIGH) {
        sendValue(RIGHT, yawControllerQueue);
    }
    else {
        sendValue(STOP, yawControllerQueue);
    }
}