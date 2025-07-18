#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

const int STEP_PIN_A = 10;
const int DIR_PIN_A = 11;
const int STEP_PIN_B = 12; 
const int DIR_PIN_B = 13;
const int LIMIT_SWITCH_X_PIN = 2; // Pin for X limit switch
const int LIMIT_SWITCH_Y_PIN = 3; // Pin for Y limit switch
const int ELECTROMAGNET_PIN = 4; // Pin for the electromagnet


const int maxX = 2200; // Outer limits (in steps)
const int maxY = 2200; // Outer limits (in steps)
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

const char init_cells[4][9] = { //strings are size 9 due to the nullptr at the end
    "########", // left side
    "########", // right side
    "########", // top side
    "########"  // bottom side
};

const char final_cells[4][9] = { //strings are size 9 due to the nullptr at the end
    "pppppppp", //left side
    "PPPPPPPP", //right side
    "rnkbqknr", //top side
    "RNKBQKNR"  //bottom side
};

const int squareSize = 225; // Size of each square in steps
const int borderSize = 200; // Size of the border around the board in steps

#endif // CONSTANTS_H