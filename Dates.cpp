
#include "Dates.h"

/*Dates::Dates(): {
    LCDKeypad lcd;

}*/

void Dates::showTime(bool open, bool luxOpen, bool puntos)
{
	_open = open;
	_luxOpen = luxOpen;
	
	
	
	 _now = RTC.now();
    const char* dayName[] = { "DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB" };
    const char* monthName[] = { "ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DEC" };
    
    lcd.clear();
    

   lcd.print(String(dayName[_now.dayOfWeek()]) + " " +
              (_now.day() < 10 ? "0" : "") + _now.day() + " " +
              monthName[_now.month()-1] + " " + _now.year());
    lcd.setCursor(0,1);
    lcd.print((_now.hour() < 10 ? "0" : "") + String(_now.hour()) +(puntos? ":":" ") +
              (_now.minute() < 10 ? "0" : "") + _now.minute() + 
              //":" +
              //(now.second() < 10 ? "0" : "") + now.second() +
               (_open ? "  open" : "")+ (_luxOpen ? "  lux":""));

}

int Dates::setTimeYear(int* tmpYear)
{
  
 unsigned long timeRef;
    bool _timeOut = true;
    _now = RTC.now();
    *tmpYear = _now.year();
    lcd.clear();
    lcd.print("Time Year");

    

    _timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set Year: ");
    lcd.setCursor(11,1);
    lcd.print(*tmpYear);
    
    
    while ( (unsigned long)(millis() - _timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *tmpYear = *tmpYear < 2022 ? *tmpYear + 1 : *tmpYear;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            lcd.print(*tmpYear);
            _timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *tmpYear = *tmpYear > 2014 ? *tmpYear - 1 :*tmpYear;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            lcd.print(*tmpYear);
            _timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd.button() != KEYPAD_NONE );
            _timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !_timeOut ) return(KEYPAD_SELECT);
    else return(TIME_OUT);
}

int Dates::setTimeMont(uint8_t* tmpMonth)
{
    unsigned long timeRef;
    boolean timeOut = true;
    _now = RTC.now();
    *tmpMonth = _now.month();
    lcd.clear();
    lcd.print("Time Month:");

    

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set Month: ");
    lcd.setCursor(11,1);
    lcd.print(*tmpMonth);
   

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *tmpMonth = *tmpMonth < 12? *tmpMonth + 1 : *tmpMonth;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            lcd.print(*tmpMonth);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *tmpMonth = *tmpMonth > 1 ? *tmpMonth - 1 : *tmpMonth;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            lcd.print(*tmpMonth);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd.button() != KEYPAD_NONE );
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut ) return(KEYPAD_SELECT);
    else return(TIME_OUT);

}

int Dates:: setTimeDay(uint8_t* tmpDay)
{
     unsigned long timeRef;
    boolean timeOut = true;
    _now = RTC.now();
    *tmpDay = _now.day();
    lcd.clear();
    lcd.print("Time Day:");

    

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set Day: ");
    lcd.setCursor(11,1);
    lcd.print(*tmpDay);
   

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *tmpDay = *tmpDay < 31 ? *tmpDay + 1 : *tmpDay;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            lcd.print(*tmpDay);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *tmpDay = *tmpDay > 1 ? *tmpDay - 1 : *tmpDay;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            lcd.print(*tmpDay);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd.button() != KEYPAD_NONE );
            timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !timeOut ) return(KEYPAD_SELECT);
    else return(TIME_OUT);


}

int Dates::setTimeHours(uint8_t* tmpHours)
{
    unsigned long timeRef;
    boolean timeOut = true;
    _now = RTC.now();
    
    lcd.clear();
    lcd.print("Time Hour");

    *tmpHours = _now.hour();

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set hours: ");
    lcd.setCursor(11,1);
    if ( *tmpHours < 10 ) lcd.print(" ");
    lcd.print(*tmpHours);

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *tmpHours = *tmpHours < 23 ? *tmpHours + 1 : *tmpHours;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( *tmpHours < 10 ) lcd.print(" ");
            lcd.print(*tmpHours);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *tmpHours = *tmpHours > 0 ? *tmpHours - 1 : *tmpHours;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( *tmpHours < 10 ) lcd.print(" ");
            lcd.print(*tmpHours);
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

    if ( !timeOut ) return(KEYPAD_SELECT);
    else return(TIME_OUT);

}

int Dates::setTimeMinutes(uint8_t* tmpMinutes)
{
    unsigned long timeRef;
    boolean timeOut = true;
   
    _now = RTC.now();
    
    *tmpMinutes = _now.minute();
    lcd.clear();
    lcd.print("Time Minutes");

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set minutes: ");
    lcd.setCursor(13,1);
    if ( *tmpMinutes < 10 ) lcd.print(" ");
    lcd.print (*tmpMinutes);
    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();
        
        if ( button == KEYPAD_UP )
        {
            *tmpMinutes = *tmpMinutes < 59 ? *tmpMinutes + 1 : *tmpMinutes;
            lcd.setCursor(13,1);
            lcd.print("  ");
            lcd.setCursor(13,1);
            if ( *tmpMinutes < 10 ) lcd.print(" ");
            lcd.print(*tmpMinutes);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN )
        {
            *tmpMinutes = *tmpMinutes > 0 ? *tmpMinutes - 1 : *tmpMinutes;
            lcd.setCursor(13,1);
            lcd.print("  ");
            lcd.setCursor(13,1);
            if ( *tmpMinutes < 10 ) lcd.print(" ");
            lcd.print(*tmpMinutes);
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
        
       return(KEYPAD_SELECT);
    }
    else return(TIME_OUT);
}

int Dates::showOpenTime(uint8_t* openHours, uint8_t* openMinutes)
{
    
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t pushed = 0;
    timeRef = millis();

    lcd.clear();
    lcd.print("open Time");
    lcd.setCursor(0,1);
    lcd.print(String("HOUR: ") + ( *openHours < 9 ? "0" : "" ) + *openHours + 
                   " MIN: " + ( *openMinutes < 9 ? "0" : "" ) + *openMinutes);

     while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_RIGHT ) // Increase 1 hour
        {
           while ( lcd.button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_RIGHT;
            break;
        }
       
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
           
            while ( lcd.button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_SELECT;
            break;
           
        }
        delay(100);
    }

    if ( !timeOut ){
         return(pushed);

        }
    else return(TIME_OUT);

   
}

int Dates::showCloseTime(uint8_t* closeHours, uint8_t* closeMinutes)
{

   
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t pushed = 0;
    timeRef = millis();

    lcd.clear();
    lcd.print("Close Time");
    lcd.setCursor(0,1);
    lcd.print(String("HOUR: ") + ( *closeHours < 9 ? "0" : "" ) + *closeHours + 
                   " MIN: " + ( *closeMinutes < 9 ? "0" : "" ) + *closeMinutes);
      while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_RIGHT ) // Increase 1 hour
        {
           while ( lcd.button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_RIGHT;
            break;
        }
       
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
           
            while ( lcd.button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_SELECT;
            break;
           
        }
        delay(100);
    }

    if ( !timeOut ){
         return(pushed);

        }
    else return(TIME_OUT);

}

int Dates::setLuxOpen(uint8_t* levelLuxOpen)
{
   
    unsigned long timeRef;
    boolean timeOut = true;
    
    
    lcd.clear();
    lcd.print("Lux Open");

   

    timeRef = millis();
    lcd.setCursor(0,1);
    lcd.print("Set Lux: ");
    lcd.setCursor(11,1);
    if ( *levelLuxOpen < 10 ) lcd.print(" ");
    lcd.print(*levelLuxOpen);

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *levelLuxOpen = *levelLuxOpen < 500 ? *levelLuxOpen + 1 : *levelLuxOpen;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( *levelLuxOpen < 10 ) lcd.print(" ");
            lcd.print(*levelLuxOpen);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *levelLuxOpen = *levelLuxOpen > 0 ? *levelLuxOpen - 1 : *levelLuxOpen;
            lcd.setCursor(11,1);
            lcd.print("  ");
            lcd.setCursor(11,1);
            if ( *levelLuxOpen < 10 ) lcd.print(" ");
            lcd.print(*levelLuxOpen);
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

     return(KEYPAD_SELECT);}
    else return(TIME_OUT);
}