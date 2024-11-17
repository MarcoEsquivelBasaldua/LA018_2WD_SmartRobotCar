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
#ifndef HCSR04_H
#define HCSR04_H

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define  DELAY_TRIGGER  (10u)  /* Delay in ms to allow trigger to be set    */
#define  CM_FACTOR      (59u)  /* Factor to get distance in cm              */
#define  MIN_DIST       ( 2u)  /* Minimum measured distance in cm           */
#define  MIN_SAFE_DIST  ( 5u)  /* Minium distance keeping a safe threshold  */
#define  MAX_DIST       (80u)  /* Maximum measured distance in cm           */
#define  MAX_SAFE_DIST  (70u)  /* Maximum distance keeping a safe threshold */
/*************************************************/

class HCSR04
{
	public:
		HCSR04(uint8 const TRIGGER, uint8 const ECHO);
		uint8 measureDistance();

	private:
		uint8 trigger;
		uint8 echo;

};

#endif
