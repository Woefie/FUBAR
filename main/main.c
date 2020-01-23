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

/* Initialisation of direction queue. Queue is used as massage parser */

void setup();
void yawControl(int value);

/* Create tasks */
void setup()
{
    dirSensorQueue = xQueueCreate(1, sizeof(Data));
    if (dirSensorQueue == NULL)
    {
        printf("Error creating the dir queue!\n");
    }
    xTaskCreate(
        dirSensor, /*Task function */
        "DIRSENSOR",        /* Name of task. */
        10000,         /* Stack size of task */
        NULL,          /* paramter of the task */
        1,             /* priority of the task */
        NULL           /* Task handle to keep track of created task */
    );

    dirControllerQueue = xQueueCreate(1, sizeof(Data));
    if (dirControllerQueue == NULL)
    {
        printf("Error creating the yaw queue!\n");
    }
    xTaskCreate(
        dirController, /*Task function */
        "dirCONTROLLER",        /* Name of task. */
        10000,         /* Stack size of task */
        NULL,          /* paramter of the task */
        1,             /* priority of the task */
        NULL           /* Task handle to keep track of created task */
    );

    speedSensorQueue = xQueueCreate(1, sizeof(Data));
    if (speedSensorQueue == NULL)
    {
        printf("Error creating the speed queue!\n");
    }
    xTaskCreate(
        speedSensor, /*Task function */
        "SPEEDSENSOR",          /* Name of task. */
        10000,           /* Stack size of task */
        NULL,            /* paramter of the task */
        1,               /* priority of the task */
        NULL             /* Task handle to keep track of created task */
    );
}

void app_main(void)
{
    setup();
    Data dirSensor, speedSensor;
    for (;;)
    {
        vTaskDelay(100);
        if(uxQueueMessagesWaiting(dirSensorQueue)) { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(dirSensorQueue, &dirSensor, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %d\n", dirSensor.sender, dirSensor.value);
            //yawControl(dirSensor.value);
        }

        if(uxQueueMessagesWaiting(speedSensorQueue)) { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(speedSensorQueue, &speedSensor, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %d\n", speedSensor.sender, speedSensor.value);
        }
    }
}

void yawControl(int value) {
    if (value == 0) {
        sendValue(STOP, dirControllerQueue);
    }
    else if(value < 0) {
        sendValue(RIGHT, dirControllerQueue);
    }
    else if(value > 0) {
        sendValue(LEFT, dirControllerQueue);
    }
}