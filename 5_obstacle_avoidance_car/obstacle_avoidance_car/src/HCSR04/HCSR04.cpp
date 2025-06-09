#include "Arduino.h"
#include "HCSR04.h"

HCSR04::HCSR04(uint8 const TRIGGER, uint8 const ECHO)
{
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO   , INPUT );

    digitalWrite(TRIGGER, LOW);

    trigger = TRIGGER;
    echo    = ECHO;
}

uint16 HCSR04::measureDistance()
{
    uint16 u_distance, u_timeFlight;

    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    u_timeFlight = pulseIn(echo, HIGH);
    u_distance   = u_timeFlight / 59;

    return u_distance;
}
