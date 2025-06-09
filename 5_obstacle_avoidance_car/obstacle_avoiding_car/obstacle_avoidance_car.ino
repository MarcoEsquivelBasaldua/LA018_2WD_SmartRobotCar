#include "src/typedefs/typedefs.h"
#include "src/DDR/DDR.h"
#include "src/HCSR04/HCSR04.h"
#include "src/myServo/myServo.h"
#include "src/BT_encodedData/BT_encodedData.h"

/**************************************************************************************
*  Wiring
*                       ________________
*    _____________     |                |     __________________      _____________
*   |          VCC|<---|GND           5V|--->|ENA           OUT1|--->|             |
*   |          GND|<---|5V  ARDUINO   11|--->|IN1               |    | RIGHT WHEEL |
*   |  HC-06    Tx|--->|Rx    UNO     10|--->|IN2   L298N   OUT2|--->|_____________|
*   |           Rx|<---|Tx             9|--->|IN3               |     _____________
*   |_____________|    |               6|--->|IN4           OUT3|--->|             |
*                      |              5V|--->|ENB               |    | LEFT WHEEL  |
*   ______________     |                |    |              OUT4|--->|_____________|
*  |              |    |                |    |      JUMPER      |
*  | IR SENSOR    |    |                |    |       .-.        |     _________________
*  |   LEFT    OUT|--->|3               |    |               VIN|<---|+  BATTERY 7.4V  |
*  |           GND|<---|GND             |    |               GND|<---|-                |
*  |           VCC|<---|5V              |    |__________________|    |_________________|
*  |______________|    |                |     ______________
*   ______________     |              5V|--->|VCC           |
*  |              |    |              13|--->|TRIGG  HCSR04 |
*  | IR SENSOR    |    |              12|<---|ECHO          |
*  |  RIGHT    OUT|--->|2            GND|--->|GND           |
*  |           GND|<---|GND             |    |______________|
*  |           VCC|<---|5V              |
*  |______________|    |________________|
*
***************************************************************************************/

//----------------- Defines ----------------//
#define SERVO_PIN       (5u)

#define MIN_DEGS        (0u)
#define CENTER_DEGS     (90u)
#define MAX_DEGS        (180u)
#define SAFETY_DISTANCE (15u)
#define TURNING_TIME    (700)
#define BACKWARD_TIME   (1000)

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

volatile sint16 u_heading;
//////////////////////////////////////////

//--------- Operational Modes ----------//
enum op_Modes{STAND_BY, OBSTACLE_AVOIDANCE, BT_COMMANDED};
op_Modes curr_opMode;
//////////////////////////////////////////

char bt_command = BT_STOP;

void setup() {
  /* INnit operational Mode */
  curr_opMode = STAND_BY;

  /* Robot Motion init */
  ddr.stop();
  headingServo.setHeading(CENTER_DEGS);
  delay(500);

  /* BT init */
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) 
  {
    char c_command = Serial.read();
    
    /* Obstacle Ovoidance enabled */
    if (c_command == BT_A)
    {
      curr_opMode = OBSTACLE_AVOIDANCE;
    }
    else if (c_command == BT_B)
    {
      curr_opMode = BT_COMMANDED;
    }
    else if (c_command == BT_C)
    {
      curr_opMode = STAND_BY;
    }
    else if (c_command == BT_FORWARD)
    {
      bt_command = BT_FORWARD;
    }
    else
    {
      switch (c_command)
      {
        case BT_STOP:
          bt_command = BT_STOP;
          break;
        case BT_FORWARD:
          bt_command = BT_FORWARD;
          break;
        case BT_BACKWARD:
          bt_command = BT_BACKWARD;
          break;
        case BT_LEFT:
          bt_command = BT_LEFT;
          break;
        case BT_RIGHT:
          bt_command = BT_RIGHT;
          break;
        case BT_FORWARD_LEFT:
          bt_command = BT_FORWARD_LEFT;
          break;
        case BT_FORWARD_RIGHT:
          bt_command = BT_FORWARD_RIGHT;
          break;
        case BT_BACKWARD_RIGHT:
          bt_command = BT_BACKWARD_RIGHT;
          break;
        case BT_BACKWARD_LEFT:
          bt_command = BT_BACKWARD_LEFT;
          break;
        default:
          bt_command = BT_STOP;
          break;
      }
    }
  }


  if (curr_opMode == OBSTACLE_AVOIDANCE)
  {
    ObstacleAvoidance();
  }
  else if (curr_opMode == BT_COMMANDED)
  {
    blueToothCommand(bt_command);
  }
  else if (curr_opMode == STAND_BY)
  {
    ddr.stop();
  }
  else
  {
    /* Do nothing*/
  }
  
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

void blueToothCommand(char c_command)
{
  switch (c_command)
  {
    case BT_STOP:
      ddr.stop();
      break;
    case BT_FORWARD:
      ddr.forward(OUTDOOR_SPEED_CONTROL);
      break;
    case BT_BACKWARD:
      ddr.backward(OUTDOOR_SPEED_CONTROL);
      break;
    case BT_LEFT:
      ddr.turnLeft(OUTDOOR_SPEED_CONTROL);
      break;
    case BT_RIGHT:
      ddr.turnRight(OUTDOOR_SPEED_CONTROL);
      break;
    case BT_FORWARD_LEFT:
      ddr.setWheelsSpeed((uint16)(THREE_QUARTERS * OUTDOOR_SPEED_CONTROL), (uint16)OUTDOOR_SPEED_CONTROL);
      break;
    case BT_FORWARD_RIGHT:
      ddr.setWheelsSpeed((uint16)OUTDOOR_SPEED_CONTROL, (uint16)(THREE_QUARTERS * OUTDOOR_SPEED_CONTROL));
      break;
    case BT_BACKWARD_RIGHT:
      ddr.setWheelsSpeed(-(uint16)(OUTDOOR_SPEED_CONTROL), -(uint16)(THREE_QUARTERS * OUTDOOR_SPEED_CONTROL));
      break;
    case BT_BACKWARD_LEFT:
      ddr.setWheelsSpeed(-(uint16)(THREE_QUARTERS * OUTDOOR_SPEED_CONTROL), -(uint16)(OUTDOOR_SPEED_CONTROL));
      break;
    default:
      ddr.stop();
      break;
  }
}
