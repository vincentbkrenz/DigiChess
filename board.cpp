#include "board.h"
#include "constants.h"
#include <Arduino.h>

Board::Board() : 
  gantry(STEP_PIN_A, DIR_PIN_A, STEP_PIN_B, DIR_PIN_B, LIMIT_SWITCH_X_PIN, LIMIT_SWITCH_Y_PIN), 
   electromagnet(ELECTROMAGNET_PIN), engine() {
    memcpy(squares, init_squares, sizeof(squares));
    gantry.home();
    engine.setSeed(micros());
}

void Board::updateBoard(String move) {
    // move format is always 4 characters, e.g., "e2e4"
    int fromFile = move.charAt(0) - 'a';
    int fromRank = move.charAt(1) - '1';
    int toFile = move.charAt(2) - 'a';
    int toRank = move.charAt(3) - '1';

    // Update the board: move the piece and clear the source square
    squares[toRank][toFile] = squares[fromRank][fromFile];
    squares[fromRank][fromFile] = '.'; // Clear the source square
}

void Board::updateBoard(String move, MOVE_TYPE moveType) {
    //placeholder
    updateBoard(move);
}

void Board::movePiece(String move, MOVE_TYPE moveType) {

    electromagnet.off();

    int toFile = move.charAt(2) - 'a';
    int toRank = move.charAt(3) - '1';
    int fromFile = move.charAt(0) - 'a';
    int fromRank = move.charAt(1) - '1';

    // Check if a piece is occupying the destination square
    if (squares[toRank][toFile] != '.') {
        // Handle capture logic here
    }


    // Move the gantry to the destination square
    moveToSquare(fromFile, fromRank, STRAIGHT);

    //turn on the electromagnet to pick up the piece
    electromagnet.on();

    switch (moveType) {
        case AVOID:

            // Divide the board into 4 sections and use diagonal half-square movements
            if (toFile < 4 && toRank < 4) {
                move_half_square(TOP_LEFT); // Top-left quadrant
            } else if (toFile >= 4 && toRank < 4) {
                move_half_square(TOP_RIGHT); // Top-right quadrant
            } else if (toFile < 4 && toRank >= 4) {
                move_half_square(BOTTOM_LEFT); // Bottom-left quadrant
            } else {
                move_half_square(BOTTOM_RIGHT); // Bottom-right quadrant
            }
            
            gantry.moveTo(
                gantry.getX() + (toFile - fromFile) * _squareSize,
                gantry.getY() + (toRank - fromRank) * _squareSize,
                Gantry::Movement::MOVE_RECTANGULAR
            );

            // Re-center the piece on the square
            if (toFile < 4 && toRank < 4) {
                move_half_square(BOTTOM_RIGHT); 
            } else if (toFile >= 4 && toRank < 4) {
                move_half_square(BOTTOM_LEFT); 
            } else if (toFile < 4 && toRank >= 4) {
                move_half_square(TOP_RIGHT); 
            } else {
                move_half_square(TOP_LEFT); 
            }

            break;

        case STRAIGHT:
            moveToSquare(toFile, fromRank, STRAIGHT);
            break;
        case DIAGONAL:
            moveToSquare(toFile, fromRank, DIAGONAL);
            break;
        case L_SHAPE:
            moveToSquare(toFile, fromRank, L_SHAPE);
            break;
        case CASTLE:
            break;
        case EN_PASSANT:
            break;
        default:
            break;
    }

    electromagnet.off(); // Turn off the electromagnet after moving

    // Update the board
    updateBoard(move, moveType);
} 

void Board::reset() {

}

void Board::moveToSquare(int file, int rank, MOVE_TYPE moveType) {
    // Calculate the target position in steps
    int targetX = file * _squareSize + _borderSize + (_squareSize / 2); 
    int targetY = (7 - rank) * _squareSize + _borderSize + (_squareSize / 2); 

    switch (moveType) {
        case STRAIGHT:
            gantry.moveTo(targetX, targetY, Gantry::Movement::MOVE_STRAIGHT);
            break;
        case DIAGONAL:
            gantry.moveTo(targetX, targetY, Gantry::Movement::MOVE_DIAGONAL);
            break;
        case L_SHAPE:
            gantry.moveTo(targetX, targetY, Gantry::Movement::MOVE_RECTANGULAR);
            break;
        default:
            break;
    }
}

void Board::move_half_square(HALF_SQUARE_DIRECTION direction) {
    switch (direction) {
        case POSITIVE_X:
            gantry.moveTo(
                gantry.getX() + (_squareSize / 2),
                gantry.getY(),
                Gantry::Movement::MOVE_STRAIGHT
            );
            break;
        case NEGATIVE_X:
            gantry.moveTo(
                gantry.getX() - (_squareSize / 2),
                gantry.getY(),
                Gantry::Movement::MOVE_STRAIGHT
            );
            break;
        case POSITIVE_Y:
            gantry.moveTo(
                gantry.getX(),
                gantry.getY() + (_squareSize / 2),
                Gantry::Movement::MOVE_STRAIGHT
            );
            break;
        case NEGATIVE_Y:
            gantry.moveTo(
                gantry.getX(),
                gantry.getY() - (_squareSize / 2),
                Gantry::Movement::MOVE_STRAIGHT
            );
            break;
        case TOP_LEFT:
            gantry.moveTo(
                gantry.getX() + (_squareSize / 2),
                gantry.getY() + (_squareSize / 2),
                Gantry::Movement::MOVE_DIAGONAL
            );
            break;
        case TOP_RIGHT:
            gantry.moveTo(
                gantry.getX() - (_squareSize / 2),
                gantry.getY() + (_squareSize / 2),
                Gantry::Movement::MOVE_DIAGONAL
            );
            break;
        case BOTTOM_LEFT:
            gantry.moveTo(
                gantry.getX() + (_squareSize / 2),
                gantry.getY() - (_squareSize / 2),
                Gantry::Movement::MOVE_DIAGONAL
            );
            break;
        case BOTTOM_RIGHT:
            gantry.moveTo(
                gantry.getX() - (_squareSize / 2),
                gantry.getY() - (_squareSize / 2),
                Gantry::Movement::MOVE_DIAGONAL
            );
            break;
        default:
            break;
    }
}



