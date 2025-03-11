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

#define ONE_DEG_DELAY   (5u)
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
  /* Robot going forward */
  ddr.forward(INDOOR_SPEED_CONTROL);

  /* Get current distance */
  u_distance = distSensor.measureDistance();

  if (u_distance < SAFETY_DISTANCE)
  {
    float f_meanDist2ObstaclesRight = 0.0f,
          f_meanDist2ObstaclesLeft = 0.0f;
    ddr.stop();

    /* Look to the right */
    for (uint8 u_heading = CENTER_DEGS, counter = 1u; 
         u_heading > MIN_SERVO_DEGREES; 
         u_heading--, counter++)
    {
      headingServo.setHeading(u_heading);
      delay(ONE_DEG_DELAY);
      f_meanDist2ObstaclesRight = ((float)(counter - 1u) * (f_meanDist2ObstaclesRight) + (float)distSensor.measureDistance()) / (float)counter;
    }

    /* Get heading back to middle */
    headingServo.setHeading(CENTER_DEGS);
    delay(500);

    /* Look to the left */
    for (uint8 u_heading = CENTER_DEGS, counter = 1u;
         u_heading < MAX_SERVO_DEGREES; 
         u_heading++, counter++)
    {
      headingServo.setHeading(u_heading);
      delay(ONE_DEG_DELAY);
      f_meanDist2ObstaclesLeft = ((float)(counter - 1u) * (f_meanDist2ObstaclesLeft) + (float)distSensor.measureDistance()) / (float)counter;
    }

    /* Get heading back to middle */
    headingServo.setHeading(CENTER_DEGS);
    delay(500);

    /* Change direction due to obstacle */
    if (f_meanDist2ObstaclesRight > f_meanDist2ObstaclesLeft)
    {
      ddr.turnRightFast(INDOOR_SPEED_CONTROL);
      delay(500);
    }
    else
    {
      ddr.turnLeftFast(INDOOR_SPEED_CONTROL);
      delay(500);
    }
  }

}