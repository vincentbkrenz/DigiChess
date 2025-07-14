#ifndef ELECTROMAGNET_H
#define ELECTROMAGNET_H

#include <Arduino.h> // Add this if using Arduino

class Electromagnet {
public:
    // Constructor: takes the output pin number
    Electromagnet(int pin);

    // Turns the electromagnet on
    void on();

    // Turns the electromagnet off
    void off();

    // Returns the pin number
    int getPin() const;

    // Returns the state of the electromagnet
    bool state() const;

private:
    int pinNumber;
    bool isOn;
};

#endif // ELECTROMAGNET_H