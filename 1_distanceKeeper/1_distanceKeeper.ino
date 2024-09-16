#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"
#include "src/HCSR04/HCSR04.h"

// DDR
uint8 const u_ins[] = {11u, 10u, 9u, 6u};

Wheel LEFTWHEEL  = {u_ins[0], u_ins[1]};
Wheel RIGHTWHEEL = {u_ins[2], u_ins[3]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

// Distance sensor
uint8 u_trigger = 13u;
uint8 u_echo    = 12u;
HCSR04 distSensor(u_trigger, u_echo);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  uint16 distance = distSensor.measureDistance();

  Serial.print(distance);
  Serial.print("cm");
  Serial.println();

  ddr.stop();
  delay(100);
}

