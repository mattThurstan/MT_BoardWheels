/*
 * MT_BoardWheels.cpp - Library for long/short/skate board wheel setup. Used in longboardLight1 project..
 * Latching bipolar hall effect sensor(s) mounted on the chassis, with 4/8 magnets mounted on the adjacent wheel(s).
 * Add one array per wheel sensor.
 * Created by MTS Standish (mattThurstan), 2017.
 * Copyleft.
 */
 
#include "Arduino.h"
#include "MT_BoardWheels.h"

//#define DEBUG	//comment/un-comment

#ifdef DEBUG
  #define DEBUG_PRINT_HEADER(x); Serial.print(F("MT_BoardWheels - Wheel "))
  #define DEBUG_PRINT(x);    Serial.print(x)
  #define DEBUG_PRINTF(x);    Serial.print(F(x))
  #define DEBUG_PRINTLN(x);  Serial.println(x)
  #define DEBUG_PRINTLNF(x); Serial.println(F(x))
#else
  #define DEBUG_PRINT_HEADER(x)
  #define DEBUG_PRINT(x)     //blank line
  #define DEBUG_PRINTF(x)    //blank line
  #define DEBUG_PRINTLN(x)   //blank line
  #define DEBUG_PRINTLNF(x)  //blank line
#endif
 
MT_BoardWheels::MT_BoardWheels(byte wheelSensorTotal) {
	//wheels = (struct *)malloc(sizeof(struct) * wheelTotal);
	//wheels = malloc(sizeof(*struct) * wheelTotal);
	wheels = malloc(wheelSensorTotal * sizeof(*wheels));
	_wheelSensorTotal = wheelSensorTotal;
	//set defaults ???
}

void MT_BoardWheels::Init(byte wheel, byte sensorPin, float radius, byte magnetTotal) {
	if (wheel < 0) { wheel = 0; } 
	else if (wheel >= _wheelSensorTotal) { wheel = _wheelSensorTotal-1; }
	
	wheels[wheel].sensorPin = sensorPin;
	wheels[wheel].radius = radius;
	wheels[wheel].circumference = 2 * PI * radius;
	wheels[wheel].magnetTotal = magnetTotal;
	wheels[wheel].counter = 0;
	wheels[wheel].sensorReadPrevMillis = 0;
	wheels[wheel].rps = 0;
	wheels[wheel].mps = 0;
	wheels[wheel].distTraveledForward = 0;
	wheels[wheel].distTraveledBackward = 0;
	
	DEBUG_PRINT_HEADER()
	DEBUG_PRINT(wheel);
	DEBUG_PRINTLNF(" setup.");
}

void MT_BoardWheels::SetCounter(byte wheel, byte value) {
	wheels[wheel].counter = value;
}

void MT_BoardWheels::IncrementCounter(byte wheel) {
	wheels[wheel].counter++;
}

void MT_BoardWheels::DecrementCounter(byte wheel) {
	wheels[wheel].counter--;
}

void MT_BoardWheels::ResetAllWheels() {
	for (byte i = 0; i < _wheelSensorTotal; i++) {
		ResetWheel(i);
	}
}

void MT_BoardWheels::ResetWheel(byte wheel) {
	if (wheel < 0) { wheel = 0; } 
	else if (wheel >= _wheelSensorTotal) { wheel = _wheelSensorTotal-1; }
	
	wheels[wheel].counter = 0;
	wheels[wheel].sensorReadPrevMillis = 0;
	wheels[wheel].rps = 0;
	wheels[wheel].mps = 0;
	wheels[wheel].distTraveledForward = 0;
	wheels[wheel].distTraveledBackward = 0;
	
	DEBUG_PRINT_HEADER();
	DEBUG_PRINT(wheel);
	DEBUG_PRINTLNF(" reset.");
}

/* DoUpdate - call from a timed-loop every second.
 * This NEEDS to be called every 1 sec, otherwise we don't get 'revolutions per SECOND'!
 * Remember to detachInterrupt before calling,
 * and (re-)attachInterrupt after calling.
 * direction: 0=forward, 1=backward, 2=
 */
void MT_BoardWheels::DoUpdate(byte direction) {
	for (byte i = 0; i < _wheelSensorTotal; i++) {
		wheels[i].rps = wheels[i].counter / wheels[i].magnetTotal;
		wheels[i].mps = wheels[i].rps * wheels[i].circumference; //speed in meters per second. distance travelled in 1 second
		
		if (direction == 0) {
		  wheels[i].distTraveledForward = (wheels[i].distTraveledForward + (unsigned long)wheels[i].mps);
		} else if (direction == 1) {
		  wheels[i].distTraveledBackward = (wheels[i].distTraveledBackward + (unsigned long)wheels[i].mps);
		}
		
		wheels[i].counter = 0; //reset the counter to 0 after reading
		
		DEBUG_PRINT_HEADER();
		DEBUG_PRINT(i);
		DEBUG_PRINTF(" - RPS = ");
		DEBUG_PRINT(wheels[i].rps);
		DEBUG_PRINTF(", MPS = ");
		DEBUG_PRINT(wheels[i].mps);
		DEBUG_PRINTF(", Forward = ");
		DEBUG_PRINT(wheels[i].distTraveledForward);
		DEBUG_PRINTF(", Backward = ");
		DEBUG_PRINT(wheels[i].distTraveledBackward);
		DEBUG_PRINTLNF(".");
	}
}
