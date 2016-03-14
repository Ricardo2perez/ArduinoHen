/*
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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

void setup() {
    lcd.createChar(0, newChar);
    lcd.begin(16, 2);
    lcd.write(0);
}

void loop() {}
*/
// Displays the current open time and transitions back to show 
// date and time after 2 sec (+ 1 sec delay from inside the loop function)
// e.g. open Time HOUR: 08 MIN: 20