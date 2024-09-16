/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Wire Inputs: None
*
*  Wire Outputs: L298n Module -> IN1, IN2, IN3, IN4
******************************************************************************/
#ifndef DDR_h
#define DDR_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define  TOP_VEL_OFFSET       (1u)
#define  BOTTOM_VEL_OFFSET    (9u)
#define  STOP_RPM             (0u)                     /* RPM when stop */
#define  MIN_SPPED_CONTROL    (50u)                    /* Minimum allowed wheel output (determined experimentally) */
#define  MAX_SPPED_CONTROL    (255u - TOP_VEL_OFFSET)  /* Maximum allowed wheel output (full PWM) */
#define  ONE_F                (1.0f)                   /* Constant 1 float*/

#define  MAX(x,y)           ( ((x)>(y)) ? (x) : (y) ) /* Max function macro */
#define  MIN(x,y)           ( ((x)<(y)) ? (x) : (y) ) /* Min function macro */

/*************************************************/

typedef struct Wheel{
	uint8 u_in1;
	uint8 u_in2;
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
		

	private:
		Wheel leftWheel;
		Wheel rightWheel;
};

uint8 getVelOffset(uint8 vel);

#endif
