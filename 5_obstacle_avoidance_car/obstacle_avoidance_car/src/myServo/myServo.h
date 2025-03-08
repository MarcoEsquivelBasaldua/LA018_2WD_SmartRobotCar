/******************************************************************************
*						  myServo
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Servo library 
******************************************************************************/
#ifndef MYSERVO_h
#define MYSERVO_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"
#include "../commonAlgo/commonAlgo.h"

/******************* DEFINES *********************/
#define SERVO_ERROR        (12u)
#define MIN_SERVO_DEGREES  (0u)
#define MAX_SERVO_DEGREES  (180u)
/*************************************************/

class myServo
{
    public:
        myServo(uint8 const PIN);
        void setHeading(uint8 const degrees);

    private:
        uint8 pin;
};

#endif
