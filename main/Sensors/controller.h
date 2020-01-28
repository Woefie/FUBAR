#ifndef CONTROLLER
#define CONTROLLER

#include "sensors.h"

/* Setting for tasks periods. 10 here equals 1 second in realtime */
#define WINDSPEED_PERIOD 50
#define WINDDIRECTION_PERIOD 20
#define MOVEPITCH_PERIOD 10
#define MOVEYAW_PERIOD 10
#define CONTROLLER_PERIOD 10

/* Settings and calibration for yaw stepper */
#define PERFECTYAWLOW 146   // Always a multiple of 2, because the sensor is rounded to multples of 2
#define PERFECTYAWHIGH 146  // Same as LOW
#define YAW_STEP_DEGREE 1.8 // Degree of 1 step on stepper
#define YAW_GEAR_RATIO 16   // x rotations of stepper == one rotation of turbine

/* Settings for measurement of wind direction */
#define ADC_TO_DEGREE 11.7           // What is one degree, sensor can only measure from 0 to 350 degree
#define WINDDIRECTION_MEASUREMENTS 5 // Number of measurements per average

static void setup(void);
void controller(void *parameter);
int calculateYawSteps(int position);
void moveYaw(int value);
int calculateSpeedSteps(int angleDif);
void setDirectionPitch(int direction);
void clearScreen();
void printData();

struct ControlData
{
    int windDirection, rotorSpeed, pitchAngle;
    double windSpeed;
} controlData;

#endif