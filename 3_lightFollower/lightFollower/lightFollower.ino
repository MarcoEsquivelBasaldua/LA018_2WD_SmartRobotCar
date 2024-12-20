#include "src/typeDefs/typeDefs.h"
#include "src/commonAlgo/commonAlgo.h"
#include "src/DDR_2/DDR_2.h"
#include "src/myServo/myServo.h"

//----------------- Defines ----------------//
#define SERVO_PIN       (5u)
#define MIN_ERROR_LIGHT (-50)
#define MAX_ERROR_LIGHT (50)
#define MIN_DEGS        (0u)
#define MAX_DEGS        (180u)

//----------- OPERATIONAL MODES ------------//
enum MODES{MODE_0,
           MODE_1,
           MODE_2,
           MODE_3,
           MODE_4};

volatile uint8 MODE_current;

//----------- Servo Heading ------------//
myServo headingServo(SERVO_PIN);

volatile uint8  prevHeading = 90u;
volatile sint16 heading;
const    float  lpfFactor = 0.25f;

//----------------- DDR ----------------//
uint8 const u_ins[] = {11u, 10u, 9u, 6u};

Wheel LEFTWHEEL  = {u_ins[0u], u_ins[1u]};
Wheel RIGHTWHEEL = {u_ins[2u], u_ins[3u]};

DDR2 ddr(LEFTWHEEL, RIGHTWHEEL);

/* LDR reading variables */
volatile uint8 leftLDRlevel;
volatile uint8 rightLDRlevel;
volatile sint8 lightError;

void setup()
{
  OP_MODE_0();
  headingServo.setHeading(90u);
  delay(500);
  Serial.begin(9600);
}

void loop()
{
  /* LDR readings */
  leftLDRlevel = u_mapLight2Percentage(analogRead(0));
  rightLDRlevel = u_mapLight2Percentage(analogRead(1));
  lightError = rightLDRlevel - leftLDRlevel;

  /* Set heading of the robot */
  heading = u_mapLigth2Degs(lightError);
  heading = (sint16)((1.0f - lpfFactor) * (float)heading + lpfFactor * (float)prevHeading);
  headingServo.setHeading((uint8)heading);
  prevHeading = heading;

  //OP_MODE_3();
  delay(10);

}

/**********************************************************
*  Function OP_MODE_0
*
*  Brief: Operational Mode 0. DDR is stopped.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_0()
{
  ddr.stop();
  MODE_current = MODE_0;
}

/**********************************************************
*  Function OP_MODE_0
*
*  Brief: Operational Mode 1. DDR moves forward in a straight line.
*         Wheels velocities are proportional to average value on both
*         light readings.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_1()
{
  /* Get average value from light readings*/
  uint8 u_ldrLevelMean = (leftLDRlevel + rightLDRlevel) >> 1;

  /* Interpolate light level to valid speed */
  uint8 u_controlSpeed = u_linearBoundedInterpolation(u_ldrLevelMean, 0u, 100, (uint8)MIN_SPPED_CONTROL, (uint8)MAX_SPPED_CONTROL);

  /* Set Motor speed to computed control */
  ddr.setVelocities((sint16)u_controlSpeed, (sint16)u_controlSpeed);

  MODE_current = MODE_1;
}

/**********************************************************
*  Function OP_MODE_0
*
*  Brief: Operational Mode 2. DDR moves backward in a straight line.
*         Wheels velocities are proportional to average value on both
*         light readings.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_2()
{
  /* Get average value from light readings*/
  uint8 u_ldrLevelMean = (leftLDRlevel + rightLDRlevel) >> 1;

  /* Interpolate light level to valid speed */
  uint8 u_controlSpeed = u_linearBoundedInterpolation(u_ldrLevelMean, 0u, 100, (uint8)MIN_SPPED_CONTROL, (uint8)MAX_SPPED_CONTROL);

  /* Set Motor speed to computed control */
  ddr.setVelocities(-((sint16)u_controlSpeed), -((sint16)u_controlSpeed));
  //Serial.println(-((sint16)u_controlSpeed));

  MODE_current = MODE_2;
}

/**********************************************************
*  Function OP_MODE_0
*
*  Brief: Operational Mode 3. DDR moves forward. Wheels
*         velocities are proportional to ligth readings,
*         on each side.
*
*  Inputs: None
*
*  Outputs: None
**********************************************************/
void OP_MODE_3()
{
  /* Map left reading to right wheel speed  and vice versa*/
  uint8 u_controlSpeedLeft  = u_linearBoundedInterpolation(leftLDRlevel, 0u, 100, (uint8)MIN_SPPED_CONTROL, (uint8)MAX_SPPED_CONTROL);
  uint8 u_controlSpeedRight = u_linearBoundedInterpolation(rightLDRlevel, 0u, 100, (uint8)MIN_SPPED_CONTROL, (uint8)MAX_SPPED_CONTROL);

  ddr.setVelocities(u_controlSpeedLeft, u_controlSpeedRight);

  MODE_current = MODE_3;
}

/**********************************************************
*  Function s_mapLight2Percentage
*
*  Brief: Maps the light read in the analog input from [0, 1024]
*         to [0, 100]
*
*        100 .|    .......
*             |   /
*             |  /
*             | /
*          0 .|/__________
*             .    .
*             0   1024
*
*  Inputs: [uint16] u_sensorInput : analog read on the light sensor
*
*  Outputs: [uint8] mapped light intensity
*
*  Wire Inputs: None
**********************************************************/
uint8 u_mapLight2Percentage(uint16 const u_sensorInput)
{
  /* Convert to float all variables */
  float f_input   = (float)u_sensorInput;
  float f_output;

  float f_slope = 0.098f;
  f_output = f_slope * (f_input);
  return (uint8)f_output;
}

/**********************************************************
*  Function u_mapLigth2Degs
*
*  Brief: Determines the heading degrees based on the error on
*         light sensors
*
*          MAX_DEGS .|              .......
*                    |             /
*                    |            /
*                    |           /
*          MIN_DEGS .|........../
*                    |________________________________________
*                               .  .
*                 MIN_ERROR_LIGHT  MAX_ERROR_LIGHT
*
*  Inputs: [uint8] s_error  : error between light sensors
*
*  Outputs: [uint8] heading degrees
*
*  Wire Inputs: None
**********************************************************/
uint8 u_mapLigth2Degs(sint8 const s_error)
{
  if(s_error <= MIN_ERROR_LIGHT)
  {
    return MIN_DEGS;
  }
  else if(s_error >= MAX_ERROR_LIGHT)
  {
    return MAX_DEGS;
  }
  else
  {
    /* Convert to float all variables */
    float f_error    = (float)s_error;
    float f_minError = (float)MIN_ERROR_LIGHT;
    float f_maxError = (float)MAX_ERROR_LIGHT;
    float f_minDegs  = (float)MIN_DEGS;
    float f_maxDegs  = (float)MAX_DEGS;
    float f_headDeg;

    float f_slope = (f_maxDegs - f_minDegs)/(f_maxError - f_minError);
    f_headDeg = f_slope * (f_error - f_minError) + f_minDegs;
    return (uint8)f_headDeg;
  }
}
/**********************************************************
*  Function u_linearBoundedInterpolation
*
*  Brief: Determines the output mapped from an input signal.
*
*       u_maxOutput .|              .......
*                    |             /
*                    |            /
*                    |           /
*       u_minOutput .|........../
*                    |________________________________________
*                               .  .
*                      u_minInput   u_maxInput
*
*  Inputs: [uint8] u_minInput  : input signal
*          [uint8] u_minDist   : min allowed input
*          [uint8] u_maxInput  : max allowed input
*          [uint8] u_minOutput : min allowed output
*          [uint8] u_maxOutput : max allowed output
*
*  Outputs: [uint8] mapped output
*
*  Wire Inputs: None
**********************************************************/
uint8 u_linearBoundedInterpolation(uint8 const u_input  , 
                                   uint8 const u_minInput, uint8 const u_maxInput, 
                                   uint8 const u_minOutput , uint8 const u_maxOutput)
{
  if(u_input <= u_minInput)
  {
    return u_minOutput;
  }
  else if(u_input >= u_maxInput)
  {
    return u_maxOutput;
  }
  else
  {
    /* Convert to float all variables */
    float f_input     = (float)u_input;
    float f_minInput  = (float)u_minInput;
    float f_maxInput  = (float)u_maxInput;
    float f_minOutput = (float)u_minOutput;
    float f_maxOutput = (float)u_maxOutput;
    float f_output;

    float f_slope = (f_maxOutput - f_minOutput)/(f_maxInput - f_minInput);
    f_output = f_slope * (f_input - f_minInput) + f_minOutput;
    return (uint8)f_output;
  }
}
