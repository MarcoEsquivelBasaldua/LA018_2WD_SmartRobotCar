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
#include "IRDecoder.h"

/****************** VARIABLES ********************/
volatile uint8  inputCaptureData[DATA_LENGTH]; // To store received time periods
volatile uint8  isFirstTriggerOccured;         // First Trigger Flag
volatile uint8  receiveCounter;                // Receiver Counter
volatile uint8  receiveComplete;               // Receive Complete Flag
volatile uint32 prevMicros;                    // Period trackers in microseconds
volatile uint32 currentMicros;
/*************************************************/

IRDecoder::IRDecoder(uint8 const u_datPin)
{
  // Enable Interruptions
  attachInterrupt(digitalPinToInterrupt(u_datPin), bitReceived, FALLING);

  // Initialize global variables
  receiveCounter        = INIT_COUNTER;
  isFirstTriggerOccured = LOW_FLAG;
  receiveComplete       = LOW_FLAG;
  prevMicros            = micros();

}

/**********************************************************
*  Function IRDecoder::getCommand()
*
*  Brief: Decodes IR recieved data
*
*  Inputs:  None
*
*  Outputs: [uint32] decoded data recibed stored in unsigned int 32 variable 
*
*  Wire Inputs: IR_DATA from IR reciever to u_datPin
*
*  Wire Outputs: None
**********************************************************/
uint32 IRDecoder::getCommand()
{
  if (receiveComplete)
  {
    uint32 receiveStream = 0u; //To store decoded value

    // Decode recieved data
    for (uint8 i = INIT_COUNTER; i < DATA_LENGTH; i++)
    {
        if (inputCaptureData[i] == LOW_DATA && i != (DATA_LENGTH - 1u))
        {
          receiveStream = (receiveStream << 1); //Only bit shift the current value
        }
        else if (inputCaptureData[i] == HIGH_DATA)
        {
          receiveStream |= 0x0001;  //increment value by 1 using Logic OR operation
          //Only shift the bit unless it is the last bit of the captured stream
          if (i != (DATA_LENGTH - 1u))
          {
            receiveStream = (receiveStream << 1);
          }

          // Reset global variables
          receiveComplete       = LOW_FLAG;
          receiveCounter        = INIT_COUNTER;
          isFirstTriggerOccured = HIGH_FLAG;
        }
    }
    return receiveStream; //Return the received data stream
  }
  return 0u; //default return value is 0
}

/**********************************************************
*  Function bitReceived()
*
*  Brief: Interrupt function for IR received data handling
*
*  Inputs:  None
*
*  Outputs: None
*
*  Wire Inputs: IR_DATA from IR reciever to u_datPin
*
*  Wire Outputs: None
**********************************************************/
void bitReceived()
{
  uint32 elapsedTime;

  if (isFirstTriggerOccured)
  {
    currentMicros = micros();
    elapsedTime = currentMicros - prevMicros;

    // if value is between 1000 and 1300 (~1.3ms)
    if(elapsedTime > LOW_DATA_MIN_LIMIT && elapsedTime < LOW_DATA_MAX_LIMIT)
    {
      inputCaptureData[receiveCounter] = HIGH_DATA;
    }
    else
    {
      inputCaptureData[receiveCounter] = LOW_DATA;
    }

    // if the value is greater than 2500 (~2.5ms), then
    if (elapsedTime > HIGH_DATA_MAX_LIMIT) 
    {  
      receiveCounter  = INIT_COUNTER; //reset receiveCounter
      receiveComplete = LOW_FLAG;     //reset receiveComplete
    } 
    else 
    {
      receiveCounter++;
      // All bits detected
      if (receiveCounter == DATA_LENGTH) 
      {
        receiveComplete = HIGH_FLAG;
      }
    }
  } 
  else
  {
    isFirstTriggerOccured = HIGH_FLAG;  //First falling edge occured! Start capturing from the second falling edge.
  }
  
  prevMicros = currentMicros;
}
