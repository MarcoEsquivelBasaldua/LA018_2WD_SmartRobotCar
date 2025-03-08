/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used to decode IR signal. Based on the code 
*         https://github.com/mbabeysekera/advanced-arduino-ir-remote
*
*  Inputs:  DAT -> PIN2
*
*  Outputs: None
******************************************************************************/
#ifndef IR_DECODER_h
#define IR_DECODER_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define DATA_LENGTH          (32u)
#define LOW_FLAG             (0u)
#define HIGH_FLAG            (1u)
#define INIT_COUNTER         (0u)
#define LOW_DATA             (0u)
#define HIGH_DATA            (1u)
#define LOW_DATA_MIN_LIMIT   (1000u)
#define LOW_DATA_MAX_LIMIT   (1300u)
#define HIGH_DATA_MAX_LIMIT  (2500u)

#define IR_STOP              (0xFF00FD02)
#define IR_FORWARD           (0xFF009D62)
#define IR_BACKWARD          (0xFF0057A8)
#define IR_TURNLEFT          (0xFF00DD22)
#define IR_TURNRIGHT         (0xFF003DC2)
/*************************************************/

class IRDecoder
{
    public:
        IRDecoder(uint8 const u_datPin);
        uint32 getCommand();
};

void bitReceived();

#endif
