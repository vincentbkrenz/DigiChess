#include "gantry.h"

// Constructor: Initializes motor and limit switch pins, sets initial position to (0,0) and default outer limits
Gantry::Gantry(int stepPinA, int dirPinA, int stepPinB, int dirPinB, int limitSwitchXPin, int limitSwitchYPin)
    : _stepPinA(stepPinA), _dirPinA(dirPinA), _stepPinB(stepPinB), _dirPinB(dirPinB),
      _limitSwitchXPin(limitSwitchXPin), _limitSwitchYPin(limitSwitchYPin), _currentX(0), _currentY(0) {
    pinMode(_stepPinA, OUTPUT);
    pinMode(_dirPinA, OUTPUT);
    pinMode(_stepPinB, OUTPUT);
    pinMode(_dirPinB, OUTPUT);
    pinMode(_limitSwitchXPin, INPUT_PULLUP);
    pinMode(_limitSwitchYPin, INPUT_PULLUP);
}

// Moves the gantry to the specified X/Y position using CoreXY kinematics and movement mode
void Gantry::moveTo(int xSteps, int ySteps, Movement mode) {
    // Clamp target position to within limits
    if (xSteps < _minX) xSteps = 0;
    if (ySteps < _minY) ySteps = 0;
    if (xSteps > _maxX) xSteps = _maxX;
    if (ySteps > _maxY) ySteps = _maxY;

    int deltaX = xSteps - _currentX;
    int deltaY = ySteps - _currentY;
    if (mode == MOVE_RECTANGULAR) {
        moveCoreXY(deltaX, 0);
        moveCoreXY(0, deltaY);
    } else if (mode == MOVE_DIAGONAL) {
        if (abs(deltaX) == abs(deltaY) && deltaX != 0) {
            int steps = abs(deltaX);
            bool xDir = deltaX > 0;
            bool yDir = deltaY > 0;
            for (int i = 0; i < steps; ++i) {
                moveCoreXY(xDir ? 1 : -1, yDir ? 1 : -1);
            }
        } else {
            moveCoreXY(deltaX, deltaY);
        }
    } else {
        moveCoreXY(deltaX, deltaY);
    }
    _currentX = xSteps;
    _currentY = ySteps;
}

// Moves the gantry by the specified deltaX and deltaY using direct stepper logic
void Gantry::moveCoreXY(int deltaX, int deltaY) {
    // CoreXY: for pure X, both motors same direction; for pure Y, both motors opposite directions
    int aSteps = deltaX + deltaY;
    int bSteps = deltaX - deltaY;
    int aAbs = abs(aSteps);
    int bAbs = abs(bSteps);
    bool aDir = (aSteps >= 0) ? LOW : HIGH;
    bool bDir = (bSteps >= 0) ? LOW : HIGH;

    // Special handling for pure X or Y moves
    if (deltaY == 0 && deltaX != 0) {
        // Pure X: both motors same direction, same steps
        aAbs = abs(deltaX);
        bAbs = abs(deltaX);
        aDir = (deltaX >= 0) ? LOW : HIGH;
        bDir = (deltaX >= 0) ? LOW : HIGH;
    } else if (deltaX == 0 && deltaY != 0) {
        // Pure Y: both motors opposite directions, same steps
        aAbs = abs(deltaY);
        bAbs = abs(deltaY);
        aDir = (deltaY >= 0) ? LOW : HIGH;
        bDir = (deltaY >= 0) ? HIGH : LOW;
    }

    digitalWrite(_dirPinA, aDir);
    digitalWrite(_dirPinB, bDir);
    int maxSteps = (aAbs > bAbs) ? aAbs : bAbs;
    for (int i = 0; i < maxSteps; ++i) {
        if (i < aAbs) digitalWrite(_stepPinA, HIGH);
        if (i < bAbs) digitalWrite(_stepPinB, HIGH);
        delay(1);
        if (i < aAbs) digitalWrite(_stepPinA, LOW);
        if (i < bAbs) digitalWrite(_stepPinB, LOW);
        delay(1);
    }
}

void Gantry::home() {
    //Move in +X direction until X limit switch is pressed
    while (digitalRead(_limitSwitchXPin) == LOW) {
        stepMotorA(true); // Move +X 
        stepMotorB(true);
    }

    delay(500);

    // Move in -Y direction until Y limit switch is pressed
    while (digitalRead(_limitSwitchYPin) == LOW) {
        stepMotorA(false);  // Move -Y (A backward, B forward)
        stepMotorB(true);
    }

    delay(500);
    
    //Move a bit
    setCurrentPosition(50, 0);
    moveTo(0, 50);

    setCurrentPosition(maxX, 0); //HOMED POSITION

}

void Gantry::setCurrentPosition(int xSteps, int ySteps) {
    _currentX = xSteps;
    _currentY = ySteps;
}

int Gantry::getX() const { return _currentX; }
int Gantry::getY() const { return _currentY; }
void Gantry::setX(int xSteps) { _currentX = xSteps; }
void Gantry::setY(int ySteps) { _currentY = ySteps; }
void Gantry::setMaxX(int maxX) { _maxX = maxX; }
void Gantry::setMaxY(int maxY) { _maxY = maxY; }
int Gantry::getMaxX() const { return _maxX; }
int Gantry::getMaxY() const { return _maxY; }
int Gantry::getLimitSwitchXPin() const { return _limitSwitchXPin; }
int Gantry::getLimitSwitchYPin() const { return _limitSwitchYPin; }
void Gantry::setLimitSwitchXPin(int pin) { _limitSwitchXPin = pin; pinMode(_limitSwitchXPin, INPUT_PULLUP); }
void Gantry::setLimitSwitchYPin(int pin) { _limitSwitchYPin = pin; pinMode(_limitSwitchYPin, INPUT_PULLUP); }


void Gantry::stepMotorA(bool direction) {
    digitalWrite(_dirPinA, direction ? LOW : HIGH);
    digitalWrite(_stepPinA, HIGH);
    delay(1);
    digitalWrite(_stepPinA, LOW);
    delay(1);
}
void Gantry::stepMotorB(bool direction) {
    digitalWrite(_dirPinB, direction ? LOW : HIGH);
    digitalWrite(_stepPinB, HIGH);
    delay(1);
    digitalWrite(_stepPinB, LOW);
    delay(1);
}
void Gantry::singleStep(int motor, bool direction) {
    if (motor == 0) stepMotorA(direction);
    else if (motor == 1) stepMotorB(direction);
}
void Gantry::calculateCoreXY(int x, int y, int& aSteps, int& bSteps) {
    aSteps = x + y;
    bSteps = x - y;
}
