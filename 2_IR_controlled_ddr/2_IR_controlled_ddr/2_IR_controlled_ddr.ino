#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"
#include "IRremote.hpp"

/**************************************************************************************
*  Wiring
*    _____________      ________________      __________________      _____________
*   |            G|<---|GND           5V|--->|ENA           OUT1|--->|             |
*   |  IR        R|<---|5V            11|--->|IN1               |    | RIGHT WHEEL |
*   | RECEIVER   Y|--->| 8   ARDUINO  10|--->|IN2   L298N   OUT2|--->|_____________|
*   |_____________|    |       UNO     9|--->|IN3               |     _____________
*                      |               6|--->|IN4           OUT3|--->|             |
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

//---------------- IR ------------------//
#define IR_RECEIVE_PIN 8

#define IR_STOP       (64u)
#define IR_FORWARD    (70u)
#define IR_BACKWARD   (21u)
#define IR_TURNLEFT   (68u)
#define IR_TURNRIGHT  (67u)
//////////////////////////////////////////

void setup() {
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  ddr.stop();
}

void loop() {
  if (IrReceiver.decode()) {
    uint8 u_command = IrReceiver.decodedIRData.command;

    switch (u_command)
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
    delay(100);
    IrReceiver.resume();
  }
}
