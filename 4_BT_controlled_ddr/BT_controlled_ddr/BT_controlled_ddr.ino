#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"
#include "src/IRDecoder/IRDecoder.h"

/**************************************************************************************
*  Wiring
*    _____________      ________________      __________________      _____________
*   |          VCC|<---|GND           5V|--->|ENA           OUT1|--->|             |
*   |          GND|<---|5V            11|--->|IN1               |    | RIGHT WHEEL |
*   |  HC-06    Tx|--->|Rx   ARDUINO  10|--->|IN2   L298N   OUT2|--->|_____________|
*   |           Rx|<---|Tx     UNO     9|--->|IN3               |     _____________
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
      case IR_STOP:
        ddr.stop();
        break;
      case IR_FORWARD:
        ddr.forward(OUTDOOR_SPEED_CONTROL);
        break;
      case IR_BACKWARD:
        ddr.backward(OUTDOOR_SPEED_CONTROL);
        break;
      case IR_TURNLEFT:
        ddr.turnLeft(OUTDOOR_SPEED_CONTROL);
        break;
      case IR_TURNRIGHT:
        ddr.turnRight(OUTDOOR_SPEED_CONTROL);
        break;
      default:
        ddr.stop();
        break;
    }
  }
}
