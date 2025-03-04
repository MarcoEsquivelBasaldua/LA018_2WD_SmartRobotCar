#include "src/typeDefs/typeDefs.h"
#include "src/BT_encodedData/BT_encodedData.h"
#include "src/DDR/DDR.h"

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
*                      |________________|    |              OUT4|--->|_____________|
*                                            |      JUMPER      |
*                      _________________     |       .-.        |
*                     |  BATTERY 7.4V  +|--->|VIN               |
*                     |                -|--->|GND               |
*                     |_________________|    |__________________|
*
***************************************************************************************/

//----------------- DDR ----------------//
uint8 const u_ins[] = {11u, 10u, 9u, 6u};

Wheel LEFTWHEEL  = {u_ins[0u], u_ins[1u]};
Wheel RIGHTWHEEL = {u_ins[2u], u_ins[3u]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);
//////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  ddr.stop();
}

void loop() {

  if (Serial.available()) {
    char c_command = Serial.read();

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
}
