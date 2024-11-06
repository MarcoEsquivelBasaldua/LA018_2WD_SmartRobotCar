#include "src/IRDecoder/IRDecoder.h"

IRDecoder IR(8);

void setup() {
  Serial.begin(115200); //Serial Interface for Debugging
  Serial.println("Decoder Starting!!");
}

void loop() {
  IR.getCommand();
}