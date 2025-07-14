#ifndef MOTOR_H
#define MOTOR_H


#ifdef ARDUINO
#include <Arduino.h>
#endif



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
        void moveTo(long xSteps, long ySteps, Movement mode = MOVE_STRAIGHT);
        void home();
        void setCurrentPosition(long xSteps, long ySteps);
        long getX() const;
        long getY() const;

        // Getters and setters for current position
        void setX(long xSteps);
        void setY(long ySteps);

        void setMaxX(long maxX);
        void setMaxY(long maxY);
        long getMaxX() const;
        long getMaxY() const;

        // Getters and setters for limit switch pins
        int getLimitSwitchXPin() const;
        int getLimitSwitchYPin() const;
        void setLimitSwitchXPin(int pin);
        void setLimitSwitchYPin(int pin);

    private:
        int _stepPinA, _dirPinA;
        int _stepPinB, _dirPinB;
        int _limitSwitchXPin, _limitSwitchYPin;
        long _currentX, _currentY;
        long _maxX = 200;
        long _maxY = 200; // Outer limits (in steps)

        void stepMotorA(bool direction);
        void stepMotorB(bool direction);
        void moveCoreXY(long deltaX, long deltaY);
        void singleStep(int motor, bool direction);
        void calculateCoreXY(long x, long y, long& aSteps, long& bSteps);

};

#endif