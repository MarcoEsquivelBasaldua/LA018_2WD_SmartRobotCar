/******************************************************************************
*						  myServo
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Servo library 
******************************************************************************/
#include "myServo.h"

myServo::myServo(uint8 const PIN)
{
    pinMode(PIN, OUTPUT);
    pin = PIN;
}

void myServo::setHeading(uint8 const degrees)
{
    sint16 degreesCompensated = degrees - SERVO_ERROR;
    uint16 dutyCycle;
    
    degreesCompensated = MIN(degreesCompensated, (sint16)MAX_SERVO_DEGREES);
    degreesCompensated = MAX(degreesCompensated, (sint16)MIN_SERVO_DEGREES);
    
    dutyCycle = (uint16)((float)degreesCompensated * 10.25f) + 500u;

    digitalWrite(pin, HIGH);
    delayMicroseconds(dutyCycle);
    digitalWrite(pin, LOW);
    delay(10);
}