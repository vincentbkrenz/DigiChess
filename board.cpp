#include "board.h"

Board::Board() {
    gantry.home();
    engine.setSeed(micros());
    updateBoard();
}

void Board::updateBoard(String move) {
    // move format is always 4 characters, e.g., "e2e4"
    int fromFile = move.charAt(0) - 'a';
    int fromRank = move.charAt(1) - '1';
    int toFile = move.charAt(2) - 'a';
    int toRank = move.charAt(3) - '1';

    // Update the board: move the piece and clear the source square
    board[toRank][toFile] = board[fromRank][fromFile];
    board[fromRank][fromFile] = '.'; // Clear the source square
}

void Board::updateBoard(String move, MOVE_TYPE moveType) {
    //placeholder
    updateBoard(move);
}

void Board::movePiece(String move, MOVE_TYPE moveType) {
    
} 

void Board::reset() {

}

void Board::move_half_square(HALF_SQUARE_DIRECTION direction) {
    switch direction {
        case POSITIVE_X:
            gantry.moveTo(gantry.getX() + squareSize, gantry.getY());
            break;
        case NEGATIVE_X:
            gantry.moveTo(gantry.getX() - squareSize, gantry.getY());
            break;
        case POSITIVE_Y:
            gantry.moveTo(gantry.getX(), gantry.getY() + squareSize);
            break;
        case NEGATIVE_Y:
            gantry.moveTo(gantry.getX(), gantry.getY() - squareSize);
            break;
        case TOP_LEFT:
            gantry.moveTo(gantry.getX() - squareSize, gantry.getY() + squareSize);
            break;
        case TOP_RIGHT:
            gantry.moveTo(gantry.getX() + squareSize, gantry.getY() + squareSize);
            break;
        case BOTTOM_LEFT:
            gantry.moveTo(gantry.getX() - squareSize, gantry.getY() - squareSize);
            break;
        case BOTTOM_RIGHT:
            gantry.moveTo(gantry.getX() + squareSize, gantry.getY() - squareSize);
            break;
    }
}


