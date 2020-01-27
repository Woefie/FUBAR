#include "yawController.h"
/* Connect Step to GPIO 25 and Direction to GPIO 26 
   The dirController whil recieve a value from the main controller as follows:
   - 0 turn left
   - 1 turn right
   - 2 stop
   When the direction sensor shows the turbine is at the right position it sends a stop
*/

#define YAW_STEP_PIN GPIO_NUM_18
#define YAW_DIR_PIN GPIO_NUM_19
#define STEP_DEGREE 1.8
#define PERFECT_POSITION_TOP 150
#define PERFECT_POSITION 145
#define PERFECT_POSITION_BOTTOM 140
#define GEAR_REATIO 16 // x rotations of stepper == one rotation of turbine

#define SCHEDULE_TIME 16 /* Time between running of task (10 = 1 second )*/

void yawController(void *parameter)
{
    setup();
    int position = 0; //

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * SCHEDULE_TIME; // Time when the function needs to run.

    while (1)
    { // Task main loop
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        if (checkDirInbox(&position))
        { // Check for a message

            if (position > PERFECT_POSITION)
            {
                direction(RIGHT);
            }
            else if (position < PERFECT_POSITION)
            {
                direction(LEFT);
            }
            int steps = abs((PERFECT_POSITION - position) * GEAR_REATIO / STEP_DEGREE);
            printf("Doing %d steps\n", steps);
            for (int i = 0; i < steps; i++)
            {
                if (uxQueueMessagesWaiting(yawControllerQueue))
                {
                    break;
                }
                gpio_set_level(YAW_STEP_PIN, 1); // Set step high and then low to take a step
                gpio_set_level(YAW_STEP_PIN, 0);
                vTaskDelay(1); // Burn some calories, otherwise it won't rotate
            }
        }
    }
    vTaskDelete(NULL); // Delete task when finished (just in case)
}

static void setup()
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());

    gpio_set_direction(YAW_STEP_PIN, GPIO_MODE_OUTPUT); // Set GPIO 25 to output mode
    gpio_set_direction(YAW_DIR_PIN, GPIO_MODE_OUTPUT);  // Set GPIO 26 to output mode
}

bool checkDirInbox(int *action)
{
    if (uxQueueMessagesWaiting(yawControllerQueue))
    {
        Data message;
        /* Check if a message is in the queue, to prevent starvation */
        xQueueReceive(yawControllerQueue, &message, portMAX_DELAY);
        printf("%s Recieved from: %s, Value is: %d\n", pcTaskGetTaskName(NULL), message.sender, message.value);
        //printf("Recieved from: %s, Value is: %d\n", message.sender, message.value);
        //if (message.value != STOP)
        //{                             // If the message is not the stop message, change direction.
        //    direction(message.value); // Adjust direction.
        //}
        *action = message.value; // Set the action, this is in case the motor needs to stop.
        return true;
    }
    return false;
}

void direction(int direction)
{
    gpio_set_level(YAW_DIR_PIN, direction); // Set gpio 26 high or low. Left is low right is high.
}