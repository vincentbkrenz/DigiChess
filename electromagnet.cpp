#include "electromagnet.h"

// Constructor: takes the output pin number
Electromagnet::Electromagnet(int pin)
    : pinNumber(pin), isOn(false) {
    pinMode(pinNumber, OUTPUT); // Set pin as output
    digitalWrite(pinNumber, LOW); // Ensure it's off initially
}

// Turns the electromagnet on
void Electromagnet::on() {
    isOn = true;
    digitalWrite(pinNumber, HIGH); // Set the pin high to turn on the electromagnet
}

// Turns the electromagnet off
void Electromagnet::off() {
    isOn = false;
    digitalWrite(pinNumber, LOW); // Set the pin low to turn off the electromagnet
}

// Returns the pin number
int Electromagnet::getPin() const {
    return pinNumber;
}

//returns the state of the electromagnet
bool Electromagnet::state() const {
    return isOn;
}
