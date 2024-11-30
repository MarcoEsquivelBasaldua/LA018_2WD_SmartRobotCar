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
#include "DDR_2.h"

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
*  Function DDR::setVelocities()
*
*  Brief: Velocities to each wheel
*
*  Inputs: [sint16] velLeft:  velocity control on the PWM cycle-duty range [-255, 255]
*                            to the left wheel
*  Inputs: [sint16] velRight: velocity control on the PWM cycle-duty range [-255, 255]
*                            to the right wheel
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
void DDR::setVelocities(sint16 const velLeft, sint16 const velRight)
{
	// Get vel offset between whels
	sint8 s_velLeftSign = s_getSign(velLeft);
	uint8 u_velOffset = getVelOffset(velLeft);

	if (velLeft > 0u)
	{
		analogWrite(leftWheel.u_in1, velLeft);
 		analogWrite(leftWheel.u_in2, STOP_RPM );
	}
	else
	{
		analogWrite(leftWheel.u_in1, STOP_RPM);
 		analogWrite(leftWheel.u_in2, velLeft);
	}

	if (velRight > 0u)
	{
		analogWrite(rightWheel.u_in1, vel + 2 * s_velLeftSign * u_velOffset);
 		analogWrite(rightWheel.u_in2, STOP_RPM);
	}
	else
	{
		analogWrite(rightWheel.u_in1, STOP_RPM);
 		analogWrite(rightWheel.u_in2, vel + 2 * s_velLeftSign * u_velOffset);
	}
  	
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
	analogWrite(leftWheel.u_in1 , STOP_RPM);
	analogWrite(leftWheel.u_in2 , STOP_RPM);
	analogWrite(rightWheel.u_in1, STOP_RPM);
	analogWrite(rightWheel.u_in2, STOP_RPM);
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

/**********************************************************
*  Function s_abs
*
*  Brief: Returns the absolute value. 
*         Datatypes are determined according to the ones used 
*         in the project.
*
*  Inputs: [sint16] s_value : value to get absolute value from
*
*  Outputs: [uint8] absolute value of s_value
*
*  Wire Inputs: None
**********************************************************/
uint8 s_abs(sint16 const s_value)
{
  if(s_value >= 0)
    return s_value;
  else
    return -s_value;
}

/**********************************************************
*  Function s_getSign
*
*  Brief: Returns the sign of a value.
*         Datatypes are determined according to the ones used 
*         in the project.
*
*  Inputs: [sint16] s_value : value to get sign from
*
*  Outputs: [uint8] sign of s_value
*
*  Wire Inputs: None
**********************************************************/
sint8 s_getSign(sint16 const s_value)
{
  if(s_value >= 0)
    return 1;
  else
    return -1;
}
