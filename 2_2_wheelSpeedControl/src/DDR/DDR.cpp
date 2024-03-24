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

// Global variables for speedometer
volatile uint32 prevTimeLeft;
volatile uint32 prevTimeRight;

volatile float32 elapsedTimeLeft;
volatile float32 elapsedTimeRight;

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

	/* Set interrupts for speedometers */
	pinMode(LEFTWHEEL.u_speedInterrupt, INPUT_PULLUP);
	pinMode(RIGHTWHEEL.u_speedInterrupt, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(LEFTWHEEL.u_speedInterrupt), interruptLeftWheel, FALLING);
	attachInterrupt(digitalPinToInterrupt(RIGHTWHEEL.u_speedInterrupt), interruptRightWheel, FALLING);

	/* Attach wheels to DDR */
	leftWheel  = LEFTWHEEL;
	rightWheel = RIGHTWHEEL;

	/* Initialize PID variables */
	PIDinit();

	/* Initialize speedometer variables */
	prevTimeLeft = millis();
	prevTimeRight = millis();
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
	float32 time = velPIDcontrol(&leftWheel, elapsedTimeLeft, vel);
	//velPIDcontrol(&rightWheel, elapsedTimeRight, vel);

	//getRPM(&leftWheel, elapsedTimeLeft);

	// left wheel
 	analogWrite(leftWheel.u_in1, leftWheel.PID_vars.u_pidControl);
 	analogWrite(leftWheel.u_in2, STOP_RPM );

	// rigth Wheel
  	analogWrite(rightWheel.u_in1, rightWheel.PID_vars.u_pidControl);
 	analogWrite(rightWheel.u_in2, STOP_RPM);

	return time;
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
 	analogWrite(leftWheel.u_in1, vel     );
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
	// rigth Wheel
  	analogWrite(rightWheel.u_in1, vel     );
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
	// rigth Wheel
  	analogWrite(rightWheel.u_in1, STOP_RPM);
 	analogWrite(rightWheel.u_in2, vel     );

 	// left wheel
 	analogWrite(leftWheel.u_in1, vel     );
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
	// rigth Wheel
  	analogWrite(rightWheel.u_in1, vel     );
 	analogWrite(rightWheel.u_in2, STOP_RPM);

 	// left wheel
 	analogWrite(leftWheel.u_in1, STOP_RPM);
 	analogWrite(leftWheel.u_in2, vel     );
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
	// rigth Wheel
  	analogWrite(rightWheel.u_in1, STOP_RPM);
 	analogWrite(rightWheel.u_in2, vel     );

 	// left wheel
 	analogWrite(leftWheel.u_in1, STOP_RPM);
 	analogWrite(leftWheel.u_in2, vel     );
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

	leftWheel.u_velRPM = STOP_RPM;
	rightWheel.u_velRPM = STOP_RPM;
}

/**********************************************************
*  Function DDR::getRPM()
*
*  Brief: Updates wheel speed by applying a Low Pass Filter
*
*  Inputs: wheel -> Pointer to wheel we want to get speed from
*          elapsedTime -> Elapsed time measured from interrupts
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: None
**********************************************************/
void DDR::getRPM(Wheel * const wheel, float32 const elapsedTime)
{
	uint8 const prevRPM = wheel->u_velRPM;
	uint8 RPM;
	
	RPM = (uint8)(LPF_Factor * (float32)prevRPM + (ONE_F - LPF_Factor) * (WHEEL_RPM_FACTOR / elapsedTime));

	wheel->u_velRPM = RPM;
}

/**********************************************************
*  Function DDR::PIDinit()
*
*  Brief: Initialize the used variables for PID implementation
*
*  Inputs:  None
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: None
**********************************************************/
void DDR::PIDinit()
{
	leftWheel.PID_vars.f_cumError = 0.0f;
	leftWheel.PID_vars.u_prevTime = millis();
	leftWheel.PID_vars.s_prevError = 0;
	leftWheel.PID_vars.u_pidControl = 0u;

	rightWheel.PID_vars.f_cumError = 0.0f;
	rightWheel.PID_vars.u_prevTime = millis();
	rightWheel.PID_vars.s_prevError = 0;
	rightWheel.PID_vars.u_pidControl = 0u;
}

/**********************************************************
*  Function DDR::velPIDcontrol()
*
*  Brief: Computes needed control output for a desired wheel speed using PID approach
*
*  Inputs: wheel -> Pointer to wheel we want to control
*          elapsedTime -> Elapsed time measured from interrupts
*          vel -> Desired vel in RPM
*
*  Outputs: Speed control for wheel
*
*  Wire Inputs: None
*
*  Wire Outputs: None
**********************************************************/
void DDR::velPIDcontrol(Wheel * const wheel, float32 const speedometerTime, uint8 const desiredVel)
{
	getRPM(wheel, speedometerTime);

	PID_vars      pid_vars = wheel->PID_vars;
	PID_ks  const pid_ks = pid_vars.pid_ks;
	uint32  const currentTime = millis();
	uint32  const prevtime = pid_vars.u_prevTime;
	float32 const elapsedTime = (float32)(currentTime - prevtime) * MILLIS_TO_MINUTES;
	float32       cumError = pid_vars.f_cumError;
	float32       rateError;
	float32       pidControl;
	sint16  const prevError = pid_vars.s_prevError;
	sint16  const error = (sint16)desiredVel - wheel->u_velRPM;

	cumError += ((float32)error * elapsedTime);
	rateError = ((float32)(error - prevError) / elapsedTime);

	pidControl = (float32)pid_vars.u_pidControl + 
	             pid_ks.f_kp * (float32)error   + 
				 pid_ks.f_ki * cumError         + 
				 pid_ks.f_kd * rateError;

	/* Bound control output */
	pidControl = MIN(pidControl, MAX_SPPED_CONTROL);
	pidControl = MAX(pidControl, MIN_SPPED_CONTROL);

	/* Update pid vars */
	wheel->PID_vars.f_cumError = cumError;
	wheel->PID_vars.u_prevTime = currentTime;
	wheel->PID_vars.s_prevError = error;
	
	/* Control output */
	wheel->PID_vars.u_pidControl = (uint8)pidControl;
}

/**********************************************************
*  Function interruptLeftWheel()
*
*  Brief: Interrupt function for left Hall effect sensor.
*         Saves elapsed time in minutes between detections.
*
*  Inputs: None
*
*  Outputs: None
*
*  Wire Inputs: Left A3144e : D0 -> DIGITAL 3
*
*  Wire Outputs: None
**********************************************************/
void interruptLeftWheel()
{
  uint32 const currentTime = millis();
  elapsedTimeLeft = (float32)(currentTime - prevTimeLeft) * MILLIS_TO_MINUTES;
  prevTimeLeft = currentTime;
}

/**********************************************************
*  Function interruptRightWheel()
*
*  Brief: Interrupt function for right Hall effect sensor.
*         Saves elapsed time in minutes between detections.
*
*  Inputs: None
*
*  Outputs: None
*
*  Wire Inputs: Right A3144e : D0 -> DIGITAL 2
*
*  Wire Outputs: None
**********************************************************/
void interruptRightWheel()
{
  uint32 const currentTime = millis();
  elapsedTimeRight = (float32)(currentTime - prevTimeRight) * MILLIS_TO_MINUTES;
  prevTimeRight = currentTime;
}
