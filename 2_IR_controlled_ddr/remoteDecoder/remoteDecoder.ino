#include "src/IRDecoder/IRDecoder.h"

IRDecoder IR(2);

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