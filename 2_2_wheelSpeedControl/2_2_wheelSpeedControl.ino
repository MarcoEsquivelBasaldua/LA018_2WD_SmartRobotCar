#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"

// DDR
uint8 const ins[] = {11, 10, 9, 6};
uint8 const interrupts[] = {2, 3};

Wheel LEFTWHEEL = {ins[0], ins[1],
                   interrupts[1],
                   STOP_RPM};
Wheel RIGHTWHEEL = {ins[2], ins[3],
                    interrupts[0],
                    STOP_RPM};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint8 speedControl;
  uint8 leftRPM;
  uint8 rightRPM;
  uint8 del = 200;

  for(speedControl = MIN_SPPED_CONTROL; speedControl <= MAX_SPPED_CONTROL; speedControl++)
  {
    ddr.forward(speedControl);

    rightRPM = ddr.rightWheel.u_velRPM;
    leftRPM = ddr.leftWheel.u_velRPM;
    Serial.print(leftRPM);
    Serial.print(",");
    Serial.println(rightRPM);

    delay(del);
  }

  for(speedControl = MAX_SPPED_CONTROL; speedControl >= MIN_SPPED_CONTROL; speedControl--)
  {
    ddr.forward(speedControl);

    rightRPM = ddr.rightWheel.u_velRPM;
    leftRPM = ddr.leftWheel.u_velRPM;
    Serial.print(leftRPM);
    Serial.print(",");
    Serial.println(rightRPM);

    delay(del);
  }
  
}

