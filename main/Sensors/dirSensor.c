#include "dirSensor.h"
#include <math.h>

/* Main function for direction controller */
void dirSensor(void *parameter)
{
    setup(); // Setup for the dirSensor

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * WINDDIRECTION_PERIOD; // Time when the function needs to run.

    int previousMeasurement = 0;

    while(true)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // To make the task periodic a delayUntil is needed.
        int average = 0;                             // Variable to store the value from the adc
        for (int i = 0; i < MEASUREMENTS; i++)
        {
            average += getDirSensorValue();
            vTaskDelay(WINDDIRECTION_PERIOD / MEASUREMENTS);
        }
        average = average / MEASUREMENTS;
        convertDirValue(&average);
        //int adcValue = getDirSensorValue();
        if (previousMeasurement != average)
        {
            previousMeasurement = average;                  // set value to the new value
            sendValue(previousMeasurement, dirSensorQueue); // send this value to the main function
        }
        //printf("Runnig! %d %d - ADC: %d\n", xLastWakeTime, xFrequency, previousMeasurement);
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
    //int value = adc1_get_raw(ADC1_GPIO34_CHANNEL); // Get the raw ADC value from GPIO34

    //convertDirValue(&value); // Convert value to degree
    //return value;
    return adc1_get_raw(ADC1_GPIO34_CHANNEL); // Get the raw ADC value from GPIO34
}

/* Convert value to degree for the main controller */
void convertDirValue(int *value)
{
    *value = *value / DEGREE; // Convert Value to degrees
    //*value = (*value >> 1) << 1;
    //*value = round((float)(*value << 1) / 10) * 5; // Round it to 5 degree.
}
