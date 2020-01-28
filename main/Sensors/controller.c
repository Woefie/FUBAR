#include "controller.h"
#include <unistd.h>

void controller(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                       // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * CONTROLLER_PERIOD; // Time when the function needs to run.

    Data message;

    // Structure to strore PID data and pointer to PID structure
    struct pid_controller ctrldata;
    pidC_t pid;

    // Control loop input,output and setpoint variables
    float input = 0, output = 0;
    float setpoint = MAX_SPEED;

    // Control loop gains
    float kp = 1.5, ki = 0.5, kd = 0.5;
    // Prepare PID controller for operation
    pid = pid_create(&ctrldata, &input, &output, &setpoint, kp, ki, kd);
    // Set controler output limits from 0 to 45
    pid_limits(pid, MIN_PITCH, MAX_PITCH);
    // Allow PID to compute and change output
    pid_auto(pid);
    while (true)
    {
        //clearScreen();
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

        if (uxQueueMessagesWaiting(rotorSpeedQueue))
        { /* Check if a message is in the queue, to prevent starvation */
            xQueueReceive(rotorSpeedQueue, &message, portMAX_DELAY);
            printf("Recieved from: %s, Value is: %.2f\n", message.sender, message.value);
            controlData.rotorSpeed = message.value;
            input = message.value;

            movePitch(pid);
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

void movePitch(pidC_t pid)
{

    pid_compute(pid);
    controlData.pitchAngle = *pid->output;
    sendValue(*pid->output, pitchControllerQueue);
}

void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}