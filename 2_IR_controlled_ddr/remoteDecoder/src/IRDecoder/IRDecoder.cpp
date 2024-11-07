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
#include "IRDecoder.h"

/****************** VARIABLES ********************/
volatile uint8  inputCaptureData[32u]; //To store received time periods
volatile uint8  isFirstTriggerOccured; //First Trigger Flag
volatile uint8  receiveCounter;        //Receiver Counter
volatile uint8  receiveComplete;       //Receive Complete Flag
volatile uint32 prevMicros;
volatile uint32 currentMicros;
/*************************************************/

IRDecoder::IRDecoder(uint8 const u_datPin)
{
  // Enable Interruptions
  attachInterrupt(digitalPinToInterrupt(u_datPin), bitReceived, FALLING);

  // Initialize global variables
  isFirstTriggerOccured = 1u;
  receiveCounter        = 0u;
  receiveComplete       = 0u;
  prevMicros            = micros();

}

uint32 IRDecoder::getCommand()
{
  if (receiveComplete)
  {  //If receive complete, start decoding process
      uint32_t receiveStream = 0u; //To store decoded value
      for (int i = 0; i < 32; i++)
      {  //Decode all 32 bits receive as time periods
          if (inputCaptureData[i] == 0u && i != 31)
          {  //if the time period t* -> 1.0ms < t < 1.3ms
              receiveStream = (receiveStream << 1); //Only bit shift the current value
          }
          else if (inputCaptureData[i] == 1u)
          {  //if the time period t* -> 2.0ms < t < 2.5ms
              receiveStream |= 0x0001;  //increment value by 1 using Logic OR operation
              if (i != 31)
              {  //Only shift the bit unless it is the last bit of the captured stream
                  receiveStream = (receiveStream << 1); //Only bit shift the current value
              }
              receiveComplete = 0;  //Set the receive complete to 0 for next data to be captured
              receiveCounter  = 0u;
              isFirstTriggerOccured = 1u;
          }
      }
      return receiveStream; //Return the received data stream
  }
  return 0; //default return value is 0
}

void bitReceived()
{
  uint32 elapsedTime;

  if (isFirstTriggerOccured)
  {  //Capturing will start after first falling edge detected by ICP1 Pin
    currentMicros = micros();
    elapsedTime = currentMicros - prevMicros;
    if(elapsedTime > 1000u && elapsedTime < 1300u)
      {
        inputCaptureData[receiveCounter] = 0u;
      }
      else
      {
        inputCaptureData[receiveCounter] = 1u;
      }

    //inputCaptureData[receiveCounter] = ICR1;  //Read the INPUT CAPTURE REGISTER VALUE
    if (elapsedTime >  2500) {  // if the value is greater than 625 (~2.5ms), then
      receiveCounter = 0; //reset "receiveCounter"
      receiveComplete = 0;  //reset "receiveComplete"
    } else {
      receiveCounter++;
      if (receiveCounter == 32) { //if all the bits are detected,
        receiveComplete = 1;  //then set the "receiveComplete" flag to "1"
      }
    }
  } else {
    isFirstTriggerOccured = 1;  //First falling edge occured! Start capturing from the second falling edge.
  }
  //TCNT1 = 0;  //Reset Timer 01 counter after every capture
  prevMicros = currentMicros;




/*
  if(isFirstTriggerOccured)
  {
    receiveCounter        = 0u;
    receiveComplete       = 0u;
    isFirstTriggerOccured = 0u;
  }
  else
  {
    if (receiveCounter > 0u && receiveCounter <= 32u)
    {
      currentMicros = micros();
      elapsedTime = currentMicros - prevMicros;

      if(elapsedTime > 1000u && elapsedTime < 1300u)
      {
        inputCaptureData[receiveCounter - 1u] = 0u;
      }
      else
      {
        inputCaptureData[receiveCounter - 1u] = 1u;
      }

      if(receiveCounter == 32u)
      {
        receiveComplete = 1u;
      }

      prevMicros = currentMicros;
    }
    else
    {
      prevMicros = micros();
    }
    
    receiveCounter++;    
  }
  */
}
