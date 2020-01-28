#include "pitchController.h"
#include <unistd.h>

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

#define SCHEDULE_TIME 10 /* Time between running of task (10 = 1 second )*/

void pitchController(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * SCHEDULE_TIME; // Time when the function needs to run.

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
        clearScreen();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        // Check if need to compute PID
        if (pid_need_compute(pid))
        {
            int rotorSpeed = getRotorSpeedValue();
            // Read process feedback
            input = convertPotSpeedValue(rotorSpeed); //GET ROTOR SPEED
            // Set the current angle for the rotor

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

int calculateSpeedSteps(int angleDif)
{
    int steps;
    return steps = abs(angleDif * GEAR_RATIO / STEP_DEGREE);
}

void setDirectionPitch(int direction)
{
    gpio_set_level(PITCH_DIR_PIN, direction); // Set gpio 26 high or low. Left is low right is high.
}

/* Retrieve value from sensor, value is in this case the return value, aswell as the previous value */
int getRotorSpeedValue(void)
{
    //int value = adc1_get_raw(ADC1_GPIO34_CHANNEL); // Get the raw ADC value from GPIO34

    //convertDirValue(&value); // Convert value to degree
    //return value;
    return adc1_get_raw(ADC1_GPIO32_CHANNEL); // Get the raw ADC value from GPIO34
}

/* Convert value to degree for the main controller */
int convertPotSpeedValue(int value)
{

    value = value / 40; // Convert Value to degrees

    value = (value >> 1) << 1;
    //*value = round((float)(*value << 1) / 10) * 5; // Round it to 5 degree.
    printf("Speed of rotor: %d\n", value);
    return value;
}
void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}