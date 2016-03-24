
#include "Dates.h"

/*Dates::Dates(): {
    LCDKeypad lcd;

}*/

void Dates::showTime(bool open, bool luxOpen, bool puntos, LCDKeypad* lcd2)
{
	_open = open;
	_luxOpen = luxOpen;
	
	
	
	 _now = RTC.now();
    const char* dayName[] = { "DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB" };
    const char* monthName[] = { "ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DEC" };
    
    lcd2->clear();
    

   lcd2->print(String(dayName[_now.dayOfWeek()]) + " " +
              (_now.day() < 10 ? "0" : "") + _now.day() + " " +
              monthName[_now.month()-1] + " " + _now.year());
    lcd2->setCursor(0,1);
    lcd2->print((_now.hour() < 10 ? "0" : "") + String(_now.hour()) +(puntos? ":":" ") +
              (_now.minute() < 10 ? "0" : "") + _now.minute() + 
              //":" +
              //(now.second() < 10 ? "0" : "") + now.second() +
               (_open ? "  open" : "")+ (_luxOpen ? "  lux":""));

}

int Dates::setTime(int* tmpDate, LCDKeypad* lcd2, char* texto, char* texto2, int minimo, int maximo)
{
  
 unsigned long timeRef;
    bool _timeOut = true;
    lcd2->clear();
    lcd2->print(texto);

    

    _timeRef = millis();
    lcd2->setCursor(0,1);
    lcd2->print(texto2);
    lcd2->setCursor(11,1);
    lcd2->print(*tmpDate);
    
    
    while ( (unsigned long)(millis() - _timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *tmpDate = *tmpDate < maximo ? *tmpDate + 1 : *tmpDate;
            lcd2->setCursor(11,1);
            lcd2->print("  ");
            lcd2->setCursor(11,1);
            lcd2->print(*tmpDate);
            _timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *tmpDate = *tmpDate > minimo ? *tmpDate - 1 :*tmpDate;
            lcd2->setCursor(11,1);
            lcd2->print("  ");
            lcd2->setCursor(11,1);
            lcd2->print(*tmpDate);
            _timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd2->button() != KEYPAD_NONE );
            _timeOut = false;
            break;
        }
        delay(150);
    }

    if ( !_timeOut ) return(KEYPAD_SELECT);
    else return(TIME_OUT);
}




int Dates::showTime(uint8_t* Hours, uint8_t* Minutes, LCDKeypad* lcd2, char* texto)
{
    
    unsigned long timeRef;
    boolean timeOut = true;
    uint8_t pushed = 0;
    timeRef = millis();

    lcd2->clear();
    lcd2->print(texto);
    lcd2->setCursor(0,1);
    lcd2->print(String("HOUR: ") + ( *Hours < 9 ? "0" : "" ) + *Hours + 
                   " MIN: " + ( *Minutes < 9 ? "0" : "" ) + *Minutes);

     while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd2->button();

        if ( button == KEYPAD_RIGHT ) // Increase 1 hour
        {
           while ( lcd2->button() != KEYPAD_NONE ) 
            timeOut = false;
            pushed = KEYPAD_RIGHT;
            break;
        }
       
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
           
            while ( lcd2->button() != KEYPAD_NONE ) 
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



int Dates::setLux(int* levelLux, LCDKeypad* lcd2, char* texto, char* texto2, int luxMin, int luxMax)
{
   
    unsigned long timeRef;
    boolean timeOut = true;
    
    
    lcd2->clear();
    lcd2->print(texto);

   

    timeRef = millis();
    lcd2->setCursor(0,1);
    lcd2->print(texto2);
    lcd2->setCursor(11,1);
    if ( *levelLux < 10 ) lcd.print(" ");
    lcd2->print(*levelLux);

    
    while ( (unsigned long)(millis() - timeRef) < 5000 )
    {
        uint8_t button = lcd.button();

        if ( button == KEYPAD_UP ) // Increase 1 hour
        {
            *levelLux = *levelLux < luxMax ? *levelLux + 1 : *levelLux;
            lcd2->setCursor(11,1);
            lcd2->print("  ");
            lcd2->setCursor(11,1);
            if ( *levelLux < 10 ) lcd.print(" ");
            lcd2->print(*levelLux);
            timeRef = millis();
        }
        else if ( button == KEYPAD_DOWN ) //Decrease 1 hour
        {
            *levelLux = *levelLux > luxMin ? *levelLux - 1 : *levelLux;
            lcd2->setCursor(11,1);
            lcd2->print("  ");
            lcd2->setCursor(11,1);
            if ( *levelLux < 10 ) lcd.print(" ");
            lcd2->print(*levelLux);
            timeRef = millis();
        }
        else if ( button == KEYPAD_SELECT ) //Save hour
        {
            while ( lcd2->button() != KEYPAD_NONE ) ;
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