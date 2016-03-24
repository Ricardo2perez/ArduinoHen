// Config.h


// ensure this library description is only included once
#ifndef Config_h
#define Config_h
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#define TIME_OUT 5  // One of the system's FSM transitions
#define OPEN_TIME_MET 6  // One of the system's FSM transitions
#define BUZZER_PIN 3  // Output PWM pin for the buzzer
#define SNOOZE 10  // Minutes to snooze
#define ADDRESS 0 // ADDRESS to store hours in eeprom
#define POSHOUROPEN 0 // Eeprom memory position to store open hour
#define POSMINUOPEN 1 // Eeprom memory position to store open minute
#define POSHOURCLOSE 2 // Eeprom memory position to store close hour
#define POSMINUCLOSE 3 // Eeprom memory position to store close minute
#define POSLUXOPEN  4 // Eeprom memory position to store open lux
#define POSLUXCLOSE 5 // Eeprom memory postion to store close lux
#define POSACTIME 6 // Eeprom memory position to store enable time control
#define POSACTLLUX 7 // Eeprom memory position to store enable lux control
#define POSUSABLE 8
#define SUBEP1 30
#define BAJAP1 32
#define P1ARRIBA 34
#define P1ABAJO 36
#define SUBEP2 38
#define BAJAP2 40
#define P2ARRIBA 42
#define P2ABAJO 44
#define TEXTO_OPEN "Open Time"
#define TEXTO_CLOSE "Close Time"
#endif // _CONFIGURATION_H

// EOF




 
