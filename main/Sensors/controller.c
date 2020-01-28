#include "controller.h"

void controller(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                       // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * CONTROLLER_PERIOD; // Time when the function needs to run.

    Data message;
    while(true)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // To make the task periodic a delayUntil is needed.
        if (uxQueueMessagesWaiting(dirSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(dirSensorQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %.1f\n", message.sender, message.value);
            controlData.windDirection = (int)message.value;
            moveYaw(message.value);
        }

        if (uxQueueMessagesWaiting(speedSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(speedSensorQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %.1f\n", message.sender, message.value);
            controlData.windSpeed = message.value;
        }
        printData();
    }
}

static void setup(void)
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());
}

int calculateYawSteps(int position)
{
    int steps = (((PERFECTYAWHIGH + PERFECTYAWLOW) >> 1) - position) / YAW_STEP_DEGREE;

    steps = steps * YAW_GEAR_RATIO;

    return steps;
}

void printData()
{
    printf("Wind direction: %d, Wind speed: %.1f, Rotor speed: %d, Pitch angle: %d\n", controlData.windDirection, controlData.windSpeed, controlData.rotorSpeed, controlData.pitchAngle);
}

void moveYaw(int value)
{
    if (value >= PERFECTYAWLOW && value <= PERFECTYAWHIGH)
    {
        value = (PERFECTYAWHIGH + PERFECTYAWLOW) >> 1;
    }

    if (value >= 0 && value <= 360)
    {
        sendValue(calculateYawSteps(value), yawControllerQueue);
    }
    else
    {
        printf("ERROR VALUE FOUND!\n");
    }
}