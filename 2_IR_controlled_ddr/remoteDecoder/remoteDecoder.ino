#include "src/IRDecoder/IRDecoder.h"

uint8 u_datPin = 2u;

IRDecoder IR(u_datPin);

void setup() {
  Serial.begin(115200); //Serial Interface for Debugging
  Serial.println("Decoder Starting!!");
}

void loop() {
  uint32 command;

  command = IR.getCommand();
  if(command)
  {
    Serial.println(command, HEX); //Print the value in serial monitor for debugging
  }
}