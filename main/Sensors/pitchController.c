#include "pitchController.h"

/* Connect Step to GPIO 25 and Direction to GPIO 26 
   The dirController whil recieve a value from the main controller as follows:
   - 0 turn left
   - 1 turn right
   - 2 stop
   When the direction sensor shows the turbine is at the right position it sends a stop
*/

#define PITCH_STEP_PIN GPIO_NUM_16
#define PITCH_DIR_PIN GPIO_NUM_17
// x rotations of stepper == one rotation of turbine

void pitchController(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                      // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * MOVEPITCH_PERIOD; // Time when the function needs to run.
    int currentAngle = 0, newAngle = 0;
    while (1)
    { // Task main loop

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        if (checkPitchInbox(&newAngle))
        {

            setPitchAngle(newAngle, currentAngle);
            currentAngle = newAngle;
        }
    }

    vTaskDelete(NULL); // Delete task when finished (just in case)
}

static void setup()
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());

    gpio_set_direction(PITCH_STEP_PIN, GPIO_MODE_OUTPUT); // Set GPIO to output mode
    gpio_set_direction(PITCH_DIR_PIN, GPIO_MODE_OUTPUT);  // Set GPIO to output mode

    // Set GPIO34 as ADC and set attenuation to 11 DB, beceause this gives full-scale voltage of 3.9
}

bool checkPitchInbox(int *action)
{
    if (uxQueueMessagesWaiting(pitchControllerQueue))
    {
        Data message;
        /* Check if a message is in the queue, to prevent starvation */
        xQueueReceive(pitchControllerQueue, &message, portMAX_DELAY);
        // printf("%s Recieved from: %s, Value is: %.1f\n", pcTaskGetTaskName(NULL), message.sender, message.value);

        *action = message.value; // Set the action, this is in case the motor needs to stop.
        return true;
    }
    return false;
}

void setPitchAngle(int newAngle, int currentAngle)
{
    // SetAnglePitch vergelijkt oude angle met nieuwe angle
    // 2. Check naar welke direction
    // 3. Bereken hoeveel stappen nodig zijn voor nieuwe angle
    // 4. return aantal stappen
    int angleDif, steps;
    angleDif = setDirectionPitch(newAngle, currentAngle);

    steps = abs(angleDif * PITCH_GEAR_RATIO / PITCH_STEP_DEGREE);
    for (int i = 0; i < steps; i++)
    {

        if (uxQueueMessagesWaiting(pitchControllerQueue))
        {
            break;
        }
        gpio_set_level(PITCH_STEP_PIN, 1); // Set step high and then low to take a step
        vTaskDelay(1);
        gpio_set_level(PITCH_STEP_PIN, 0);
        vTaskDelay(1);
        // Burn some calories, otherwise it won't rotate
    }
}

int setDirectionPitch(int newAngle, int currentAngle)
{
    int angleDif;

    if (newAngle > currentAngle)
    {

        gpio_set_level(PITCH_DIR_PIN, RIGHT);
        angleDif = newAngle - currentAngle;
    }
    else if (newAngle < currentAngle)
    {

        gpio_set_level(PITCH_DIR_PIN, LEFT);
        angleDif = currentAngle - newAngle;
    }
    else
    {

        gpio_set_level(PITCH_DIR_PIN, STOP);
        angleDif = 0;
    }

    return angleDif;

    // Set gpio 26 high or low. Left is low right is high.
}
