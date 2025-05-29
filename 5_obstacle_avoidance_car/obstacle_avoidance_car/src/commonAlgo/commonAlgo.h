/******************************************************************************
*						  commonAlgo
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Library with common algorithms for the other algorithms
******************************************************************************/
#ifndef COMMONALGO_h
#define COMMONALGO_h

#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define  MAX(x,y)          ( ((x)>(y)) ? (x) : (y) )  /* Max function macro */
#define  MIN(x,y)          ( ((x)<(y)) ? (x) : (y) )  /* Min function macro */

/*************************************************/

uint8 u_abs_16to8(sint16 const inVal);

float32 f_abs_floatTofloat(float32 const inVal);

#endif
