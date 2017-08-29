/*
 * MT_BoardWheels.h - Library for long/short/skate board wheel setup. Used in longboardLight1 project..
 * Latching bipolar hall effect sensor(s) mounted on the chassis, with 4/8 magnets mounted on the adjacent wheel(s).
 * Add one array per wheel sensor.
 * Created by MTS Standish (mattThurstan), 2017.
 * Copyleft.
 */
 
#ifndef __MT_BOARDWHEELS_H__
#define __MT_BOARDWHEELS_H__
 
#include "Arduino.h"

typedef struct {
	byte sensorPin;
	float radius;
	float circumference;
	byte magnetTotal;
	volatile byte counter;
	unsigned long sensorReadPrevMillis;
	double rps;
	double mps;
	unsigned long distTraveledForward;
	unsigned long distTraveledBackward;
} WHEEL;

/*----------------------------main header declerations----------------------------*/
class MT_BoardWheels
{
  private:
	byte _wheelSensorTotal;
	WHEEL *wheels;
  
  public:
	MT_BoardWheels(byte wheelSensorTotal);
	void Init(byte wheel, byte sensorPin, float radius, byte magnetTotal);
	void SetCounter(byte wheel, byte value); //set counter to value
	void IncrementCounter(byte wheel);	//increment counter by 1
	void DecrementCounter(byte wheel);	//decrement counter by 1
	void ResetAllWheels();
	void ResetWheel(byte wheel);
	void DoUpdate(byte direction);		//call every 1 second from main program loop

};

#endif