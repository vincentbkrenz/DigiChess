#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

#define CALIBRATION true
#define SERIAL_DEBUG false

const int STEP_PIN_A = 10;
const int DIR_PIN_A = 11;
const int STEP_PIN_B = 12; 
const int DIR_PIN_B = 13;
const int MS1_PIN = 7;
const int MS2_PIN = 8;
const int LIMIT_SWITCH_X_PIN = 2; // Pin for X limit switch
const int LIMIT_SWITCH_Y_PIN = 3; // Pin for Y limit switch
const int ELECTROMAGNET_PIN = 6; // Pin for the electromagnet


const int maxX = 2345; // Outer limits (in steps)
const int maxY = 2345; // Outer limits (in steps)
const int homingX = 2200;
const int minX = 0;   //Inner limits (in steps)
const int minY = 0;   //Inner limits (in steps)

const char init_squares[8][8] = {
    {'r','n','b','q','k','b','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.'},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

const char init_cells[4][9] = { //strings are size 9 due to the nullptr at the end
    "########", // left side
    "########", // right side
    "########", // bottom side
    "########"  // top side
};

const char final_cells[4][9] = { //strings are size 9 due to the nullptr at the end
    "pppppppp", //left side
    "PPPPPPPP", //right side
    "rnbkqbnr", //bottom side
    "RNBKQBNR"  //top side
};

const int squareSize = 220; // Size of each square in steps
const int y_borderSize = 150; // Size of the border around the board in steps
const int x_borderSize = 275;

#endif // CONSTANTS_H