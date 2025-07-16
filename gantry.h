#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "constants.h"


class Gantry {
    public:

            // Enum for movement modes
        enum Movement {
            MOVE_STRAIGHT = 0, // straight line (default, Bresenham-style)
            MOVE_RECTANGULAR = 1, // L-shaped (first X, then Y)
            MOVE_DIAGONAL = 2 // diagonal (only if |deltaX| == |deltaY|)
        };
        
        Gantry(int stepPinA, int dirPinA, int stepPinB, int dirPinB, int limitSwitchXPin, int limitSwitchYPin);

        // Updated moveTo with movement mode
        void moveTo(int xSteps, int ySteps, Movement mode = MOVE_STRAIGHT);
        void home();
        void setCurrentPosition(int xSteps, int ySteps);
        int getX() const;
        int getY() const;

        // Getters and setters for current position
        void setX(int xSteps);
        void setY(int ySteps);

        void setMaxX(int maxX);
        void setMaxY(int maxY);
        int getMaxX() const;
        int getMaxY() const;

        // Getters and setters for limit switch pins
        int getLimitSwitchXPin() const;
        int getLimitSwitchYPin() const;
        void setLimitSwitchXPin(int pin);
        void setLimitSwitchYPin(int pin);

    private:
        int _stepPinA, _dirPinA;
        int _stepPinB, _dirPinB;
        int _limitSwitchXPin, _limitSwitchYPin;
        int _currentX = 0;
        int _currentY = 0;
        int _maxX = maxX;
        int _maxY = maxY;
        int _minX = minX;
        int _minY = minY;


        void stepMotorA(bool direction);
        void stepMotorB(bool direction);
        void moveCoreXY(int deltaX, int deltaY);
        void singleStep(int motor, bool direction);
        void calculateCoreXY(int x, int y, int& aSteps, int& bSteps);

};

#endif