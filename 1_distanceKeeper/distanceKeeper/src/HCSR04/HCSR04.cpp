/******************************************************************************
*						HCSR04
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Commands used for HCSR04 sensor.
*
*  Wire Inputs: Echo -> ECHO
*
*  Wire Outputs: Trig -> TRIGGER
******************************************************************************/
#include "HCSR04.h"

HCSR04::HCSR04(uint8 const TRIGGER, uint8 const ECHO)
{
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO   , INPUT );

    digitalWrite(TRIGGER, LOW);

    trigger = TRIGGER;
    echo    = ECHO;
}

/**********************************************************
*  Function HCSR04::measureDistance()
*
*  Brief: Measure distaance in cm
*
*  Inputs: None
*
*  Outputs: [uint8] Distance in cm
*
*  Wire Inputs: None
**********************************************************/
uint8 HCSR04::measureDistance()
{
    uint16 u_timeFlight;
    uint8  u_distance;

    digitalWrite(trigger, HIGH);
    delayMicroseconds(DELAY_TRIGGER);
    digitalWrite(trigger, LOW);

    u_timeFlight = pulseIn(echo, HIGH);
    u_distance   = (uint8)(u_timeFlight / CM_FACTOR);

    return u_distance;
}