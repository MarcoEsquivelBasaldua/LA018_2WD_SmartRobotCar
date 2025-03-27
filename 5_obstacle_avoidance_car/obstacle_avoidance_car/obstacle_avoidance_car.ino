#include "src/typedefs/typedefs.h"
#include "src/DDR/DDR.h"
#include "src/HCSR04/HCSR04.h"
#include "src/myServo/myServo.h"
#include "src/BT_encodedData/BT_encodedData.h"

//----------------- Defines ----------------//
#define SERVO_PIN       (5u)

#define MIN_DEGS        (0u)
#define CENTER_DEGS     (90u)
#define MAX_DEGS        (180u)
#define SAFETY_DISTANCE (15u)
#define TURNING_TIME    (250)
#define BACKWARD_TIME   (500)

#define ONE_DEG_DELAY   (5u)

#define STUCKED_BETWEEN_OBS_TH (5.0f)
//////////////////////////////////////////

//----------------- Enums ----------------//
enum lookDirection {FRONT, RIGHT, LEFT};
//////////////////////////////////////////

//----------------- DDR ----------------//
uint8 const u_ins[] = {11u, 10u, 9u, 6u};

Wheel LEFTWHEEL  = {u_ins[0u], u_ins[1u]};
Wheel RIGHTWHEEL = {u_ins[2u], u_ins[3u]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);
//////////////////////////////////////////

//----------- Distance sensor ----------//
const uint8 u_trigger = 13u;
const uint8 u_echo    = 12u;
HCSR04 distSensor(u_trigger, u_echo);

volatile uint8 u_distance;
//////////////////////////////////////////

//----------- Servo Heading ------------//
myServo headingServo(SERVO_PIN);

volatile uint8  u_prevHeading = 90u;
volatile sint16 u_heading;
const    float  lpfFactor = 0.25f;
//////////////////////////////////////////


void setup() {
  ddr.stop();
  headingServo.setHeading(CENTER_DEGS);
  delay(500);
}

void loop() {
  ObstacleAvoidance();
}

/**********************************************************
*  Function ObstacleAvoidance
*
*  Brief: Main function for obstacle avoidance functionality
*
*  Inputs: None
*
*  Outputs: None
*
*  Callsequence:
*
**********************************************************/
void ObstacleAvoidance()
{
  /* Robot going forward */
  ddr.forward(INDOOR_SPEED_CONTROL);

  /* Get current distance */
  u_distance = distSensor.measureDistance();

  if (u_distance < SAFETY_DISTANCE)
  {
    float f_meanDist2ObstaclesRight, f_meanDist2ObstaclesLeft;
    ddr.stop();

    /* Look to the right */
    f_meanDist2ObstaclesRight = getMeanFreeSpace(RIGHT);

    /* Get heading back to middle */
    headingServo.setHeading(CENTER_DEGS);
    delay(500);

    /* Look to the left */
    f_meanDist2ObstaclesLeft = getMeanFreeSpace(LEFT);

    /* Get heading back to middle */
    headingServo.setHeading(CENTER_DEGS);
    delay(500);

    /* Change direction due to obstacle */
    if (f_abs_floatTofloat(f_meanDist2ObstaclesRight - f_meanDist2ObstaclesLeft) <= STUCKED_BETWEEN_OBS_TH)
    {
      ddr.backward(INDOOR_SPEED_CONTROL);
      delay(BACKWARD_TIME);
      ddr.turnRightFast(INDOOR_SPEED_CONTROL);
      delay(TURNING_TIME);
    }
    else if (f_meanDist2ObstaclesRight > f_meanDist2ObstaclesLeft)
    {
      ddr.turnRightFast(INDOOR_SPEED_CONTROL);
      delay(TURNING_TIME);
    }
    else
    {
      ddr.turnLeftFast(INDOOR_SPEED_CONTROL);
      delay(TURNING_TIME);
    }
  }
}

/**********************************************************
*  Function getMeanFreeSpace
*
*  Brief: Get mean distances to obstacles in the given direction
*
*  Inputs: [lookDirection] direction : RIGHT or LEFT
*
*  Outputs: [float] : mean distance to obstacles
*
*  Callsequence:
*         start
*           : init mean value with 0;
*           : check which direction to check;
*           : repeat
*             : move heading angle;
*             : update mean value with measured distance;
*           : repeat (unit reaching side direction)
*           : return mean distances;
*         end
**********************************************************/
float getMeanFreeSpace(lookDirection direction)
{
  float f_meanDist2Obstacles = 0.0f;
  sint8 s_headingIncrement = (direction == LEFT) ? (1) : (-1); 

  for (uint8 u_heading = CENTER_DEGS, counter = 1u; 
       u_heading > MIN_SERVO_DEGREES && u_heading < MAX_SERVO_DEGREES; 
       u_heading += s_headingIncrement, counter++)
  {
    headingServo.setHeading(u_heading);
    delay(ONE_DEG_DELAY);
    f_meanDist2Obstacles = ((float)(counter - 1u) * (f_meanDist2Obstacles) + (float)distSensor.measureDistance()) / (float)counter;
  }

  return f_meanDist2Obstacles;
}