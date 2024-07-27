#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"

// DDR
uint8 const ins[] = {11, 10, 9, 6};

Wheel LEFTWHEEL  = {ins[0], ins[1]};
Wheel RIGHTWHEEL = {ins[2], ins[3]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  uint8 u_speed = (MAX_SPPED_CONTROL - MIN_SPPED_CONTROL) / 2;
  ddr.forward(u_speed);
  delay(100);
}

