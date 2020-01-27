#include "controller.h"

#define PERFECTYAWLOW 140
#define PERFECTYAWHIGH 150

#define SCHEDULE_TIME 10 /* Time between running of task (10 = 1 second )*/

void controller(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * SCHEDULE_TIME; // Time when the function needs to run.

    Data message;
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // To make the task periodic a delayUntil is needed.
        if (uxQueueMessagesWaiting(dirSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(dirSensorQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %d\n", message.sender, message.value);
            moveYaw(message.value);
        }

        if (uxQueueMessagesWaiting(speedSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(speedSensorQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %d\n", message.sender, message.value);
        }
    }
}

static void setup(void)
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());
}

void moveYaw(int value)
{
    if (value >= PERFECTYAWLOW && value <= PERFECTYAWHIGH)
    {
        value = (PERFECTYAWHIGH + PERFECTYAWLOW) >> 1;
    }

    if (value >= 0 && value <= 360)
    {
        sendValue(value, yawControllerQueue);
        sendValue(value, pitchControllerQueue);
    }
    else
    {
        printf("ERROR VALUE FOUND!\n");
    }
}