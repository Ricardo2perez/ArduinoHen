#include "Config.h"
#include "Enum.h"
#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h>  // Required by RTClib
#include <LiquidCrystal.h>  // Required by LCDKeypad
#include <LCDKeypad.h>
#include "RTClib.h" //Library RTC Module 1307
#include <EEPROM.h>
#include <Dates.h>

union MEMORIA{
   DATOS dato;
   byte  b[sizeof(DATOS)];
}
miMemoria;

 
// Creates an LCDKeypad instance
// It handles the LCD screen and buttons on the shield
LCDKeypad lcd;

// Creates an RTC_DS1307 instance
// It handles the DS1307 Real-Time Clock
RTC_DS1307 RTC;

Dates fecha;
states state;  // Holds the current state of the system

int8_t button;  // Holds the current button pressed
uint8_t openHours = 0, openMinutes = 0;  // Holds the current open time
uint8_t closeHours = 0, closeMinutes = 0;
uint8_t timeHours = 0, timeMinutes = 0;
uint8_t tmpHours = 0, tmpMonth = 0, tmpDay = 0, tmpMinutes = 0;
uint8_t levelLuxOpen = 0, levelLuxClose = 0;
int  tmpYear = 2016;
boolean open = false;  // Holds the current state of the open  gate
boolean close = false; // Holds the current state of the close gate
boolean luxOpen = false; // Holds the current state of the copen lux Level
boolean luxClose = false; // Holds the current state of the close Lux Level
unsigned long timeRef;
DateTime now;  // Holds the current date and time information
uint8_t time =0;

int ledPin = 13;
boolean puntos = false;
// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//My Thread
Thread myThread = Thread();
//His Thread
Thread hisThread = Thread();
//Blink Led Thread
Thread blinkLedThread = Thread();
//ThreadController, that will be added to controll
ThreadController groupOfThreads = ThreadController();

// callback for myThread
void lcdControl(){
    Serial.print("COOL! I'm running on: ");
    Serial.println(millis());
     timeRef = millis();

    // Uses the current state to decide what to process
    switch (state)
    {
        case SHOW_TIME:
            showTime();
            checkOpenTime();
            checkCloseTime();
            checkOpenLux();
            checkCloseLux();
            break;
        case SHOW_TIME_AUT_MAN:
            showTime();
            checkOpenTime();
            checkCloseTime();
            checkOpenLux();
            checkCloseLux();
            break;
        case SET_TIME_YEAR:
            setTimeYear();
            break;
        case SET_TIME_MONTH:
            setTimeMont();
            break;
        case SET_TIME_DAY:
            setTimeDay();
            break;
        case SET_TIME_HOUR:
            setTimeHours();
            break;
        case SET_TIME_MINUTES:
            setTimeMinutes();
            break;
        case SHOW_OPEN_TIME:
            showOpenTime();
            break;
        case SET_OPEN_HOUR:
            SetOpenHours();
            if ( state != SET_OPEN_MINUTES ) 
            break;
        case SET_OPEN_MINUTES:
            SetOpenMinutes();
            break;
        case SHOW_CLOSE_TIME:
            showCloseTime();
            //checkCloseTime();
            break;
        case SET_CLOSE_HOUR:
            SetCloseHours();
            break;         
        case SET_CLOSE_MINUTES:
            SetCloseMinutes();
            break;
        case SHOW_LUX:
            Showlux();
            break;
        case SET_LUX_OPEN:
            SetLuxOpen();
            break;
        case SET_LUX_CLOSE:
            SetLuxClose();
            break; 
        case ACTIVATE:
            ShowActivate();
            break; 
        case ACTIVATE_TIME:
            ShowActivateTime();
            break;   
        case ACTIVATE_LUX:
            ShowActivateLux();
            break; 
        case DOOR_OPEN:
            showTime();
            break;
    }

    // Waits about 1 sec for events (button presses)
    // If a button is pressed, it blocks until the button is released
    // and then it performs the applicable state transition
    while ( (unsigned long)(millis() - timeRef) < 970 )
    {
        if ( (button = lcd.button()) != KEYPAD_NONE )
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            transition(button);
            break;
        }
    }
}

// callback for hisThread
void boringCallback(){
    time += 1;
    if (time >10000){
        time = 10000;
    }
    Serial.print("Time value...");
    Serial.println(time);
}

// callback for blinkLedThread
void blinkLed(){
    static bool ledStatus = false;
    ledStatus = !ledStatus;

    digitalWrite(ledPin, ledStatus);

    Serial.print("blinking: ");
    Serial.println(ledStatus);
}
// Looks at the provided trigger (event) 
// and performs the appropriate state transition
// If necessary, sets secondary variables
void transition(uint8_t trigger)
{
    switch (state)
    {
        case SHOW_TIME:
            if ( trigger == KEYPAD_RIGHT ) state = SHOW_OPEN_TIME;
            /*else if ( trigger == KEYPAD_LEFT ) { state = SHOW_TIME_AUT_MAN; } */
            else if ( trigger == KEYPAD_SELECT ) state = SET_TIME_YEAR;
            else if ( trigger == OPEN_TIME_MET ) {openDoor(); }
            break;
      /*  case SHOW_TIME_AUT_MAN:
            if ( trigger == KEYPAD_RIGHT ) state = SHOW_OPEN_TIME;
            else if ( trigger == KEYPAD_LEFT ) { state = SHOW_TIME; }
            else if ( trigger == KEYPAD_SELECT ) state = SET_OPEN_HOUR;
            else if ( trigger == OPEN_TIME_MET ) { analogWrite(BUZZER_PIN, 220); state =DOOR_OPEN; }
            break; */
        case SET_TIME_YEAR:
            if ( trigger == KEYPAD_SELECT ) state = SET_TIME_MONTH;
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_TIME_MONTH:
            if ( trigger == KEYPAD_SELECT ) state = SET_TIME_DAY;
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_TIME_DAY:
             if ( trigger == KEYPAD_SELECT ) state = SET_TIME_HOUR;
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_TIME_HOUR:
            if ( trigger == KEYPAD_SELECT ) state = SET_TIME_MINUTES;
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_TIME_MINUTES:
            if ( trigger == TIME_OUT || trigger == KEYPAD_SELECT ) { state = SHOW_TIME; }
            break;

        case SHOW_OPEN_TIME:
            if ( trigger == KEYPAD_RIGHT ) state = SHOW_CLOSE_TIME;
            else if ( trigger == KEYPAD_SELECT ) state = SET_OPEN_HOUR;
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_OPEN_HOUR:
            if ( trigger == KEYPAD_SELECT ) state = SET_OPEN_MINUTES;
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_OPEN_MINUTES:
            if ( trigger == KEYPAD_SELECT ) { open = true; state = SHOW_OPEN_TIME; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
             break;
        case SHOW_CLOSE_TIME:
            if ( trigger == KEYPAD_RIGHT ) state = SHOW_LUX; 
            else if ( trigger == KEYPAD_SELECT ) { state = SET_CLOSE_HOUR; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
             break; 
        case SET_CLOSE_HOUR:

            if ( trigger == KEYPAD_SELECT ) { state = SET_CLOSE_MINUTES; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_CLOSE_MINUTES:
            if ( trigger == KEYPAD_SELECT ) {  state = SHOW_CLOSE_TIME; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SHOW_LUX:
            if ( trigger == KEYPAD_RIGHT ) state = ACTIVATE;
            else if ( trigger == KEYPAD_SELECT ) { state = SET_LUX_OPEN; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_LUX_OPEN:
            if ( trigger == KEYPAD_SELECT) { state = SET_LUX_CLOSE; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case SET_LUX_CLOSE:
            if ( trigger == KEYPAD_SELECT ) { luxOpen = true; state = SHOW_LUX; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case ACTIVATE:
            if ( trigger == KEYPAD_RIGHT ) state = SHOW_TIME;
            else if ( trigger == KEYPAD_SELECT) { state = ACTIVATE_TIME; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break; 
        case ACTIVATE_TIME:
            if ( trigger == KEYPAD_SELECT) { state = ACTIVATE_LUX; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case ACTIVATE_LUX:
            if ( trigger == KEYPAD_SELECT) { state = ACTIVATE; }
            else if ( trigger == TIME_OUT ) { state = SHOW_TIME; }
            break;
        case DOOR_OPEN:
            if ( trigger == KEYPAD_UP || trigger == KEYPAD_DOWN ) { openDoor(); }
            if ( trigger == KEYPAD_SELECT || trigger == KEYPAD_LEFT ) { state = SHOW_TIME; }
            break;
    }
}
// Displays the current date and time, and also an open indication
// e.g. SAT 04 JAN 2014, 22:59:10  open
void showTime()
{
    puntos = !puntos;
    fecha.showTime(open, luxOpen, puntos);
    
    
    
    /*
     now = RTC.now();
    const char* dayName[] = { "DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB" };
    const char* monthName[] = { "ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DEC" };
    
    lcd.clear();
    

   lcd.print(String(dayName[now.dayOfWeek()]) + " " +
              (now.day() < 10 ? "0" : "") + now.day() + " " +
              monthName[now.month()-1] + " " + now.year());
    lcd.setCursor(0,1);
    lcd.print((now.hour() < 10 ? "0" : "") + String(now.hour()) +(puntos? ":":" ") +
              (now.minute() < 10 ? "0" : "") + now.minute() + 
              //":" +
              //(now.second() < 10 ? "0" : "") + now.second() +
               (open ? "  open" : "")+ (luxOpen ? "  lux":""));*/

}

 void setTimeYear()
 {
    transition(fecha.setTimeYear(&tmpYear));
 }

void setTimeMont()
{
   transition(fecha.setTimeMont(&tmpMonth));
}

void setTimeDay()
{
    transition(fecha.setTimeDay(&tmpDay));

}

void setTimeHours()
{
   transition(fecha.setTimeHours(&tmpHours));
}


void setTimeMinutes()
{  
    int receive = fecha.setTimeMinutes(&tmpMinutes);

    if ( receive == KEYPAD_SELECT )
    {
        //update hour and minutes
        timeHours = tmpHours;
        timeMinutes = tmpMinutes;
         // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        RTC.adjust(DateTime(tmpYear,tmpMonth,tmpDay,timeHours,timeMinutes,0));
        transition(KEYPAD_SELECT);
    }
    else transition(TIME_OUT);

}

void showOpenTime()
{
    int receive = fecha.showOpenTime(&openHours, &openMinutes);
    transition(receive);

}

void showCloseTime()
{
   int receive = fecha.showCloseTime(&closeHours, &closeMinutes);
   transition(receive);
   
}
 

void checkCloseTime()
{
  
}

void checkOpenLux()
{
}

void checkCloseLux()
{
}
void SetLuxOpen()
{
   int receive = fecha.setLuxOpen(&levelLuxOpen);
   if ( receive == KEYPAD_SELECT){
     EEPROM.write( ADDRESS+POSLUXOPEN , levelLuxOpen );
   }
  

   transition(receive);
}
void SetLuxClose()
{
    unsigned long timeRef;
    boolean timeOut = true;
    
    
    lcd.clear();
    lcd.print("Lux Close");

   

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set Lux: ");
    lcd.setCursor(11,1);
    if ( levelLuxClose < 10 ) lcd.print(" ");
    lcd.print(levelLuxClose);

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            levelLuxClose = levelLuxClose < 500 ? levelLuxClose + 1 : levelLuxClose;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( levelLuxClose < 10 ) lcd.print(" ");
            lcd.print(levelLuxClose);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            levelLuxClose = levelLuxClose > 0 ? levelLuxClose - 1 : levelLuxClose;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( levelLuxClose < 10 ) lcd.print(" ");
            lcd.print(levelLuxClose);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut )
    {
     
       EEPROM.write( ADDRESS+POSLUXCLOSE , levelLuxClose );
     transition(KEYPAD_SELECT);}
    else transition(TIME_OUT);
}
void ShowActivate()
{
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t pushed = 0;
    timeRef = millis();

    lcd.clear();
    lcd.print("Activation");
    lcd.setCursor(0,1);
    lcd.print(String("T: ") + (open == 1? "ON ":"OFF") + 
                   " L: " + (luxOpen == 1? "ON ":"OFF"));
      while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_RIGHT ) // Transition
        {
           while ( lcd.button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_RIGHT;
            break;
        }
       
        else if ( button == KEYPAD_SELECT ) //Enter to modify values
        {
           
            while ( lcd.button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_SELECT;
            break;
           
        }
        delay(100);
    }

    if ( !timeOut ){
         transition(pushed);

        }
    else transition(TIME_OUT);


}
void ShowActivateTime(){

    unsigned long timeRef;
    boolean timeOut = true;
        
    lcd.clear();
    lcd.print("Active Time ");

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Activation: ");
    lcd.setCursor(13,1);
   
    lcd.print (open == 1? "ON ":"OFF");
    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();
        
        if ( button == KEYPAD_UP )
        {
            open = !open;
            lcd.setCursor(13,1);
            lcd.print(open == 1? "ON ":"OFF");
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN )
        {
            open = !open;
            lcd.setCursor(13,1);
            lcd.print(open == 1? "ON ":"OFF");
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT )
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut )
    {
        
        EEPROM.write( ADDRESS+POSACTIME , open );
        transition(KEYPAD_SELECT);
    }
    else transition(TIME_OUT);

}
void ShowActivateLux(){
    unsigned long timeRef;
    boolean timeOut = true;
        
    lcd.clear();
    lcd.print("Active Light ");

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Activation: ");
    lcd.setCursor(13,1);
   
    lcd.print (luxOpen == 1? "ON ":"OFF");
    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();
        
        if ( button == KEYPAD_UP )
        {
            luxOpen = !luxOpen;
            lcd.setCursor(13,1);
            lcd.print(luxOpen == 1? "ON ":"OFF");
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN )
        {
             luxOpen = !luxOpen;
            lcd.setCursor(13,1);
            lcd.print(luxOpen == 1? "ON ":"OFF");
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT )
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut )
    {
        
        EEPROM.write( ADDRESS+POSACTLLUX , luxOpen );
        transition(KEYPAD_SELECT);
    }
    else transition(TIME_OUT);


}
// Checks if the open time has been met, 
// and if so initiates a state transition
void checkOpenTime()
{
    if ( now.hour() == openHours && now.minute() == openMinutes && open )
    {
     transition(OPEN_TIME_MET);
    }
    else digitalWrite(SUBEP1, OFF);
}

// When the buzzer is ringing, by pressing the UP or DOWN buttons, 
// a SNOOZE (default is 10) minutes delay on the open time happens
void openDoor()
{
  /* openMinutes += SNOOZE;
    if ( openMinutes > 59 )
    {
        openHours += openMinutes / 60;
        openMinutes = openMinutes % 60;
    }*/
    boolean puerta1Abierta;
    boolean puerta1Cerrada;
    boolean subirPuerta1;
    boolean subirPuerta2;
    boolean abierta1=true;
    digitalWrite(SUBEP1,ON);

    puerta1Abierta = digitalRead(P1ARRIBA);
    puerta1Cerrada = digitalRead(P1ABAJO);
    unsigned long timeRef;
    boolean timeOut = true;
    /*
    if (puerta1Cerrada){ 
        abierta1 = false;
        digitalWrite(subirPuerta1, ON);
    }
      while ( (unsigned long)(millis() - timeRef) < 10000 )
    {
        
       if(puerta1Abierta){
        digitalWrite(subirPuerta1, OFF);
        timeOut = false;
        break;
       } 

          
        
        delay(100);
    } */

    if ( !timeOut ){
         transition(TIME_OUT);

        }
    else transition(TIME_OUT);


}

// The first of a 2 part process for setting the open time
// Receives the open time hour. If not provided within 5 sec,
// times out and returns to a previous (time and date) state
void SetOpenHours()
{
    unsigned long timeRef;
    boolean timeOut = true;
    
    lcd.clear();
    lcd.print("open Time");

    tmpHours = openHours;
    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print(String("Set hours: ") + (tmpHours  < 10 ? "0" : "")+tmpHours);

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            tmpHours = tmpHours < 23 ? tmpHours + 1 : tmpHours;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( tmpHours < 10 ) lcd.print(" ");
            lcd.print(tmpHours);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            tmpHours = tmpHours > 0 ? tmpHours - 1 : tmpHours;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( tmpHours < 10 ) lcd.print(" ");
            lcd.print(tmpHours);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut ) transition(KEYPAD_SELECT);
    else transition(TIME_OUT);
}

// The second of a 2 part process for setting the open time
// Receives the open time minutes. If not provided within 5 sec,
// times out and returns to a previous (time and date) state
// If minutes are provided, sets the open time and turns the open on
void SetOpenMinutes()
{
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t tmpMinutes = 0;
    tmpMinutes = openMinutes;
    lcd.clear();
    lcd.print("open Time");

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print(String("Set minutes: ") + (tmpMinutes  < 10 ? "0" : "")+tmpMinutes);;
    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();
        
        if ( button == KEYPAD_UP )
        {
            tmpMinutes = tmpMinutes < 60 ? tmpMinutes + 1 : tmpMinutes;
            lcd.setCursor(13,1);
            lcd.print("  ");
            lcd.setCursor(13,1);
            if ( tmpMinutes < 10 ) lcd.print("0");
            lcd.print(tmpMinutes);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN )
        {
            tmpMinutes = tmpMinutes > 0 ? tmpMinutes - 1 : tmpMinutes;
            lcd.setCursor(13,1);
            lcd.print("  ");
            lcd.setCursor(13,1);
            if ( tmpMinutes < 10 ) lcd.print("0");
            lcd.print(tmpMinutes);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT )
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut )
    {
        //update hour and minutes
        openHours = tmpHours;
        openMinutes = tmpMinutes;
        EEPROM.write( ADDRESS+POSHOUROPEN , openHours );
        EEPROM.write( ADDRESS+POSMINUOPEN , openMinutes );
        transition(KEYPAD_SELECT);
    }
    else transition(TIME_OUT);
}



// The first of a 2 part process for setting the open time
// Receives the open time hour. If not provided within 5 sec,
// times out and returns to a previous (time and date) state
void SetCloseHours()
{
    unsigned long timeRef;
    boolean timeOut = true;
    
    lcd.clear();
    lcd.print("Close Time");

    tmpHours = closeHours;
    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print(String("Set hours: ") + (tmpHours  < 10 ? "0" : "")+tmpHours);;
    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            tmpHours = tmpHours < 23 ? tmpHours + 1 : tmpHours;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( tmpHours < 10 ) lcd.print(" ");
            lcd.print(tmpHours);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            tmpHours = tmpHours > 0 ? tmpHours - 1 : tmpHours;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( tmpHours < 10 ) lcd.print(" ");
            lcd.print(tmpHours);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(100);
    }

    if ( !timeOut ) transition(KEYPAD_SELECT);
    else transition(TIME_OUT);
}

// The second of a 2 part process for setting the open time
// Receives the open time minutes. If not provided within 5 sec,
// times out and returns to a previous (time and date) state
// If minutes are provided, sets the open time and turns the open on
void SetCloseMinutes()
{
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t tmpMinutes = 0;
    tmpMinutes = closeMinutes;
    lcd.clear();
    lcd.print("Close Time");

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print(String("Set minutes: ") + (tmpMinutes  < 10 ? "0" : "")+tmpMinutes);;
    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();
        
        if ( button == KEYPAD_UP )
        {
            tmpMinutes = tmpMinutes < 60 ? tmpMinutes + 1 : tmpMinutes;
            lcd.setCursor(13,1);
            lcd.print("  ");
            lcd.setCursor(13,1);
            if ( tmpMinutes < 10 ) lcd.print(" ");
            lcd.print(tmpMinutes);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN )
        {
            tmpMinutes = tmpMinutes > 0 ? tmpMinutes - 1 : tmpMinutes;
            lcd.setCursor(13,1);
            lcd.print("  ");
            lcd.setCursor(13,1);
            if ( tmpMinutes < 10 ) lcd.print(" ");
            lcd.print(tmpMinutes);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT )
        {
            while ( lcd.button() != KEYPAD_NONE ) ;
            timeOut = false;
            break;
        }
        delay(100);
    }

    if ( !timeOut )
    {
        //update hour and minutes
       
        closeHours = tmpHours;
        closeMinutes = tmpMinutes;
        EEPROM.write( ADDRESS+POSHOURCLOSE , closeHours);
        EEPROM.write( ADDRESS+POSMINUCLOSE , closeMinutes);
        transition(KEYPAD_SELECT);
    }
    else transition(TIME_OUT);
}

void Showlux(){
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t tmpMinutes = 0;
    
    lcd.clear();
    lcd.print("Show Lux");

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Actual Lux = 999");
    

}
// Initialize values, read from memory
void initValues(){
      openHours = miMemoria.dato.hourOpen ;
      openMinutes = miMemoria.dato.minuteOpen;
      closeHours = miMemoria.dato.hourClose;
      closeMinutes = miMemoria.dato.minuteClose;
      levelLuxOpen = miMemoria.dato.memLuxOpen;
      levelLuxClose =  miMemoria.dato.memLuxClose;
      open = miMemoria.dato.activateTime;
      luxOpen = miMemoria.dato.activateLux;
}

void setup(){

     // Se recuperan los datos de la memoria EEPROM:
   for( int i=0 ; i<sizeof(DATOS) ; i++  )
      miMemoria.b[i] = EEPROM.read( ADDRESS+i );

      // Se comprueba que se hayan podido leer los datos (que no se haya leido basura)
   // No es la forma mas elegante de hacerlo pero funciona:
   if( miMemoria.dato.usable == true )
   {
     initValues();
   }
   // Si nunca se habian usado se inicializan todos los datos:
   else
   {
      miMemoria.dato.hourOpen  = 8;
      miMemoria.dato.minuteOpen = 0;
      miMemoria.dato.hourClose = 22;
      miMemoria.dato.minuteClose = 0;
      miMemoria.dato.memLuxOpen = 200;
      miMemoria.dato.memLuxClose = 100;
      miMemoria.dato.activateTime=true;
      miMemoria.dato.activateLux = true;
      miMemoria.dato.usable = true;

      // Se guardan los datos en la memoria EEPROM:
      for( int i=0 ; i<sizeof(DATOS) ; i++  )
      EEPROM.write( ADDRESS+i , miMemoria.b[i] );
      initValues();
   }
 


    pinMode(BUZZER_PIN, OUTPUT);  // Buzzer pin
    // Initializes the LCD and RTC instances
    pinMode(P1ARRIBA, INPUT);
    pinMode(P1ABAJO, INPUT);
    pinMode(SUBEP1,OUTPUT);
    //digitalWrite(SUBEP1,ON);
    pinMode(BAJAP1, OUTPUT);

    pinMode(P2ARRIBA, INPUT);
    pinMode(P2ABAJO, INPUT);
    pinMode(SUBEP2 , OUTPUT);
    pinMode(BAJAP2, OUTPUT);

    lcd.begin(16, 2);
    Wire.begin();
    RTC.begin();

    state = SHOW_TIME;  // Initial state of the FSM
 
    // Uncomment this to set the current time on the RTC module
    // RTC.adjust(DateTime(__DATE__, __TIME__));
    Serial.begin(9600);

    pinMode(ledPin, OUTPUT);

    // Configure myThread
    myThread.onRun(lcdControl);
    myThread.setInterval(200);

    // Configure hisThread
    hisThread.onRun(boringCallback);
    hisThread.setInterval(250);
    
    // Configure blinkLedThread
    blinkLedThread.onRun(blinkLed);
    blinkLedThread.setInterval(100);

    // Adds myThread to the controll
    controll.add(&myThread);

    // Adds hisThread and blinkLedThread to groupOfThreads
    groupOfThreads.add(&hisThread);
    groupOfThreads.add(&blinkLedThread);

    // Add groupOfThreads to controll
    controll.add(&groupOfThreads);
    
}

void loop(){
    // run ThreadController
    // this will check every thread inside ThreadController,
    // if it should run. If yes, he will run it;
    controll.run();

    // Rest of code
    float h = 3.1415;
    h/=2;
}
