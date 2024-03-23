/******************************************************************************
*						DDR
*
*  Author : Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  brief: Commands used for DDR (Differential Driven Robot) using the L298N module.
*
*  Inputs:  INS -> IN1, IN2, IN3, IN4
*
*  Outputs: None
******************************************************************************/
#ifndef DDR_h
#define DDR_h

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

/******************* DEFINES *********************/
#define  MILLIS_TO_MINUTES  (1.666e-5)  /* Factor to convert miliseconds to minutes */
#define  WHEEL_RPM_FACTOR   (0.2f)      /* Factor of distribution of magnets in wheels */
#define  LPF_Factor         (0.25f)     /* Low Pass Filter factor to smooth RPM signals */
#define  STOP_RPM           (0u)        /* RPM when stop */
#define  MIN_RPM            (60u)       /* Minimum allowed RPM (determined experimentally) */
#define  MAX_RPM            (170u)      /* Maximum allowed RPM (determined experimentally) */
#define  ONE_F              (1.0f)      /* Constant 1 float*/

/*************************************************/

typedef struct PID_ks{
	float32 f_kp;
	float32 f_ki;
	float32 f_kd;
} PID_ks;

typedef struct PID_vars{
	PID_ks pid_ks;
	float32 f_cumError;
	uint32 u_prevTime;
	sint16 s_prevError;
	uint8 u_pidControl;
} PID_vars;

typedef struct Wheel{
	PID_vars PID_vars;
	uint8 u_in1;
	uint8 u_in2;
	uint8 u_speedInterrupt;
	uint8 u_velRPM;
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
		

	//private:
		void PIDinit();
		void getRPM(Wheel * const wheel, float32 const elapsedTime);
		uint8 velPIDcontrol(Wheel * const wheel, float32 const elapsedTime, uint8 const desiredVel);
		Wheel leftWheel;
		Wheel rightWheel;
};

void interruptLeftWheel();

void interruptRightWheel();

#endif
