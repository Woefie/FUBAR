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

static void setup();

/* Create tasks */
static void setup()
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());

    int rtos_core = (xPortGetCoreID() & 0b1) ^ 1; /* Start all RTOS functions on the other core than the main app */

    xTaskCreatePinnedToCore(
        controller,  /*Task function */
        "CONTROLLER", /* Name of task. */
        10000,       /* Stack size of task */
        NULL,        /* paramter of the task */
        1,           /* priority of the task */
        NULL,        /* Task handle to keep track of created task */
        rtos_core    /* core to run task on */
    );

    dirSensorQueue = xQueueCreate(1, sizeof(Data));
    if (dirSensorQueue == NULL)
    {
        printf("Error creating the dir queue!\n");
    }
    xTaskCreatePinnedToCore(
        dirSensor,   /*Task function */
        "DIRSENSOR", /* Name of task. */
        10000,       /* Stack size of task */
        NULL,        /* paramter of the task */
        1,           /* priority of the task */
        NULL,        /* Task handle to keep track of created task */
        rtos_core    /* core to run task on */
    );

    yawControllerQueue = xQueueCreate(1, sizeof(Data));
    if (yawControllerQueue == NULL)
    {
        printf("Error creating the yaw queue!\n");
    }
    xTaskCreatePinnedToCore(
        yawController,   /*Task function */
        "YAWCONTROLLER", /* Name of task. */
        10000,           /* Stack size of task */
        NULL,            /* paramter of the task */
        1,               /* priority of the task */
        NULL,            /* Task handle to keep track of created task */
        rtos_core        /* core to run task on */
    );

    speedSensorQueue = xQueueCreate(1, sizeof(Data));
    if (speedSensorQueue == NULL)
    {
        printf("Error creating the speed queue!\n");
    }
    xTaskCreatePinnedToCore(
        speedSensor,   /*Task function */
        "SPEEDSENSOR", /* Name of task. */
        10000,         /* Stack size of task */
        NULL,          /* paramter of the task */
        1,             /* priority of the task */
        NULL,          /* Task handle to keep track of created task */
        rtos_core      /* core to run task on */
    );

    pitchControllerQueue = xQueueCreate(1, sizeof(Data));
    if (pitchControllerQueue == NULL)
    {
        printf("Error creating the yaw queue!\n");
    }
    xTaskCreatePinnedToCore(
        pitchController,   /*Task function */
        "PITCHCONTROLLER", /* Name of task. */
        10000,           /* Stack size of task */
        NULL,            /* paramter of the task */
        1,               /* priority of the task */
        NULL,            /* Task handle to keep track of created task */
        rtos_core        /* core to run task on */
    );

}

void app_main(void)
{
    setup();
    /* Reserverd for other function that are not realtime */
    while(true)
    {
        vTaskDelay(100000);
    }
}