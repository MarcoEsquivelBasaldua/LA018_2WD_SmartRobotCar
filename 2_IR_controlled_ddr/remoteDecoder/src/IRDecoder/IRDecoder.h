/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used to decode IR signal. Based on the code 
*         https://github.com/mbabeysekera/advanced-arduino-ir-remote
*
*  Inputs:  DAT -> PIN8
*
*  Outputs: None
******************************************************************************/
#ifndef IR_DECODER_h
#define IR_DECODER_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
/*************************************************/




class IRDecoder
{
    public:
        IRDecoder(uint8 const u_datPin);
        uint32 getCommand();

    private:

        void timerOneConfigForCapture();
};

//ISR(TIMER1_CAPT_vect);


#endif
