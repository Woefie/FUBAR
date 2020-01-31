#include "rotorSpeed.h"

void rotorSpeed(void *parameter)
{
    setup();

    TickType_t xLastWakeTime = xTaskGetTickCount();       // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * 1; // Time when the function needs to run.

    while (true)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // To make the task periodic a delayUntil is needed.

        // Read process feedback
        sendValue(getRotorSpeedValue(), rotorSpeedQueue); //GET ROTOR SPEED
        //printf("Runnig! %d %d - ADC: %d\n", xLastWakeTime, xFrequency, previousMeasurement);
    }

    vTaskDelete(NULL); // Delete task when finished (just in case)
}
static void setup()
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());

    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);
    // Set GPIO32 as ADC and set attenuation to 11 DB, beceause this gives full-scale voltage of 3.9
}
/* Retrieve value from sensor, value is in this case the return value, aswell as the previous value */
int getRotorSpeedValue(void)
{

    int value = adc1_get_raw(ADC1_GPIO32_CHANNEL); // Get the raw ADC value from GPIO32

    value = value / 5; // Convert Value to speed

    value = (value >> 1) << 1;

    return value;
}
