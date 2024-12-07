/******************************************************************************
*						DDR_2
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Wire Inputs: None
*
*  Wire Outputs: L298n Module -> IN1, IN2, IN3, IN4
******************************************************************************/
#ifndef DDR2_h
#define DDR2_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"
#include "../commonAlgo/commonAlgo.h"

/******************* DEFINES *********************/
#define  TOP_VEL_OFFSET         (  1u)
#define  BOTTOM_VEL_OFFSET      (  9u)
#define  STOP_RPM               (  0u)                   /* RPM when stop                                            */
#define  MIN_SPPED_CONTROL      ( 50u)                   /* Minimum allowed wheel output (determined experimentally) */
#define  INDOOR_SPEED_CONTROL   ( 60u)                   /* Desired control for indoor usage                         */
#define  OUTDOOR_SPEED_CONTROL  (100u)                   /* Desired control for outdoor usage                        */
#define  MAX_SPPED_CONTROL      (255u - TOP_VEL_OFFSET)  /* Maximum allowed wheel output (full PWM)                  */
#define  ONE_F                  (1.0f)                   /* Constant 1 float
/*************************************************/

typedef struct Wheel{
	uint8 u_in1;
	uint8 u_in2;
} Wheel; // End Wheel

class DDR2
{
	public:
		DDR2(Wheel const LEFTWHEEL, Wheel const RIGHTWHEEL);
		void setVelocities(sint16 const velLeft, sint16 const velRight);
		void stop();
		

	private:
		Wheel leftWheel;
		Wheel rightWheel;
};

uint8 velOffset(uint8 vel);
uint8 u_abs(sint16 const s_value);
sint8 s_getSign(sint16 const s_value);

#endif
