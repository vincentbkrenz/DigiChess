#ifndef BOARD_H
#define BOARD_H

#include "gantry.h"
#include "electromagnet.h"
#include "ChessEngine.h"
#include <Arduino.h> 

class Board {
    public:

        enum MOVE_TYPE {
            AVOID,
            STRAIGHT,
            DIAGONAL,
            L_SHAPE,
            CASTLE,
            EN_PASSANT
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

        //updates_board 
        void update_board(String move);
        void update_board(String move, MOVE_TYPE moveType);

        // Move a piece from (fromX, fromY) to (toX, toY)
        void movePiece(String move, MOVE_TYPE moveType);

        // Reset the board to the initial chess position
        void reset();

        void move_half_square(HALF_SQUARE_DIRECTION direction);

        void move_full_square();

        private:
        char squares[8][8] = {
            {'r','n','k','b','q','k','n','r'},
            {'p','p','p','p','p','p','p','p'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.'},
            {'P','P','P','P','P','P','P','P'},
            {'R','N','K','B','Q','K','N','R'}
        };
        int squareSize = 50; //Size of each square in steps
        Gantry gantry; // Gantry for moving pieces
        Electromagnet electromagnet; // Electromagnet for picking up pieces
        ChessEngine engine;

};


#endif // BOARD_H