/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Inputs:  INS -> IN1, IN2, IN3, IN4
*
*  Outputs: None
******************************************************************************/
#ifndef DDR_h
#define DDR_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define  MILLIS_TO_MINUTES  (1.666e-5)  /* Factor to convert miliseconds to minutes */
#define  WHEEL_RPM_FACTOR   (0.2f)      /* Factor of distribution of magnets in wheels */
#define  LPF_Factor         (0.25f)     /* Low Pass Filter factor to smooth RPM signals */
#define  STOP_RPM           (0u)        /* RPM when stop */
#define  MIN_RPM            (70u)       /* Minimum allowed RPM (determined experimentally) */
#define  MAX_RPM            (170u)      /* Maximum allowed RPM (determined experimentally) */
#define  MIN_SPPED_CONTROL  (40.0f)     /* Minimum allowed wheel output (determined experimentally) */
#define  MAX_SPPED_CONTROL  (255.0f)    /* Maximum allowed wheel output (full PWM) */
#define  ONE_F              (1.0f)      /* Constant 1 float*/

#define  MAX(x,y)           ( ((x)>(y)) ? (x) : (y) ) /* Max function macro */
#define  MIN(x,y)           ( ((x)<(y)) ? (x) : (y) ) /* Min function macro */

/*************************************************/

typedef struct Wheel{
	uint8 u_in1;
	uint8 u_in2;
	uint8 u_speedInterrupt;
	uint8 u_velRPM;
} Wheel; // End Wheel

class DDR
{
	public:
		DDR(Wheel const LEFTWHEEL, Wheel const RIGHTWHEEL);
		void forward(uint8 const vel);
		void backward(uint8 const vel);
		void turnRight(uint8 const vel);
		void turnLeft(uint8 const vel);
		void turnRightFast(uint8 const vel);
		void turnLeftFast(uint8 const vel);
		void stop();
		

	//private:
		void getRPM(Wheel * const wheel, float32 const elapsedTime);
		
		Wheel leftWheel;
		Wheel rightWheel;
};

void interruptLeftWheel();

void interruptRightWheel();

#endif
