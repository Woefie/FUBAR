#include "controller.h"
#include <unistd.h>

#define PERFECTYAWLOW 146
#define PERFECTYAWHIGH 146
#define STEP_DEGREE 1.8
#define GEAR_REATIO 16 // x rotations of stepper == one rotation of turbine

void controller(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                       // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * CONTROLLER_PERIOD; // Time when the function needs to run.

    Data message;
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // To make the task periodic a delayUntil is needed.
        if (uxQueueMessagesWaiting(dirSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(dirSensorQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %.2f\n", message.sender, message.value);
            moveYaw(message.value);
        }

        if (uxQueueMessagesWaiting(speedSensorQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(speedSensorQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %.2f\n", message.sender, message.value);
        }

        if (uxQueueMessagesWaiting(rotorSpeedQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(rotorSpeedQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %.2f\n", message.sender, message.value);
                }
    }
}

static void setup(void)
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());
}

int calculateYawSteps(int position)
{
    int steps = ((((PERFECTYAWHIGH + PERFECTYAWLOW) >> 1) - position) / STEP_DEGREE) * GEAR_REATIO;

    return steps;
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

int calculateSpeedSteps(int angleDif)
{
    int steps;
    return steps = abs(angleDif * GEAR_RATIO / STEP_DEGREE);
}

void setDirectionPitch(int direction)
{
    gpio_set_level(PITCH_DIR_PIN, direction); // Set gpio 26 high or low. Left is low right is high.
}

void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}