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
	// rigth Wheel
  	analogWrite(rightWheel.u_in1, vel     );
 	analogWrite(rightWheel.u_in2, STOP_RPM);

 	// left wheel
 	analogWrite(leftWheel.u_in1, vel      );
 	analogWrite(leftWheel.u_in2, STOP_RPM );

	getRPM();
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
*  Brief: Updates each wheel speed by applying a Low Pass Filter
*
*  Inputs: None
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: None
**********************************************************/
void DDR::getRPM()
{
	uint8 const prevLeftRPM = leftWheel.u_velRPM;
	uint8 const prevRightRPM = rightWheel.u_velRPM;
	uint8 leftRPM;
	uint8 rightRPM;
	
	leftRPM = (uint8)(LPF_Factor * (float32)prevLeftRPM + (ONE_F - LPF_Factor) * (WHEEL_RPM_FACTOR / elapsedTimeLeft));
	rightRPM = (uint8)(LPF_Factor * (float32)prevRightRPM + (ONE_F - LPF_Factor) * (WHEEL_RPM_FACTOR / elapsedTimeRight));

	leftWheel.u_velRPM = leftRPM;
	rightWheel.u_velRPM = rightRPM;
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
