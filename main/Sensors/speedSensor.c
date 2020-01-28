#include "speedSensor.h"
#include "driver/pcnt.h"
#include <math.h>

#define PCNT_UNIT PCNT_UNIT_0
#define PCNT_H_LIM_VAL 1000
#define PCNT_INPUT_SIG_IO 4 // Pulse Input GPIO
//#include "Controllers/PID.h"

void speedSensor(void *parameter)
{
    setup();
    int16_t pulses = 0;
    int lastSpeed = 0; // Variables to keep track of the number of pulses and the lastSpeed

    TickType_t xLastWakeTime = xTaskGetTickCount();                   // Get tickCount, used to calculate time between running of task
    const TickType_t xFrequency = portTICK_PERIOD_MS * WINDSPEED_PERIOD; // Time when the function needs to run.

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);   // To make the task periodic a delayUntil is needed.
        pcnt_get_counter_value(PCNT_UNIT, &pulses);    // Get number of pulses from the pulse counter
        double mpersecond = convertSpeedValue(pulses); // Convert pulses to m/s
        //printf("Runnig! %d %d - PULSES: %d - m/s %.1f\n", xLastWakeTime, xFrequency, pulses, mpersecond);
        mpersecond = round(mpersecond);

        if (mpersecond != lastSpeed)
        {                                           // If speed changed
            lastSpeed = mpersecond;                 // Change last
            sendValue(lastSpeed, speedSensorQueue); // Send value to main function
        }
        /* Pause counter to reset the counter for a new measurement */
        pcnt_counter_pause(PCNT_UNIT);
        pcnt_counter_clear(PCNT_UNIT);

        /* Everything is set up, now go to counting */
        pcnt_counter_resume(PCNT_UNIT);
    }
    vTaskDelete(NULL);
}

static void setup()
{
    printf("Starting %s on core %d\n", pcTaskGetTaskName(NULL), xPortGetCoreID());
    //adc1_config_width(ADC_WIDTH_BIT_12);
    //adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);

    /* Got initialisation code from https://github.com/programonauta/pulse-counter/blob/master/main/pcnt_example_main.c */
    /* Prepare configuration for the PCNT unit */
    pcnt_config_t pcnt_config = {
        // Set PCNT input signal and control GPIOs
        .pulse_gpio_num = PCNT_INPUT_SIG_IO, // GPIO4
        .channel = PCNT_CHANNEL_0,
        .unit = PCNT_UNIT,
        // What to do on the positive / negative edge of pulse input?
        .pos_mode = PCNT_COUNT_INC, // Count up on the positive edge
        .neg_mode = PCNT_COUNT_DIS, // Keep the counter value on the negative edge
        // Set the maximum limit value
        .counter_h_lim = PCNT_H_LIM_VAL,
    };
    /* Initialize PCNT unit */
    pcnt_unit_config(&pcnt_config);

    /* Configure and enable the input filter */
    pcnt_set_filter_value(PCNT_UNIT, 100);
    pcnt_filter_enable(PCNT_UNIT);

    /* Initialize PCNT's counter */
    pcnt_counter_pause(PCNT_UNIT);
    pcnt_counter_clear(PCNT_UNIT);

    /* Everything is set up, now go to counting */
    pcnt_counter_resume(PCNT_UNIT);
}

double convertSpeedValue(int pulses)
{
    double speed;                                                 // Variable to hold the speed
    speed = (float)pulses / (WINDSPEED_PERIOD / 10) * (30.0 / 67.0); // Datasheet shows 67 pulsen per 30 m/s, so 1 pulse is 30 / 67
    return speed;                                                 // Return speed value
}
