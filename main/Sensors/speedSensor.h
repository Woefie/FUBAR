#ifndef SPEEDSENSOR
#define SPEEDSENSOR

void speedSensor(void *parameter);
void getSpeedSensorValue(char *value, int length);
int convertSpeedValue(char *str);

#endif