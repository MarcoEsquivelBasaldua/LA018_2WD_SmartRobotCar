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

volatile float elapsedTimeLeft;
volatile float elapsedTimeRight;

DDR::DDR(Wheel const LEFTWHEEL, Wheel const RIGHTWHEEL)
{
	/* Left Wheel */
	pinMode(LEFTWHEEL.IN1, OUTPUT);
	pinMode(LEFTWHEEL.IN2, OUTPUT);

	/* Rigth Wheel */
	pinMode(RIGHTWHEEL.IN1, OUTPUT);
	pinMode(RIGHTWHEEL.IN2, OUTPUT);

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
	// rigth Wheel
  	analogWrite(rightWheel.IN1, vel);
 	analogWrite(rightWheel.IN2, 0 );

 	// left wheel
 	analogWrite(leftWheel.IN1, vel);
 	analogWrite(leftWheel.IN2, 0 );

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
  	analogWrite(rightWheel.IN1, 0);
 	analogWrite(rightWheel.IN2, 0);

 	// left wheel
 	analogWrite(leftWheel.IN1, vel);
 	analogWrite(leftWheel.IN2, 0 );
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
  	analogWrite(rightWheel.IN1, vel);
 	analogWrite(rightWheel.IN2, 0 );

 	// left wheel
 	analogWrite(leftWheel.IN1, 0);
 	analogWrite(leftWheel.IN2, 0);
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
  	analogWrite(rightWheel.IN1, 0 );
 	analogWrite(rightWheel.IN2, vel);

 	// left wheel
 	analogWrite(leftWheel.IN1, vel);
 	analogWrite(leftWheel.IN2, 0 );
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
  	analogWrite(rightWheel.IN1, vel);
 	analogWrite(rightWheel.IN2, 0 );

 	// left wheel
 	analogWrite(leftWheel.IN1, 0 );
 	analogWrite(leftWheel.IN2, vel);
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
  	analogWrite(rightWheel.IN1, 0 );
 	analogWrite(rightWheel.IN2, vel);

 	// left wheel
 	analogWrite(leftWheel.IN1, 0 );
 	analogWrite(leftWheel.IN2, vel);
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
	analogWrite(rightWheel.IN1, 0);
	analogWrite(rightWheel.IN1, 0);
	analogWrite(leftWheel.IN1 , 0);
	analogWrite(leftWheel.IN2 , 0);

	leftWheel.RPM = MIN_RPM;
	rightWheel.RPM = MIN_RPM;
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
	uint8 const prevLeftRPM = leftWheel.RPM;
	uint8 const prevRightRPM = rightWheel.RPM;
	uint8 leftRPM;
	uint8 rightRPM;
	
	leftRPM = (uint8)(LPF_Factor * (float)prevLeftRPM + (1.0 - LPF_Factor) * (WHEEL_RPM_FACTOR / elapsedTimeLeft));
	rightRPM = (uint8)(LPF_Factor * (float)prevRightRPM + (1.0 - LPF_Factor) * (WHEEL_RPM_FACTOR / elapsedTimeRight));

	leftWheel.RPM = leftRPM;
	rightWheel.RPM = rightRPM;
}

/**********************************************************
*  Function ddrInit()
*
*  Brief: DDR initialization (to be excecuted in void setup()):
*           - Stop DDR
*           - Initialize speedometer variables
*           - Set interrupts for speedometers
*
*  Inputs: None
*
*  Outputs: void
*
*  Wire Inputs: None
*
*  Wire Outputs: None
**********************************************************/
void ddrInit(DDR const * const ddr)
{
	/* Stop vehicle */
	ddr->stop();

	/* Get left and right wheels */
	Wheel leftWheel = ddr->leftWheel;
	Wheel rightWheel = ddr->rightWheel;

	/* Initialize speedometer variables */
	prevTimeLeft = millis();
	prevTimeRight = millis();

	/* Set interrupts for speedometers */
	pinMode(leftWheel.SPEED_INTERRUPT, INPUT_PULLUP);
	pinMode(rightWheel.SPEED_INTERRUPT, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(leftWheel.SPEED_INTERRUPT), interruptLeftWheel, FALLING);
	attachInterrupt(digitalPinToInterrupt(rightWheel.SPEED_INTERRUPT), interruptRightWheel, FALLING);

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
  elapsedTimeLeft = (float)(currentTime - prevTimeLeft) * MILLIS_TO_MINUTES;
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
  elapsedTimeRight = (float)(currentTime - prevTimeRight) * MILLIS_TO_MINUTES;
  prevTimeRight = currentTime;
}
