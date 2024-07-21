#include <SPI.h>
#include <SD.h>
#include "src/typeDefs/typeDefs.h"
#include "src/DDR/DDR.h"

// DDR
//uint8 const ins[] = {11, 10, 9, 6};
uint8 const ins[] = {8, 7, 9, 6};
uint8 const interrupts[] = {2, 3};

Wheel LEFTWHEEL = {ins[0], ins[1],
                   interrupts[1],
                   STOP_RPM};
Wheel RIGHTWHEEL = {ins[2], ins[3],
                    interrupts[0],
                    STOP_RPM};

DDR ddr(LEFTWHEEL, RIGHTWHEEL);

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;

const int chipSelect = 4;
const int W5500_SS = 10;

void setup() {
  //Initialize SD card
  pinMode(W5500_SS, OUTPUT);     // set the Ethernet SS pin as an output (necessary!)
  digitalWrite(W5500_SS, HIGH);  // but turn off the W5500 chip for now  
}

void loop() {
  uint16 totalSamples;
  uint16 index = 0;
  uint8  samplesPerControl = 20;
  uint8  deltaControlInput = MAX_SPPED_CONTROL - MIN_SPPED_CONTROL;
  uint8  del = 100;
  
  totalSamples = (uint16)samplesPerControl * (uint16)deltaControlInput;

  uint8 controlInputs[totalSamples];
  uint8 speedLeft[totalSamples];
  uint8 speedRight[totalSamples];

  for(uint8 speedControl = (uint8)MAX_SPPED_CONTROL; speedControl > (uint8)MIN_SPPED_CONTROL; speedControl--)
  {
    //delay(100);
    for(uint8 sample = 0; sample < samplesPerControl; sample++)
    {
      //Serial.println(speedControl);
      ddr.forward(speedControl);

      uint8 rightRPM = ddr.rightWheel.u_velRPM;
      uint8 leftRPM = ddr.leftWheel.u_velRPM;

      delay(del);

      controlInputs[index] = speedControl;
      speedLeft[index] = leftRPM;
      speedRight[index++] = rightRPM;
    }
  }

  // Open SSD and write 
  card.init(SPI_HALF_SPEED, chipSelect);
  volume.init(card);
  root.openRoot(volume);
  myFile = SD.open("velocities.txt", FILE_WRITE);

  if(myFile)
  {
    myFile.println("Control, left speed (rpm), rigth speed (rpm)");

    for(index = 0; index < totalSamples; index++)
    {
      myFile.print(controlInputs[index]);
      myFile.print(",");
      myFile.print(speedLeft[index]);
      myFile.print(",");
      myFile.println(speedRight[index]);
    }
  }
  
}

