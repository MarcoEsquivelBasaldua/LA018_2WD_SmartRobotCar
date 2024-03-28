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
}

void loop() {
  uint16 totalSamples;
  uint16 index = 0;
  uint8  samplesPerControl = 10;
  uint8  deltaControlInput = MAX_SPPED_CONTROL - MIN_SPPED_CONTROL;
  uint8  del = 100;
  
  totalSamples = (uint16)samplesPerControl * (uint16)deltaControlInput;

  uint8 controlInputs[totalSamples];
  uint8 speedLeft[totalSamples];
  uint8 speedRight[totalSamples];

  for(uint8 speedControl = MIN_SPPED_CONTROL; speedControl < MAX_SPPED_CONTROL; speedControl++)
  {
    for(uint8 sample = 0; sample < totalSamples; sample++)
    {
      ddr.forward(speedControl);

      uint8 rightRPM = ddr.rightWheel.u_velRPM;
      uint8 leftRPM = ddr.leftWheel.u_velRPM;
      
      controlInputs[index] = speedControl;
      speedLeft[index] = leftRPM;
      speedRight[index++] = rightRPM;

      delay(del);
    }
  }
  
}

