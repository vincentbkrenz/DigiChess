#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <SerLCD.h>


class LCD {
    public:
    // Initialize the LCD
    void begin();

    // Clear the display
    void clear();

    // Print a string to the LCD
    void print(String str);

    // Print a centered string to the LCD
    void printCentered(String text, int row, int lcdWidth = 20);

    // Set cursor position
    void setCursor(int col, int row);
    
    private:
    SerLCD lcd; 
};

#endif
