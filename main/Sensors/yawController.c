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

#define SCHEDULE_TIME 16 /* Time between running of task (10 = 1 second )*/

void yawController(void *parameter)
{
    setup();
    int steps = 0; //

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * SCHEDULE_TIME; // Time when the function needs to run.

    while (1)
    { // Task main loop
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        if (checkDirInbox(&steps))
        { // Check for a message

            if (steps < 0)
            {
                direction(RIGHT);
                steps = abs(steps);
            }
            else if (steps > 0)
            {
                direction(LEFT);
            }
            
            for (int i = 0; i < steps; i++)
            {
                if (uxQueueMessagesWaiting(yawControllerQueue))
                {
                    break;
                }
                gpio_set_level(YAW_STEP_PIN, 1); // Set step high and then low to take a step
                vTaskDelay(1); // Burn some calories, otherwise it won't rotate
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
        printf("%s Recieved from: %s, Value is: %.1f\n", pcTaskGetTaskName(NULL), message.sender, message.value);
        *action = message.value; // Set the action, this is in case the motor needs to stop.
        return true;
    }
    return false;
}

void direction(int direction)
{
    gpio_set_level(YAW_DIR_PIN, direction); // Set gpio 26 high or low. Left is low right is high.
}