#include "sensors.h"

/* Send value to main controller */

void sendValue(int value)
{
    Data data;
    data.sender = pcTaskGetTaskName(NULL);
    data.value = value;

    xQueueOverwrite(speedQueue, &data);
}