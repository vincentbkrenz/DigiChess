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
const int LEFT_BUTTON_PIN = 15;
const int RIGHT_BUTTON_PIN = 14;


const int maxX = 2500; // Outer limits (in steps)
const int maxY = 2500; // Outer limits (in steps)
const int homingX = 2200;
const int minX = -500;   //Inner limits (in steps)
const int minY = -500;   //Inner limits (in steps)

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

const int squareSize = 230; // Size of each square in steps
const int y_borderSize = 115; // Size of the border around the board in steps
const int x_borderSize = 220;

// If you're getting bored of computer vs computer games with the chess engine, here are some famous games
const String games[][46] = {
    {"e2e4", "e7e5", "f2f4", "e5f4", "f1c4", "d8h4", "e1f1", "b7b5", "c4b5", "g8f6",
    "g1f3", "h4h6", "d2d3", "f6h5", "f3h4", "h6g5", "h4f5", "c7c6", "g2g4", "h5f6",
    "h1g1", "c6b5", "h2h4", "g5g6", "h4h5", "g6g5", "d1f3", "f6g8", "c1f4", "g5f6",
    "b1c3", "f8c5", "c3d5", "f6b2", "f4d6", "c5g1", "e4e5", "b2b1", "f1e2", "b8a6",
    "f5g7", "e8d8", "f3f6", "g8f6", "d6e7", "DONE"},

    {"d2d4", "e7e6", "g1f3", "f8e7", "b1c3", "g8f6", "e2e4", "e8g8",
    "e4e5", "f6e8", "h2h4", "d7d6", "f1d3", "d6e5", "d4e5", "b8c6",
    "d3h7", "g8h7", "f3g5", "h7g6", "h4h5", "g6f5", "d1f3", "f5e5",
    "c1f4", "e5d4", "f3d3", "d4c5", "d3b5", "c5d4", "e1c1", "DONE",
    "", "", "", "", "", "", "",
    "", "", "", "", "", "", ""},
    
    {"e2e4", "e7e5", "g1f3", "d7d6", "d2d4", "c8g4", "d4e5", "g4f3",
    "d1f3", "d6e5", "f1c4", "g8f6", "f3b3", "d8e7", "b1c3", "c7c6",
    "c1g5", "b7b5", "c3b5", "c6b5", "c4b5", "b8d7", "e1c1", "a8d8",
    "d1d7", "d8d7", "h1d1", "e7e6", "b5d7", "f6d7", "b3b8", "d7b8",
    "d1d8", "DONE", "", "", "",
    "", "", "", "", "", "", ""}
};

    
const String gameNames[3] = {"Immortal Game", "Q-Castle Mate", "Opera Game"};
const int MAX_GAMES = 3;

#endif // CONSTANTS_H