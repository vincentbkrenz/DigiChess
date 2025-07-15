#ifndef CONSTANTS_H
#define CONSTANTS_H

const int STEP_PIN_A = 10;
const int DIR_PIN_A = 11;
const int STEP_PIN_B = 12; 
const int DIR_PIN_B = 13;
const int LIMIT_SWITCH_X_PIN = 2; // Pin for X limit switch
const int LIMIT_SWITCH_Y_PIN = 3; // Pin for Y limit switch
const int ELECTROMAGNET_PIN = 4; // Pin for the electromagnet


const int maxX = 200; // Outer limits (in steps)
const int maxY = 200; // Outer limits (in steps)
const int minX = 0;   //Inner limits (in steps)
const int minY = 0;   //Inner limits (in steps)

const char init_squares[8][8] = {
    {'r','n','k','b','q','k','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','K','B','Q','K','N','R'}
};

const int squareSize = 50; // Size of each square in steps
const int borderSize = 50; // Size of the border around the board in steps

#endif // CONSTANTS_H