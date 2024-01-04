/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Inputs: None
*
*  Outputs: INS -> IN1, IN2, IN3, IN4
******************************************************************************/
#include "Arduino.h"
#include "DDR.h"

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
*  Function forward()
*
*  Brief: DDR wheels are set to move forward
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: left wheel command to vel
*           left wheel command to 0
*           left wheel command to vel
*           left wheel command to 0
**********************************************************/
void DDR::forward(uint8 const vel)
{
	// rigth Wheel
  	analogWrite(rightWheel.IN1, vel);
 	analogWrite(rightWheel.IN2, 0 );

 	// left wheel
 	analogWrite(leftWheel.IN1, vel);
 	analogWrite(leftWheel.IN2, 0 );
}

/**********************************************************
*  Function turnRight()
*
*  Brief: DDR wheels are set to turn right
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: left wheel command to 0
*           left wheel command to 0
*           left wheel command to vel
*           left wheel command to 0
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
*  Function turnLeft()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: left wheel command to vel
*           left wheel command to 0
*           left wheel command to 0
*           left wheel command to 0
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
*  Function turnRightFast()
*
*  Brief: DDR wheels are set to turn right fast
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: left wheel enabled to vel
*           left wheel command to 0
*           left wheel command to vel
*           left wheel command to vel
*           left wheel command to 0
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
*  Function turnLeftFast()
*
*  Brief: DDR wheels are set to turn left
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: left wheel enabled
*           left wheel command to vel
*           left wheel command to 0
*           left wheel command to 0
*           left wheel command to 0
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
*  Function backward()
*
*  Brief: DDR wheels are set to move backward
*
*  Inputs: vel -> desired velocity on the PWM cycle-duty range [0, 255]
*
*  Outputs: left wheel command to 0
*           left wheel command to vel
*           left wheel command to 0
*           left wheel command to vel
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
*  Function stop()
*
*  Brief: DDR wheels are stopped
*
*  Inputs: None
*
*  Outputs: left wheel command to 0
*           left wheel command to 0
*           left wheel command to 0
*           left wheel command to 0
**********************************************************/
void DDR::stop()
{
	analogWrite(rightWheel.IN1, 0);
	analogWrite(rightWheel.IN1, 0);
	analogWrite(leftWheel.IN1 , 0);
	analogWrite(leftWheel.IN2 , 0);
}
