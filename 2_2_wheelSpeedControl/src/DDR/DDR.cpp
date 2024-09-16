/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Wire Inputs: Left A3144e Hall effect sensor module : D0 -> DIGITAL 3
*               Right A3144e Hall effect sensor module : D0 -> DIGITAL 2
*
*  Wire Outputs: L298n Module -> IN1, IN2, IN3, IN4
******************************************************************************/
#include "DDR.h"

DDR::DDR(Wheel const LEFTWHEEL, Wheel const RIGHTWHEEL)
{
	/* Set Left Wheel outputs */
	pinMode(LEFTWHEEL.u_in1, OUTPUT);
	pinMode(LEFTWHEEL.u_in2, OUTPUT);

	/* Set Rigth Wheel outputs */
	pinMode(RIGHTWHEEL.u_in1, OUTPUT);
	pinMode(RIGHTWHEEL.u_in2, OUTPUT);

	/* Stop DDR */
	analogWrite(LEFTWHEEL.u_in1 , STOP_RPM);
	analogWrite(LEFTWHEEL.u_in2 , STOP_RPM);
	analogWrite(RIGHTWHEEL.u_in1, STOP_RPM);
	analogWrite(RIGHTWHEEL.u_in2, STOP_RPM);

	/* Attach wheels to DDR */
	leftWheel  = LEFTWHEEL;
	rightWheel = RIGHTWHEEL;
}

/**********************************************************
*  Function DDR::forward()
*
*  Brief: DDR wheels are set to move forward
*
*  Inputs:  vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel  IN1 to vel
*                left wheel IN2 to 0
**********************************************************/
void DDR::forward(uint8 const vel)
{
	// Get vel offset between whels
	uint8 u_velOffset = getVelOffset(vel);

	// left wheel
 	analogWrite(leftWheel.u_in1, vel);
 	analogWrite(leftWheel.u_in2, STOP_RPM );

	// rigth Wheel
  	analogWrite(rightWheel.u_in1, vel + u_velOffset);
 	analogWrite(rightWheel.u_in2, STOP_RPM);
}

/**********************************************************
*  Function DDR::turnRight()
*
*  Brief: DDR wheels are set to turn right
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to 0
*                left wheel IN1 to vel
*                left wheel IN2 to 0
**********************************************************/
void DDR::turnRight(uint8 const vel)
{
	// rigth Wheel
  	analogWrite(rightWheel.u_in1, STOP_RPM);
 	analogWrite(rightWheel.u_in2, STOP_RPM);

 	// left wheel
 	analogWrite(leftWheel.u_in1, vel);
 	analogWrite(leftWheel.u_in2, STOP_RPM);
}

/**********************************************************
*  Function DDR::turnLeft()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel IN1 to 0
*                left wheel IN2 to 0
**********************************************************/
void DDR::turnLeft(uint8 const vel)
{
	// Get vel offset between whels
	uint8 u_velOffset = getVelOffset(vel);

	// rigth Wheel
  	analogWrite(rightWheel.u_in1, vel + u_velOffset);
 	analogWrite(rightWheel.u_in2, STOP_RPM);

 	// left wheel
 	analogWrite(leftWheel.u_in1, STOP_RPM);
 	analogWrite(leftWheel.u_in2, STOP_RPM);
}

/**********************************************************
*  Function DDR::turnRightFast()
*
*  Brief: DDR wheels are set to turn right fast
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to vel
*                left wheel IN1 to vel
*                left wheel IN2 to 0
**********************************************************/
void DDR::turnRightFast(uint8 const vel)
{
	// Get vel offset between whels
	uint8 u_velOffset = getVelOffset(vel);

	// rigth Wheel
  	analogWrite(rightWheel.u_in1, STOP_RPM);
 	analogWrite(rightWheel.u_in2, vel + u_velOffset);

 	// left wheel
 	analogWrite(leftWheel.u_in1, vel);
 	analogWrite(leftWheel.u_in2, STOP_RPM);
}

/**********************************************************
*  Function DDR::turnLeftFast()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to vel
*                right wheel IN2 to 0
*                left wheel IN1 to 0
*                left wheel IN2 to 0
**********************************************************/
void DDR::turnLeftFast(uint8 const vel)
{
	// Get vel offset between whels
	uint8 u_velOffset = getVelOffset(vel);

	// rigth Wheel
  	analogWrite(rightWheel.u_in1, vel + u_velOffset);
 	analogWrite(rightWheel.u_in2, STOP_RPM);

 	// left wheel
 	analogWrite(leftWheel.u_in1, STOP_RPM);
 	analogWrite(leftWheel.u_in2, vel);
}

/**********************************************************
*  Function DDR::backward()
*
*  Brief: DDR wheels are set to move backward
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to vel
*                left wheel IN1 to 0
*                left wheel IN2 to vel
**********************************************************/
void DDR::backward(uint8 const vel)
{
	// Get vel offset between whels
	uint8 u_velOffset = getVelOffset(vel);

	// rigth Wheel
  	analogWrite(rightWheel.u_in1, STOP_RPM);
 	analogWrite(rightWheel.u_in2, vel + u_velOffset);

 	// left wheel
 	analogWrite(leftWheel.u_in1, STOP_RPM);
 	analogWrite(leftWheel.u_in2, vel);
}

/**********************************************************
*  Function DDR::stop()
*
*  Brief: DDR wheels are stopped
*
*  Inputs: None
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: right wheel IN1 to 0
*                right wheel IN2 to 0
*                left wheel IN1 to 0
*                left wheel IN2 to 0
**********************************************************/
void DDR::stop()
{
	analogWrite(rightWheel.u_in1, STOP_RPM);
	analogWrite(rightWheel.u_in2, STOP_RPM);
	analogWrite(leftWheel.u_in1 , STOP_RPM);
	analogWrite(leftWheel.u_in2 , STOP_RPM);
}

/**********************************************************
*  Function getVelOffset()
*
*  Brief: On the current robot, left wheel spins faster than
*         the right wheel when same control is set. This function
*         helps finding the right control offset so wheels speed
*         are closer one to the other. Values here used were found
*         experimentally.
*
*  Inputs: [uint8] u_vel: control speed to the wheels.
*
*  Outputs: [uint8] control offset for the right wheel.
*
*  Wire Inputs: None
*
*  Wire Outputs: None
**********************************************************/
uint8 getVelOffset(uint8 u_vel)
{
	uint8 u_steps = MAX(TOP_VEL_OFFSET, BOTTOM_VEL_OFFSET) - MIN(TOP_VEL_OFFSET, BOTTOM_VEL_OFFSET) + 1u;
	uint8 u_deltaVel = (MAX_SPPED_CONTROL - MIN_SPPED_CONTROL) / u_steps;
	uint8 u_offset = 0u;
	sint8 s_sign = (TOP_VEL_OFFSET > BOTTOM_VEL_OFFSET) ? (1) : (-1);

	for (uint8 i=0; i<u_steps; i++)
	{
		if(u_vel < (i+1u)*u_deltaVel)
		{
			u_offset = (uint8)(BOTTOM_VEL_OFFSET + (sint8)i*s_sign);
			break;
		}
		u_offset = TOP_VEL_OFFSET;
	}

	return u_offset;
}
