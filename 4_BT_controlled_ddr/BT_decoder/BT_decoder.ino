#include "src/typeDefs/typeDefs.h"
#include <LiquidCrystal_I2C.h>

/**************************************************************************************
*  Wiring
*    _____________      _________________     ___________________
*   |          VCC|<---|GND           5V |--->|VCC               |
*   |          GND|<---|5V            GND|--->|GND               |
*   |  HC-06    Tx|--->|Rx   ARDUINO   A4|--->|SDA  16x2 LCD_I2C |
*   |           Rx|<---|Tx     UNO     A5|--->|SCL               |
*   |_____________|    |_________________|    |__________________|
*
***************************************************************************************/

LiquidCrystal_I2C lcd(0x27,16,2);  //
uint8 c_command;

void setup() {
  // Bluetooth will communicate by Serial
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.print("Data received");

  if (Serial.available())
  {
    // Read data received
    c_command = Serial.read();
  }

  // Set cursor on second row
  lcd.setCursor(0, 1);
  // Show received data
  lcd.print(c_command);

  delay(100);
  lcd.clear();
}