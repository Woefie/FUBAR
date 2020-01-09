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
QueueHandle_t dirQueue;

void setup();

/* Create tasks */
void setup()
{
    dirQueue = xQueueCreate(1, sizeof(Data));
    if (dirQueue == NULL)
    {
        printf("Error creating the queue!\n");
    }
    xTaskCreate(
        dirController,               /*Task function */
        "Wind direction controller", /* Name of task. */
        10000,                       /* Stack size of task */
        NULL,                        /* paramter of the task */
        1,                           /* priority of the task */
        NULL                         /* Task handle to keep track of created task */
    );
}

void app_main(void)
{
    setup();
    Data data;
    for (;;)
    {
        vTaskDelay(100);
        xQueueReceive(dirQueue, &data, portMAX_DELAY);
        printf("Recieved from: %d, Value is: %d\n", data.sender, data.value);
    }
}