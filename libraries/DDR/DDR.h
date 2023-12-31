/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Inputs:  ENS -> ENA and ENB
*           INS -> IN1, IN2, IN3, IN4
*
*  Outputs: None
******************************************************************************/
#ifndef DDR_h
#define DDR_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

typedef struct Wheel{
	uint8 EN;
	uint8 IN1;
	uint8 IN2;
} Wheel; // End Wheel

class DDR
{
	public:
		DDR(Wheel const LEFTWHEEL, Wheel const RIGHTWHEEL);
		void forward(uint8 vel);
		void backward(uint8 vel);
		void turnRight(uint8 vel);
		void turnLeft(uint8 vel);
		void turnRightFast(uint8 vel);
		void turnLeftFast(uint8 vel);
		void stop();

	private:
		Wheel leftWheel;
		Wheel rightWheel;
};

#endif
