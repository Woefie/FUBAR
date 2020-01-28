#include "dirSensor.h"
#include <math.h>

/* Main function for direction controller */
void dirSensor(void *parameter)
{
    setup(); // Setup for the dirSensor

    TickType_t xLastWakeTime = xTaskGetTickCount();                          // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * WINDDIRECTION_PERIOD; // Time when the function needs to run.

    int previousMeasurement = 0;

    while (true)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // To make the task periodic a delayUntil is needed.
        int measurement = getDirSensorValue();                             // Variable to store the value from the adc

        convertDirValue(&measurement);

        if (previousMeasurement != measurement)
        {
            previousMeasurement = measurement;                  // set value to the new value
            sendValue(previousMeasurement, dirSensorQueue); // send this value to the main function
        }
    }
    vTaskDelete(NULL);
}

static void setup(void)
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());
    // Show starting message of task
    adc1_config_width(ADC_WIDTH_BIT_12);
    // ADC_WIDTH_BIT_12 gives a 12 bit ADC value (0-4095)
    adc1_config_channel_atten(ADC1_GPIO34_CHANNEL, ADC_ATTEN_DB_11);
    // Set GPIO34 as ADC and set attenuation to 11 DB, beceause this gives full-scale voltage of 3.9
}

/* Retrieve value from sensor, value is in this case the return value, aswell as the previous value */
int getDirSensorValue(void)
{
    int average = 0;

    for (int i = 0; i < WINDDIRECTION_MEASUREMENTS; i++)
    {
        average += adc1_get_raw(ADC1_GPIO34_CHANNEL);
        vTaskDelay(WINDDIRECTION_PERIOD / WINDDIRECTION_MEASUREMENTS);
    }

    average = average / WINDDIRECTION_MEASUREMENTS;
    return average; // Get the raw ADC value from GPIO34
}

/* Convert value to degree for the main controller */
void convertDirValue(int *value)
{
    *value = *value / ADC_TO_DEGREE; // Convert Value to degrees
}
