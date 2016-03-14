// Enum.h

#ifndef Enum_h
#define Enum_h
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
// The different states of the system
enum states
{
    SHOW_TIME,           // Displays the time and date
    SET_TIME_YEAR,       //Setting the year
    SET_TIME_MONTH,      //Setting the Month
    SET_TIME_DAY,        //Setting the Day
    SET_TIME_HOUR,       //Setting the Hour
    SET_TIME_MINUTES,    //Setting the Minutes
    SHOW_TIME_AUT_MAN,   // Displays the time and date, and open is on
    SHOW_OPEN_TIME,      // Displays the open time and goes back to time and date after 3 seconds
    SET_OPEN_HOUR,      // Option for setting the open hours. If provided, it moves on to open minutes.
                         //   Otherwise, it times out after 5 seconds and returns to time and date
    SET_OPEN_MINUTES,   // Option for setting the open minutes.
                         //   Otherwise, it times out after 5 seconds and returns to time and date      
    SHOW_CLOSE_TIME,     // Displays the close time and goes back to time and date after 3 seconds
    SET_CLOSE_HOUR,     // Option for setting the close hours, If provided, it moves on to close minutes
    SET_CLOSE_MINUTES,  // Option for setting the close minutes.
    SHOW_LUX,           // Show the light level
    SET_LUX_OPEN,      // Option for setting the light level for open the door
    SET_LUX_CLOSE,       // Option for setting the dark level for close the door
    ACTIVATE,           //Option for activate timen and lux 
    ACTIVATE_TIME,      // Selection on/off timer aperture/clos
    ACTIVATE_LUX,       // Selection on/off lux aperture/close
    DOOR_OPEN,         // Activate open the door 
    DOOR_CLOSE         // activate close the door
};

// variable to store hour in memory
typedef struct DATOS{
   uint8_t   hourOpen;
   uint8_t   minuteOpen;
   uint8_t   hourClose;
   uint8_t   minuteClose;
   uint8_t   memLuxOpen;
   uint8_t   memLuxClose;
   boolean   activateTime;
   boolean   activateLux;
   boolean   usable;
} Master;

byte newChar[8] = {
        B00100,
        B10101,
        B01110,
        B11011,
        B01110,
        B10101,
        B00100,
        B00000
};

#endif //Enum_h

//EOF