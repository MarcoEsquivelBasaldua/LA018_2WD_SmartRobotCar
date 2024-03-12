#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"

// DDR
float32 const left_PID_K[] = {0.0, 0.0, 0.0};
float32 const right_PID_K[] = {0.0, 0.0, 0.0};
uint8 const ins[] = {11, 10, 9, 6};
uint8 const interrupts[] = {2, 3};

Wheel LEFTWHEEL = {left_PID_K[0], left_PID_K[1], left_PID_K[2],
                   ins[0], ins[1],
                   interrupts[1],
                   0.0};
Wheel RIGHTWHEEL = {right_PID_K[0], right_PID_K[1], right_PID_K[2],
                    ins[2], ins[3],
                    interrupts[0],
                    0.0};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

void setup() {
  // put your setup code here, to run once:
  
  ddrInit(&ddr);
  //Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //float inSignal = analogRead(0);
  ddr.forward(40);
  uint8 leftRPM = ddr.leftWheel.RPM;
  uint8 rightRPM = ddr.rightWheel.RPM;
  
  //elapsedTimeLeft = ddr.getRPMLeft();
  //elapsedTimeRight = ddr.getRPMRight();

  //Serial.println(leftRPM);
  
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
