

#ifndef Dates_h
#define Dates_h
#include <Wire.h>
#include <LiquidCrystal.h>  // Required by LCDKeypad
#include <LCDKeypad.h>
#include "RTClib.h" //Library RTC Module 1307
#include "Config.h"
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
class Dates
{

private:
	LCDKeypad lcd;
	RTC_DS1307 RTC;
	DateTime _now;
	boolean _open;
	bool _luxOpen;
	bool _puntos;
	bool _timeout;
	unsigned long _timeRef;
public:
	//Dates();
	void showTime(bool, bool, bool,LCDKeypad*);
	int setTimeYear(int*, LCDKeypad*, char*, char*, uint8_t, uint8_t);
	int setTimeMont(uint8_t*);
	int setTimeDay(uint8_t*);
	int setTimeHours(uint8_t*);
	int setTimeMinutes(uint8_t*);
	int showTime(uint8_t*, uint8_t*, LCDKeypad*, char*);
	int setLuxOpen(uint8_t*);
	
};












#endif //Dates_h

//EOF