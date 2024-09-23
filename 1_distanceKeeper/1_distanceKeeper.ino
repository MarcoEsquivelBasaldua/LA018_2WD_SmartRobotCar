#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"
#include "src/HCSR04/HCSR04.h"

// DDR
uint8 const u_ins[] = {11u, 10u, 9u, 6u};

Wheel LEFTWHEEL  = {u_ins[0u], u_ins[1u]};
Wheel RIGHTWHEEL = {u_ins[2u], u_ins[3u]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

// Distance sensor
uint8 u_trigger = 13u;
uint8 u_echo    = 12u;
HCSR04 distSensor(u_trigger, u_echo);

void setup() {
  ddr.stop();
}

void loop() {
  uint8 u_distThreshold = 2u;

  uint8 u_minVel = INDOOR_SPEED_CONTROL;
  uint8 u_maxVel = OUTDOOR_SPEED_CONTROL;
  
  uint8 u_keepDist    = 10u;
  uint8 u_currentDist = distSensor.measureDistance();
  sint8 s_error       = (sint8)u_currentDist - (sint8)u_keepDist;

  uint8 u_error = (uint8)s_abs(s_error); // Get absolute value of dist error

  if(u_error > u_distThreshold)
  {
    sint8 s_errorSign   = s_getSign(s_error);
    uint8 u_vel = s_mapDist2Vel(       u_error      ,
                              (uint8)MIN_SAFE_DIST, (uint8)MAX_SAFE_DIST,
                                     u_minVel     ,        u_maxVel      );

    if(s_errorSign > 0) // Move forward
    {
      ddr.forward(u_vel);
    }
    else               // Move backward
    {
      ddr.backward(u_vel);
    }
  }
  else
  {
    ddr.stop();
  }

  delay(100);
}

/* Linear interpolation */
uint8 s_mapDist2Vel(uint8 const u_input  , 
                    uint8 const u_minDist, uint8 const u_maxDist, 
                    uint8 const u_minVel , uint8 const u_maxVel)
{
  if(u_input <= u_minDist)
  {
    return u_minVel;
  }
  else if(u_input >= u_maxDist)
  {
    return u_maxVel;
  }
  else
  {
    /* Convert to float all variables */
    float f_input   = (float)u_input;
    float f_minDist = (float)u_minDist;
    float f_maxDist = (float)u_maxDist;
    float f_minVel  = (float)u_minVel;
    float f_maxVel  = (float)u_maxVel;
    float f_vel;

    float f_slope = (f_maxVel - f_minVel)/(f_maxVel - f_minVel);
    f_vel = f_slope * (f_input - f_minDist) + f_minVel;
    return (uint8)f_vel;
  }
}

/* Absolute value */
sint8 s_abs(sint8 const s_value)
{
  if(s_value >= 0)
    return s_value;
  else
    return -s_value;
}

/* Sign of a value */
sint8 s_getSign(sint8 const s_value)
{
  if(s_value >= 0)
    return 1;
  else
    return -1;
}

