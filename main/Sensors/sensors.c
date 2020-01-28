#include "sensors.h"

/* Send value to main controller */

void sendValue(double value, QueueHandle_t queue)
{
    Data data;
    data.sender = pcTaskGetTaskName(NULL);
    data.value = value;

    xQueueOverwrite(queue, &data);
}