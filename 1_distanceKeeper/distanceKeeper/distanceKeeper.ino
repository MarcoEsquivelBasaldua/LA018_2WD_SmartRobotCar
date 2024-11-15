#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"
#include "src/HCSR04/HCSR04.h"

/**************************************************************************************
*  Wiring
*    ______________      ________________      __________________      _____________
*   |           VCC|<---|5V            5V|--->|ENA           OUT1|--->|             |
*   | HCSR04  TRIGG|<---|13            11|--->|IN1               |    | RIGHT WHEEL |
*   |          ECHO|--->|12   ARDUINO  10|--->|IN2   L298N   OUT2|--->|_____________|
*   |           GND|<---|GND    UNO     9|--->|IN3               |     _____________
*   |______________|    |               6|--->|IN4           OUT3|--->|             |
*                       |              5V|--->|ENB               |    | LEFT WHEEL  |
*                       |________________|    |              OUT4|--->|_____________|
*                                             |      JUMPER      |
*                       _________________     |       .-.        |
*                      |  BATTERY 7.4V  +|--->|VIN               |
*                      |                -|--->|GND               |
*                      |_________________|    |__________________|
*
***************************************************************************************/

//----------------- DDR ----------------//
uint8 const u_ins[] = {11u, 10u, 9u, 6u};

Wheel LEFTWHEEL  = {u_ins[0u], u_ins[1u]};
Wheel RIGHTWHEEL = {u_ins[2u], u_ins[3u]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);
//////////////////////////////////////////

//----------- Distance sensor ----------//
uint8 u_trigger = 13u;
uint8 u_echo    = 12u;
HCSR04 distSensor(u_trigger, u_echo);
//////////////////////////////////////////

/**********************************************************
*  setup()
*  Call sequence:
*                -> stop ddr
**********************************************************/
void setup() {
  ddr.stop();
}

/**********************************************************
*  loop()
*  Call sequence:
*                -> set distance threshold
*                -> set desired distance
*                -> get distance error as
*                   currentDistance - desiredDistance
*                -> get error absolute value 
*                -> if error abs val is greater than threshold
*                   -> get sign of the error
*                   -> map error to speed control with a bounded linear interpolation
*                   -> if sign error is positive, this means ddr is far from desired distance
*                      -> move forwards
*                   -> else, ddr is too close
*                      -> move backwards
*                -> else
*                   -> stop ddr
**********************************************************/
void loop() {
  uint8 u_distThreshold = 2u; // We want the car to stop within a distance range

  uint8 u_minVel = INDOOR_SPEED_CONTROL;   // Min allowed speed
  uint8 u_maxVel = OUTDOOR_SPEED_CONTROL;  // Max allowed spped
  
  uint8 u_keepDist    = 10u;                          // Desired distance
  uint8 u_currentDist = distSensor.measureDistance(); // Current distance
  sint8 s_error       = (sint8)u_currentDist - (sint8)u_keepDist;

  uint8 u_error = (uint8)s_abs(s_error); // Get absolute value of dist error

  if(u_error > u_distThreshold)
  {
    sint8 s_errorSign   = s_getSign(s_error);
    uint8 u_vel         = s_mapDist2Vel(       u_error      ,
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

/**********************************************************
*  Function s_mapDist2Vel
*
*  Brief: Determines the needed spped control based on error.
*         For this a bounded linear interpolation is done.

*          u_maxVel .|              .......
*                    |             /
*                    |            /
*                    |           /
*          u_minVel .|........../
*                    |________________________________________
*                               .  .
*                      u_minDist   u_maxDist
*
*  Inputs: [uint8] u_input   : distance input to be mapped
*          [uint8] u_minDist : minimum allowed distance
*          [uint8] u_maxDist : maximum allowed distance
*          [uint8] u_minVel  : minimum allowed speed control
*          [uint8] u_maxVel  : maximum allowed speed control
*
*  Outputs: [uint8] mapped control speed value
*
*  Wire Inputs: None
**********************************************************/
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

/**********************************************************
*  Function s_abs
*
*  Brief: Returns the absolute value. 
*         Datatypes are determined according to the ones used 
*         in the project.
*
*  Inputs: [sint8] s_value : value to get absolute value from
*
*  Outputs: [uint8] absolute value of s_value
*
*  Wire Inputs: None
**********************************************************/
sint8 s_abs(sint8 const s_value)
{
  if(s_value >= 0)
    return s_value;
  else
    return -s_value;
}

/**********************************************************
*  Function s_abs
*
*  Brief: Returns the absolute value. 
*         Datatypes are determined according to the ones used 
*         in the project.
*
*  Inputs: [sint8] s_value : value to get absolute value from
*
*  Outputs: [uint8] absolute value of s_value
*
*  Wire Inputs: None
**********************************************************/
sint8 s_getSign(sint8 const s_value)
{
  if(s_value >= 0)
    return 1;
  else
    return -1;
}

