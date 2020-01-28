#include "pitchController.h"

/* Connect Step to GPIO 25 and Direction to GPIO 26 
   The dirController whil recieve a value from the main controller as follows:
   - 0 turn left
   - 1 turn right
   - 2 stop
   When the direction sensor shows the turbine is at the right position it sends a stop
*/

#define PITCH_STEP_PIN GPIO_NUM_18
#define PITCH_DIR_PIN GPIO_NUM_19
#define STEP_DEGREE 1.8
#define GEAR_RATIO 16 // x rotations of stepper == one rotation of turbine
#define MAX_SPEED 50
#define MAX_PITCH 45
#define MIN_PITCH 0

void pitchController(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                      // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * MOVEPITCH_PERIOD; // Time when the function needs to run.

    // Structure to strore PID data and pointer to PID structure
    struct pid_controller ctrldata;
    pidC_t pid;

    // Control loop input,output and setpoint variables
    float input = 0, output = 0;
    float setpoint = MAX_SPEED;
    int currentAngle = 0;

    // Control loop gains
    float kp = 0.2, ki = 0.2, kd = 0.2;
    // Prepare PID controller for operation
    pid = pid_create(&ctrldata, &input, &output, &setpoint, kp, ki, kd);
    // Set controler output limits from 0 to 45
    pid_limits(pid, MIN_PITCH, MAX_PITCH);
    // Allow PID to compute and change output
    pid_auto(pid);

    while (1)
    { // Task main loop

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        if (pid_need_compute(pid))
        {

            int rotorSpeed = getRotorSpeedValue();
            // Read process feedback
            input = convertPotSpeedValue(rotorSpeed); //GET ROTOR SPEED
            // Set the current angle for the rotor
            currentAngle = output;

            // Compute new PID output value;
            pid_compute(pid);
            //Change actuator value
            setAnglePitch(output, currentAngle);
            currentAngle = output;
            printf("Current angle: %d\n", currentAngle);
        }
    }
    vTaskDelete(NULL); // Delete task when finished (just in case)
}

static void setup()
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());

    gpio_set_direction(PITCH_STEP_PIN, GPIO_MODE_OUTPUT); // Set GPIO to output mode
    gpio_set_direction(PITCH_DIR_PIN, GPIO_MODE_OUTPUT);  // Set GPIO to output mode

    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);
    // Set GPIO34 as ADC and set attenuation to 11 DB, beceause this gives full-scale voltage of 3.9
}

bool checkPitchInbox(int *action)
{
    if (uxQueueMessagesWaiting(pitchControllerQueue))
    {
        Data message;
        /* Check if a message is in the queue, to prevent starvation */
        xQueueReceive(pitchControllerQueue, &message, portMAX_DELAY);
        printf("%s Recieved from: %s, Value is: %.1f\n", pcTaskGetTaskName(NULL), message.sender, message.value);

        *action = message.value; // Set the action, this is in case the motor needs to stop.
        return true;
    }
    return false;
}

void setAnglePitch(int newAngle, int currentAngle)
{
    // SetAnglePitch vergelijkt oude angle met nieuwe angle
    // 2. Check naar welke direction
    // 3. Bereken hoeveel stappen nodig zijn voor nieuwe angle
    // 4. return aantal stappen
    int angleDif, steps;

    if (newAngle > currentAngle)
    {
        setDirectionPitch(RIGHT);
        angleDif = newAngle - currentAngle;
    }
    else if (newAngle < currentAngle)
    {
        setDirectionPitch(LEFT);
        angleDif = currentAngle - newAngle;
    }
    else
    {
        setDirectionPitch(STOP);
        angleDif = 0;
    }

    printf("Angle change: %d\n", angleDif);

    steps = calculateSpeedSteps(angleDif);
    for (int i = 0; i < steps; i++)
    {
        if (uxQueueMessagesWaiting(pitchControllerQueue))
        {
            break;
        }
        gpio_set_level(PITCH_STEP_PIN, 1); // Set step high and then low to take a step
        gpio_set_level(PITCH_STEP_PIN, 0);
        vTaskDelay(1); // Burn some calories, otherwise it won't rotate
    }
}

