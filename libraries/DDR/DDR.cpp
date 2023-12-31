/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Inputs: None
*
*  Outputs: ENS -> ENA and ENB
*           INS -> IN1, IN2, IN3, IN4
******************************************************************************/
#include "Arduino.h"
#include "DDR.h"

DDR::DDR(Wheel const LEFTWHEEL, Wheel const RIGHTWHEEL)
{
	/* Left Wheel */
	pinMode(LEFTWHEEL.EN , OUTPUT);
	pinMode(LEFTWHEEL.IN1, OUTPUT);
	pinMode(LEFTWHEEL.IN2, OUTPUT);

	/* Rigth Wheel */
	pinMode(RIGHTWHEEL.EN , OUTPUT);
	pinMode(RIGHTWHEEL.IN1, OUTPUT);
	pinMode(RIGHTWHEEL.IN2, OUTPUT);

	leftWheel  = LEFTWHEEL;
	rightWheel = RIGHTWHEEL;

}

/**********************************************************
*  Function forward()
*
*  Brief: DDR wheels are set to move forward
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: rigth wheel enabled to vel
*           left wheel enabled to vel
*           left wheel command to HIGH
*           left wheel command to LOW
*           left wheel command to HIGH
*           left wheel command to LOW
**********************************************************/
void DDR::forward(uint8 const vel)
{
	// rigth Wheel
  	analogWrite (rightWheel.EN , vel );
  	digitalWrite(rightWheel.IN1, HIGH);
 	digitalWrite(rightWheel.IN2, LOW );

 	// left wheel
 	analogWrite (leftWheel.EN , vel );
 	digitalWrite(leftWheel.IN1, HIGH);
 	digitalWrite(leftWheel.IN2, LOW );
}

/**********************************************************
*  Function turnRight()
*
*  Brief: DDR wheels are set to turn right
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: rigth wheel enabled
*           left wheel enabled to vel
*           left wheel command to LOW
*           left wheel command to LOW
*           left wheel command to HIGH
*           left wheel command to LOW
**********************************************************/
void DDR::turnRight(uint8 const vel)
{
	// rigth Wheel
  	analogWrite (rightWheel.EN , 255);
  	digitalWrite(rightWheel.IN1, LOW);
 	digitalWrite(rightWheel.IN2, LOW);

 	// left wheel
 	analogWrite (leftWheel.EN , vel );
 	digitalWrite(leftWheel.IN1, HIGH);
 	digitalWrite(leftWheel.IN2, LOW );
}

/**********************************************************
*  Function turnLeft()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: rigth wheel enabled to vel
*           left wheel enabled
*           left wheel command to HIGH
*           left wheel command to LOW
*           left wheel command to LOW
*           left wheel command to LOW
**********************************************************/
void DDR::turnLeft(uint8 const vel)
{
	// rigth Wheel
  	analogWrite (rightWheel.EN , vel );
  	digitalWrite(rightWheel.IN1, HIGH);
 	digitalWrite(rightWheel.IN2, LOW );

 	// left wheel
 	analogWrite (leftWheel.EN , 255);
 	digitalWrite(leftWheel.IN1, LOW);
 	digitalWrite(leftWheel.IN2, LOW);
}

/**********************************************************
*  Function turnRightFast()
*
*  Brief: DDR wheels are set to turn right fast
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: rigth wheel enabled to vel
*           left wheel enabled to vel
*           left wheel command to LOW
*           left wheel command to HIGH
*           left wheel command to HIGH
*           left wheel command to LOW
**********************************************************/
void DDR::turnRightFast(uint8 const vel)
{
	// rigth Wheel
  	analogWrite (rightWheel.EN , vel );
  	digitalWrite(rightWheel.IN1, LOW );
 	digitalWrite(rightWheel.IN2, HIGH);

 	// left wheel
 	analogWrite (leftWheel.EN , vel );
 	digitalWrite(leftWheel.IN1, HIGH);
 	digitalWrite(leftWheel.IN2, LOW );
}

/**********************************************************
*  Function turnLeftFast()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: rigth wheel enabled to vel
*           left wheel enabled
*           left wheel command to HIGH
*           left wheel command to LOW
*           left wheel command to LOW
*           left wheel command to LOW
**********************************************************/
void DDR::turnLeftFast(uint8 const vel)
{
	// rigth Wheel
  	analogWrite (rightWheel.EN , vel );
  	digitalWrite(rightWheel.IN1, HIGH);
 	digitalWrite(rightWheel.IN2, LOW );

 	// left wheel
 	analogWrite (leftWheel.EN , vel );
 	digitalWrite(leftWheel.IN1, LOW );
 	digitalWrite(leftWheel.IN2, HIGH);
}

/**********************************************************
*  Function backward()
*
*  Brief: DDR wheels are set to move backward
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: rigth wheel enabled to vel
*           left wheel enabled to vel
*           left wheel command to LOW
*           left wheel command to HIGH
*           left wheel command to LOW
*           left wheel command to HIGH
**********************************************************/
void DDR::backward(uint8 const vel)
{
	// rigth Wheel
  	analogWrite (rightWheel.EN , vel );
  	digitalWrite(rightWheel.IN1, LOW );
 	digitalWrite(rightWheel.IN2, HIGH);

 	// left wheel
 	analogWrite (leftWheel.EN , vel );
 	digitalWrite(leftWheel.IN1, LOW );
 	digitalWrite(leftWheel.IN2, HIGH);
}

/**********************************************************
*  Function stop()
*
*  Brief: DDR wheels are stopped
*
*  Inputs: None
*
*  Outputs: rigth wheel enabled
*           left wheel enabled
*           left wheel command to LOW
*           left wheel command to LOW
*           left wheel command to LOW
*           left wheel command to LOW
**********************************************************/
void DDR::stop()
{
	analogWrite(rightWheel.EN , 255);
	analogWrite(leftWheel.EN  , 255);

	digitalWrite(rightWheel.IN1, LOW);
	digitalWrite(rightWheel.IN1, LOW);
	digitalWrite(leftWheel.IN1 , LOW);
	digitalWrite(leftWheel.IN2 , LOW);
}
