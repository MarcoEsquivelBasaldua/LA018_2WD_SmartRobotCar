/******************************************************************************
*						  commonAlgo
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Library with common algorithms for the other algorithms
******************************************************************************/
#include "commonAlgo.h"

/**********************************************************
*  Function u_abs_16to8()
*
*  Brief: Get absolute value from sint16 value and cast it into uint8
*
*  Inputs: [sint16] inVal : input value
*
*  Outputs: [uint8] outVal : output value
**********************************************************/
uint8 u_abs_16to8(sint16 const inVal)
{
	uint8 outVal;

	if (inVal >= 0)
	{
		outVal = uint8(inVal);
	}
	else
	{
		outVal = uint8(-inVal);
	}

	return outVal;
}

/**********************************************************
*  Function f_abs_floatTofloat()
*
*  Brief: Get absolute value from float32 value and also return float32
*
*  Inputs: [float32] inVal : input value
*
*  Outputs: [float32] outVal : output value
**********************************************************/
float32 f_abs_floatTofloat(float32 const inVal)
{
	float32 outVal;
	
	if (inVal >= 0)
	{
		outVal = inVal;
	}
	else
	{
		outVal = -inVal;
	}

	return outVal;
}
