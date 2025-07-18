#include "board.h"
#include "constants.h"
#include <Arduino.h>

Board::Board() : 
  gantry(STEP_PIN_A, DIR_PIN_A, STEP_PIN_B, DIR_PIN_B, LIMIT_SWITCH_X_PIN, LIMIT_SWITCH_Y_PIN), 
   electromagnet(ELECTROMAGNET_PIN), engine() {
    memcpy(squares, init_squares, sizeof(squares));
    memcpy(cells, init_cells, sizeof(cells));
    gantry.home();
    engine.setSeed(micros());
}

void Board::updateBoard(String move) {
    
    //placeholder
    updateBoard(move, NULL_MOVE);

}

void Board::updateBoard(String move, MOVE_TYPE moveType) {


    int toFile = move.charAt(2) - 'a';
    int toRank = 7 - (move.charAt(3) - '1');
    int fromFile = move.charAt(0) - 'a';
    int fromRank = 7 - (move.charAt(1) - '1');

    // Update the board: move the piece and clear the source square
    squares[toRank][toFile] = squares[fromRank][fromFile];
    squares[fromRank][fromFile] = '.'; // Clear the source square

}

void Board::movePiece(String move, MOVE_TYPE moveType) {

    electromagnet.off();

    int toFile = move.charAt(2) - 'a';
    int toRank = 7 - (move.charAt(3) - '1');
    int fromFile = move.charAt(0) - 'a';
    int fromRank = 7 - (move.charAt(1) - '1');


    // Check if a piece is occupying the destination square
    if (squares[toRank][toFile] != '.') {
        // Handle capture logic here
        capturePiece(toFile, toRank);
    }

    // Move the gantry to the destination square
    moveToSquare(fromFile, fromRank, RECTANGULAR);
    delay(500);

    //turn on the electromagnet to pick up the piece
    electromagnet.on();

    switch (moveType) {
        case AVOID:
            delay(500);
            moveToSquare(toFile, toRank, AVOID, fromFile, fromRank);
            delay(500);
            break;

        case STRAIGHT:
            delay(500);
            moveToSquare(toFile, toRank, STRAIGHT);
            delay(500);
            break;
        case DIAGONAL:
            delay(500);
            moveToSquare(toFile, toRank, DIAGONAL);
            delay(500);
            break;
        case L_SHAPE:
        case RECTANGULAR:
            delay(500);
            moveToSquare(toFile, toRank, L_SHAPE);
            delay(500);
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

void Board::moveToSquare(int file, int rank, MOVE_TYPE moveType, int fromFile, int fromRank) {

    if ((moveType == AVOID) || (moveType == CAPTURE)) {

        if ((fromFile == -1) || (fromRank == -1)) {
            fromFile = file;
            fromRank = rank;
        }

         // Divide the board into 4 sections and use diagonal half-square movements
        if (file < 4 && rank < 4) {
            move_half_square(TOP_LEFT); // Top-left quadrant
        } else if (file >= 4 && rank < 4) {
            move_half_square(TOP_RIGHT); // Top-right quadrant
        } else if (file < 4 && rank >= 4) {
            move_half_square(BOTTOM_LEFT); // Bottom-left quadrant
        } else {
            move_half_square(BOTTOM_RIGHT); // Bottom-right quadrant
        }

        delay(500);
        
        gantry.moveTo(
            gantry.getX() + (-file + fromFile) * _squareSize,
            gantry.getY() + (-rank + fromRank) * _squareSize,
            Gantry::Movement::MOVE_RECTANGULAR
        );

        delay(500);

        if (moveType == AVOID) {
            // Re-center the piece on the square
            if (file < 4 && rank < 4) {
                move_half_square(BOTTOM_RIGHT); 
            } else if (file >= 4 && rank < 4) {
                move_half_square(BOTTOM_LEFT); 
            } else if (file < 4 && rank >= 4) {
                move_half_square(TOP_RIGHT); 
            } else {
                move_half_square(TOP_LEFT); 
            }
        }
        else { //moveType == CAPTURE (continue to avoid pieces, no recenter)
            //placeholder
        }
        
    }
    else {
        // Calculate the target position in steps
        int targetX = (7 - file) * _squareSize + _borderSize + (_squareSize / 2); 
        int targetY = (7 - rank) * _squareSize + _borderSize + (_squareSize / 2); 

        switch (moveType) {
            case STRAIGHT:
                gantry.moveTo(targetX, targetY, Gantry::Movement::MOVE_STRAIGHT);
                break;
            case DIAGONAL:
                gantry.moveTo(targetX, targetY, Gantry::Movement::MOVE_DIAGONAL);
                break;
            case L_SHAPE:
            case RECTANGULAR:
                gantry.moveTo(targetX, targetY, Gantry::Movement::MOVE_RECTANGULAR);
                break;
            default:
                break;
        }
    }
}

void Board::capturePiece(int file, int rank) {

    char sym = squares[rank][file];

    //find first availiable cell to hold captured piece, then update cell
    int side, pos;
    bool found = false;
    for (int i = 0; (i < 4) && (!found); i++) {
        for (int j = 0; (j < 8) && (!found); j++) {
            if ((final_cells[i][j] == sym) && (cells[i][j] == '#')) {
                cells[i][j] = sym;
                side = i;
                pos = j;
                found = true;
            }
        }
    }

    Serial.print(side);
    Serial.print(" ");
    Serial.println(pos);

    //move the gantry to the piece being captured
    electromagnet.off();
    moveToSquare(file, rank, RECTANGULAR);
    Serial.println("MOVED TO CAPTURED PIECE");
    delay(2000);
    
    //move captured piece to square adjacent to cell
    int adj_file, adj_rank;
    switch (side) {
        case 0: // left side
            adj_file = 0;
            adj_rank = pos;
            break;
        case 1: // right side
            adj_file = 7;
            adj_rank = pos;
            break;
        case 2: // top side
            adj_file = pos;
            adj_rank = 7;
            break;
        case 3: // bottom side
            adj_file = pos;
            adj_rank = 0;
            break;
        default:
            break;
    }
    electromagnet.on();
    Serial.print(adj_file);
    Serial.print(" ");
    Serial.print(adj_rank);
    Serial.print("  ");
    Serial.print(file);
    Serial.print(" ");
    Serial.println(rank);
    moveToSquare(adj_file, adj_rank, CAPTURE, file, rank);
    Serial.println("MOVED TO ADJACENT CELL");
    delay(2000);

    //shift the piece onto adjacent cell
    switch (side) {
        case 0: // left side
            move_half_square(POSITIVE_X);
            move_half_square(POSITIVE_X);
            break;
        case 1: // right side
            move_half_square(NEGATIVE_X);
            move_half_square(NEGATIVE_X);
            break;
        case 2: // top side
            move_half_square(NEGATIVE_Y);
            move_half_square(NEGATIVE_Y);
            break;
        case 3: // bottom side
            move_half_square(POSITIVE_Y);
            move_half_square(POSITIVE_Y);
            break;
        default:
            break;
    }
    Serial.println("SHIFTED CAPTURED PIECE");
    delay(2000);

    // Re-center the piece on the cell
    if (adj_file < 4 && adj_rank < 4) {
        move_half_square(BOTTOM_RIGHT); 
    } else if (adj_file >= 4 && adj_rank < 4) {
        move_half_square(BOTTOM_LEFT); 
    } else if (adj_file < 4 && adj_rank >= 4) {
        move_half_square(TOP_RIGHT); 
    } else {
        move_half_square(TOP_LEFT); 
    }
    electromagnet.off();
    Serial.println("RE-CENTERED CAPTURED PIECE");
    delay(2000);

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

void Board::printState() {
    Serial.println();  // Add a blank line for readability

    // Print top column labels
    Serial.print("   ");  // Align left edge
    for (char col = 'a'; col <= 'h'; col++) {
        Serial.print(" ");
        Serial.print(col);
    }
    Serial.println();

    // Print each row from 8 (top) down to 1 (bottom)
    for (int row = 0; row < 8; row++) {
        Serial.print(" ");
        Serial.print(row + 1);  // Row labels 8 to 1
        Serial.print(" ");      // Left border

        for (int col = 0; col < 8; col++) {
            Serial.print("|");
            Serial.print(squares[row][col]);
        }
        Serial.println("|");  // Right border
    }

    // Print bottom border (just column letters again for reference)
    Serial.print("   ");
    for (char col = 'a'; col <= 'h'; col++) {
        Serial.print(" ");
        Serial.print(col);
    }
    Serial.println();
}



