#ifndef HCSR04_H
#define HCSR04_H

#include "Arduino.h"
#include "../typeDefs/typeDefs.h"

class HCSR04
{
	public:
		HCSR04(uint8 const TRIGGER, uint8 const ECHO);
		uint16 measureDistance();

	private:
		uint8 trigger;
		uint8 echo;

};

#endif
