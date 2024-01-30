#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"

// DDR
uint8 const ins[] = {11, 10, 9, 6};
uint8 const interrupts[] = {2, 3};
Wheel LEFTWHEEL = {ins[0],
                   ins[1],
                   interrupts[1]};
Wheel RIGHTWHEEL = {ins[2],
                    ins[3],
                    interrupts[0]};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

void setup() {
  // put your setup code here, to run once:
  
  ddrInit(&ddr);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //float inSignal = analogRead(0);
  uint16 elapsedTimeLeft;
  uint16 elapsedTimeRight;
  
  elapsedTimeLeft = ddr.getElapsedTimeLeft();
  elapsedTimeRight = ddr.getElapsedTimeRight();

  Serial.println(elapsedTimeLeft);
  
  delay(100);
}

/*
void interruptLeftWheel()
{
  uint32 const currentTime = millis();
  elapsedTimeLeft = currentTime - prevTimeLeft;
  prevTimeLeft = currentTime;
}

void interruptRightWheel()
{
  uint32 const currentTime = millis();
  elapsedTimeRight = currentTime - prevTimeRight;
  prevTimeRight = currentTime;
}
*/
