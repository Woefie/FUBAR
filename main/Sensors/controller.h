#ifndef CONTROLLER
#define CONTROLLER

#include "sensors.h"
#include "PID.h"

/* Setting for tasks periods. 10 here equals 1 second in realtime */
#define WINDSPEED_PERIOD 50
#define WINDDIRECTION_PERIOD 30
#define MOVEPITCH_PERIOD 30
#define MOVEYAW_PERIOD 30
#define CONTROLLER_PERIOD 10

/* Settings and calibration for yaw stepper */
#define PERFECTYAWLOW 146   // Always a multiple of 2, because the sensor is rounded to multples of 2
#define PERFECTYAWHIGH 146  // Same as LOW
#define YAW_STEP_DEGREE 1.8 // Degree of 1 step on stepper
#define YAW_GEAR_RATIO 16   // x rotations of stepper == one rotation of turbine

/* Settings for measurement of wind direction */
#define ADC_TO_DEGREE 1.46           // What is one degree, sensor can only measure from 0 to 350 degree
#define WINDDIRECTION_MEASUREMENTS 5 // Number of measurements per average

/* Settings and calibration for PID controller         */
#define KP 3
#define KI 10
#define KD 0.005

#define MAX_SPEED 50
#define MAX_PITCH 45
#define MIN_PITCH 0

/* Settings for calibration for pitch stepper*/
#define PITCH_STEP_DEGREE 1.8
#define PITCH_GEAR_RATIO 4

static void setup(void);
void controller(void *parameter);
int calculateYawSteps(int position);
void moveYaw(int value);
void movePitch(pidC_t pid);
void setDirectionPitch(int direction);
void clearScreen();
void printData();

struct ControlData
{
    int windDirection, rotorSpeed, pitchAngle;
    double windSpeed;
} controlData;

#endif