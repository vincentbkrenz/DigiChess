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
    lcd.print(str);
    Serial.println(str);
}

void LCD::printCentered(String text, int row, int lcdWidth) {
  int textLength = text.length();
  int startCol = (lcdWidth - textLength) / 2;
  if (startCol < 0) startCol = 0; // Prevent negative positioning

  lcd.setCursor(startCol, row);
  lcd.print(text);
}

void LCD::setCursor(int col, int row) {
    // Set cursor position
    lcd.setCursor(col, row);
    Serial.print("Cursor set to: ");
    Serial.print(col);
    Serial.print(", ");
    Serial.println(row);
}