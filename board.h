#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "gantry.h"
#include "electromagnet.h"
#include "ChessEngine.h"
#include <Arduino.h> 

class Board {
    public:

        enum MOVE_TYPE {
            AVOID,
            CAPTURE,
            STRAIGHT,
            DIAGONAL,
            RECTANGULAR,
            L_SHAPE,
            CASTLE,
            EN_PASSANT,
            NULL_MOVE
        };

        enum HALF_SQUARE_DIRECTION {
            POSITIVE_X,
            NEGATIVE_X,
            POSITIVE_Y,
            NEGATIVE_Y,
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            BOTTOM_RIGHT
        };

        Board();

        //updates board, returns the position of taken piece if captured or "00" if not
        void updateBoard(String move);
        void updateBoard(String move, MOVE_TYPE moveType);

        // Move a piece from (fromX, fromY) to (toX, toY)
        void movePiece(String move, MOVE_TYPE moveType);

        // make sure each piece is centered on the square
        void reset_wiggle();

        void reset_board();

        void return_piece(char symbol, int from_file, int from_rank);
        
        void move_half_square(HALF_SQUARE_DIRECTION direction);

        void moveToSquare(int file, int rank, MOVE_TYPE moveType = STRAIGHT, int fromFile = -1, int fromRank = -1);

        void capturePiece(int file, int rank);

        void printState();

        MOVE_TYPE getMoveType(String move);

        Gantry* get_gantry() {return &gantry;};
        Electromagnet* get_electromagnet() {return &electromagnet;};
        ChessEngine* get_engine() {return &engine;};

        private:
            char squares[8][8]; // 2D array representing the chess board
            char cells[4][9]; //cells for captured pieces //strings are size 9 due to nullptr at end
            int _squareSize = squareSize; //Size of each square in steps
            int _y_borderSize = y_borderSize; //Size of the y border around the board in steps
            int _x_borderSize = x_borderSize;
            Gantry gantry; // Gantry for moving pieces
            Electromagnet electromagnet; // Electromagnet for picking up pieces
            ChessEngine engine;

};


#endif // BOARD_H