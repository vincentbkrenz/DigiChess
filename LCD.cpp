#include "LCD.h"
#include <Arduino.h>
#include <Wire.h>
#include <SerLCD.h>


void LCD::begin() {
    Wire.begin();
    lcd.begin(Wire);
    lcd.setBacklight(255,255,255);
    lcd.setContrast(5);
    lcd.clear();
    lcd.print("Digichess LCD");
}

void LCD::clear() {
    // Clear the LCD display
    lcd.clear();
}

void LCD::print(String str) {
    // Print a string to the LCD
    lcd.clear();
    delay(5);
    lcd.setCursor(0, 0);
    lcd.print(str);
    Serial.println(str);
}

void LCD::setCursor(int col, int row) {
    // Set cursor position
    lcd.setCursor(col, row);
    Serial.print("Cursor set to: ");
    Serial.print(col);
    Serial.print(", ");
    Serial.println(row);
}