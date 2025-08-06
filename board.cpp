#include "board.h"
#include "constants.h"
#include <Arduino.h>

Board::Board() : 
  gantry(STEP_PIN_A, DIR_PIN_A, STEP_PIN_B, DIR_PIN_B, LIMIT_SWITCH_X_PIN, LIMIT_SWITCH_Y_PIN, MS1_PIN, MS2_PIN), 
   electromagnet(ELECTROMAGNET_PIN), engine() {
    Serial.begin(9600);
    #if SERIAL_DEBUG
    while (!Serial) { 
        delay(1); 
    }
    delay(100);
    #endif
    Serial.println("Start");
    memcpy(squares, init_squares, sizeof(squares));
    memcpy(cells, init_cells, sizeof(cells));
    electromagnet.off();
    gantry.home();
    engine.setSeed(micros());
}

Board::Board(int cur_x, int cur_y) : 
  gantry(STEP_PIN_A, DIR_PIN_A, STEP_PIN_B, DIR_PIN_B, LIMIT_SWITCH_X_PIN, LIMIT_SWITCH_Y_PIN, MS1_PIN, MS2_PIN), 
   electromagnet(ELECTROMAGNET_PIN), engine() {
    memcpy(squares, init_squares, sizeof(squares));
    memcpy(cells, init_cells, sizeof(cells));
    electromagnet.off();
    engine.setSeed(micros());
    gantry.setCurrentPosition(cur_x, cur_y);
}

Board::MOVE_TYPE Board::getMoveType(String move) {

    int toFile = move.charAt(2) - 'a';
    int toRank = 7 - (move.charAt(3) - '1');
    int fromFile = move.charAt(0) - 'a';
    int fromRank = 7 - (move.charAt(1) - '1');

    int dF = abs(toFile - fromFile);
    int dR = abs(toRank - fromRank);

    Serial.print("Squares rank file   ");
    Serial.println(squares[fromRank][fromFile]);
    
    switch(squares[fromRank][fromFile]) {
        case 'P':
        case 'p':
            if ((dF == dR) && (dF == 1) && (dR == 1)) { //diagonal, but not promoted
                if (squares[toRank][toFile] != '.') { //normal capture
                    return(DIAGONAL);
                } else { //en passant
                    return(EN_PASSANT);
                }
            } else { //normal pawn move
                return(RECTANGULAR);
            }
            break;
            
        case 'K':
        case 'k':
            if ((dF == 2) || (dR == 2)) { //castle
                return(CASTLE);
            } //else -> just move normally
        default:
            if (dF == dR) { //along a diagonal
                return(DIAGONAL);
            } else if (dF == 0 || dR == 0) { //straight
                return(RECTANGULAR);
            } else { //L-shaped, avoid other pieces
                return(AVOID);
            } 
            break;
    }

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

    printState();

}

void Board::movePiece(String move, MOVE_TYPE moveType) {

    electromagnet.off();

    int toFile = move.charAt(2) - 'a';
    int toRank = 7 - (move.charAt(3) - '1');
    int fromFile = move.charAt(0) - 'a';
    int fromRank = 7 - (move.charAt(1) - '1');

    if (moveType == CASTLE) {
        movePiece(move, STRAIGHT); //king is moved to proper space
        delay(500);
        String rook_move = "";
        if (toFile < 4) { // queenside castle
            rook_move += (char)('a' + toFile - 2);
            rook_move += (char)('1' + (7 - fromRank));
            rook_move += (char)('a' + toFile + 1);
            rook_move += (char)('1' + (7 - toRank));
        } else { // kingside castle
            rook_move += (char)('a' + toFile + 1);
            rook_move += (char)('1' + (7 - fromRank));
            rook_move += (char)('a' + toFile - 1);
            rook_move += (char)('1' + (7 - toRank));
        }
        Serial.println("rook move");
        Serial.println(rook_move);

        movePiece(rook_move, AVOID);
        delay(500);
    } else if (moveType == EN_PASSANT) {
         if ((toRank - fromRank) < 0) { // bottom player move
            capturePiece(toFile, toRank + 1); //captures piece below
            squares[toRank + 1][toFile] = '.'; // Clear the captured pawns square
        } else { //top player move
            capturePiece(toFile, toRank - 1); //captures piece above
            squares[toRank - 1][toFile] = '.'; //Clear the captured pawns square
        }
        delay(500);
        electromagnet.off();
        moveToSquare(fromFile, fromRank, RECTANGULAR);
        delay(500);

    } else { //default
        // Check if a piece is occupying the destination square
        if (squares[toRank][toFile] != '.') {
            // Handle capture logic here
            capturePiece(toFile, toRank);
        }
        // Move the gantry to the destination square
        moveToSquare(fromFile, fromRank, RECTANGULAR);
        delay(500);
    }

    //turn on the electromagnet to pick up the piece
    ////////////////////////////////////////////////////////////////
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
        case EN_PASSANT:
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
        default:
            break;
    }

    electromagnet.off(); // Turn off the electromagnet after moving

    // Update the board
    if (moveType != CASTLE) {
        updateBoard(move, moveType);
    }

    
} 

#ifndef wiggle
#define wiggle false
#endif

void Board::reset_wiggle() {
    #if wiggle

    electromagnet.off();
    for (int j = 0; j < 2; j++) {
        if (j == 0) {
            for (int i = 0; i < 8; i++) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                int x = gantry.getX();
                int y = gantry.getY();
                gantry.moveTo(x + 30, y + 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x - 30, y - 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x, y, Gantry::Movement::MOVE_RECTANGULAR);
                delay(600);
                electromagnet.off();
            }
        } else { // j == 1
            for (int i = 7; i >= 0; i--) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                int x = gantry.getX();
                int y = gantry.getY();
                gantry.moveTo(x + 30, y + 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x - 30, y - 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x, y, Gantry::Movement::MOVE_RECTANGULAR);
                delay(600);
                electromagnet.off();
            }
        }
    }
    for (int j = 6; j < 8; j++) {
        if (j == 6) {
            for (int i = 0; i < 8; i++) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                int x = gantry.getX();
                int y = gantry.getY();
                gantry.moveTo(x + 30, y + 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x - 30, y - 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x, y, Gantry::Movement::MOVE_RECTANGULAR);
                delay(600);
                electromagnet.off();
            }
        } else { //j == 7
            for (int i = 7; i >= 0; i--) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                int x = gantry.getX();
                int y = gantry.getY();
                gantry.moveTo(x + 30, y + 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x - 30, y - 30, Gantry::Movement::MOVE_RECTANGULAR);
                gantry.moveTo(x, y, Gantry::Movement::MOVE_RECTANGULAR);
                delay(600);
                electromagnet.off();
            }
        }
    }

    #else
    electromagnet.off();
    for (int j = 6; j < 8; j++) {
        if (j == 7) {
            for (int i = 0; i < 8; i++) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                move_half_square(NEGATIVE_Y);
                move_half_square(NEGATIVE_Y);
                delay(200);
                move_half_square(POSITIVE_Y);
                move_half_square(POSITIVE_Y);
                delay(500);
                electromagnet.off();
            }
        } else { // j == 6
            for (int i = 7; i >= 0; i--) {
                moveToSquare(i, j, RECTANGULAR);
                 electromagnet.on();
                move_half_square(POSITIVE_Y);
                move_half_square(POSITIVE_Y);
                delay(200);
                move_half_square(NEGATIVE_Y);
                move_half_square(NEGATIVE_Y);
                delay(500);
                electromagnet.off();
            }
        }
    }
    for (int j = 0; j < 2; j++) { 
        if (j == 0) {
            for (int i = 0; i < 8; i++) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                move_half_square(POSITIVE_Y);
                move_half_square(POSITIVE_Y);
                delay(200);
                move_half_square(NEGATIVE_Y);
                move_half_square(NEGATIVE_Y);
                delay(500);
                electromagnet.off();
            }
        } else { //j == 1
            for (int i = 7; i >= 0; i--) {
                moveToSquare(i, j, RECTANGULAR);
                electromagnet.on();
                move_half_square(NEGATIVE_Y);
                move_half_square(NEGATIVE_Y);
                delay(200);
                move_half_square(POSITIVE_Y);
                move_half_square(POSITIVE_Y);
                delay(500);
                electromagnet.off();
            }
        }
    }

    #endif //#if wiggle
    
}

void Board::reset_board() {

    //capture all pieces blocking the reset
    for (char i = 'a'; i <= 'h'; i++) {
        for (char j = '1'; j <= '8'; j++){
            int file = i - 'a';
            int rank = 7 - (j - '1');
            if ((squares[rank][file] != init_squares[rank][file])
                    && squares[rank][file] != '.'
                        && init_squares[rank][file] != '.') {
                    capturePiece(file, rank);
                    squares[rank][file] = '.';
                    Serial.print("Capturing:  ");
                    Serial.print(i);
                    Serial.println(j);
                }
        }
    }

    //return the pieces that are still on the board
    for (char i = 'a'; i <= 'h'; i++) {
        for (char j = '1'; j <= '8'; j++) {
            int file = i - 'a';
            int rank = 7 - (j - '1');
            if ((squares[rank][file] != init_squares[rank][file])
                    && squares[rank][file] != '.'
                        && init_squares[rank][file] == '.') {
                    return_piece(squares[rank][file], file, rank);
                }
        }
    }

    //return the pieces that are captured

    for (int side = 0; side < 4; side++) {
        for (int cell = 0; cell < 8; cell++) {
            if (cells[side][cell] != '#') {
                return_captured_piece(side, cell);
            }
        }
    }
}

void Board::return_piece(char symbol, int from_file, int from_rank) {
    for (int i = 0; i < 8; i++) { //file
        for (int j = 0; j < 8; j++) { //rank
            if (init_squares[j][i] == symbol && squares[j][i] == '.') {
                electromagnet.off();
                moveToSquare(from_file, from_rank, RECTANGULAR);
                electromagnet.on();
                moveToSquare(i, j, AVOID, from_file, from_rank);
                delay(500);
                squares[j][i] = symbol;
                squares[from_rank][from_file] = '.';
                return;
            }
        }
    }
}

void Board::return_captured_piece(int side, int cell) {
    char symbol = cells[side][cell];
    int adj_file, adj_rank;
    electromagnet.off();
    //move to adjacent square
    switch (side) {
        case 0: // left side
            adj_file = 0;
            adj_rank = cell;
            break;
        case 1: // right side
            adj_file = 7;
            adj_rank = cell;
            break;
        case 2: // top side
            adj_file = cell;
            adj_rank = 7;
            break;
        case 3: // bottom side
            adj_file = cell;
            adj_rank = 0;
            break;
        default:
            break;
    }
    moveToSquare(adj_file, adj_rank, RECTANGULAR);
    //move onto square
    switch (side) {
        case 0: //left side
            move_half_square(POSITIVE_X);
            move_half_square(POSITIVE_X);
            break;
        case 1: //right side
            move_half_square(NEGATIVE_X);
            move_half_square(NEGATIVE_X);
            break;
        case 2: //bottom side
            move_half_square(NEGATIVE_Y);
            move_half_square(NEGATIVE_Y);
            break;
        case 3: //top side
            move_half_square(POSITIVE_Y);
            move_half_square(POSITIVE_Y);

    }
    //move piece to correct square
    for (int i = 0; i < 8; i++) { //file
        for (int j = 0; j < 8; j++) { //rank
            if (init_squares[j][i] == symbol && squares[j][i] == '.') {
                electromagnet.on();
                delay(500);
                switch(side) {
                    case 0: //left side
                        moveToSquare(i, j, AVOID, adj_file-1, adj_rank);
                        break;
                    case 1: //right side
                        moveToSquare(i, j, AVOID, adj_file+1, adj_rank);
                        break;
                    case 2: //bottom side
                        moveToSquare(i, j, AVOID, adj_file, adj_rank+1);
                        break;
                    case 3: //top side
                        moveToSquare(i, j, AVOID, adj_file, adj_rank-1);
                        break;
                    default:
                        break;
                }
                squares[j][i] = symbol;
                cells[side][cell] = '#';
                return;
            }
        }
    }
}


void Board::moveToSquare(int file, int rank, MOVE_TYPE moveType, int fromFile, int fromRank) {

    if ((moveType == AVOID) || (moveType == CAPTURE)) {

        if ((fromFile == -2) || (fromRank == -2)) {
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
            delay(500);
        }
        else { //moveType == CAPTURE (continue to avoid pieces, no recenter)
            //placeholder
        }
        
    }
    else {
        // Calculate the target position in steps
        int targetX = (7 - file) * _squareSize + _x_borderSize + (_squareSize / 2); 
        int targetY = (7 - rank) * _squareSize + _y_borderSize + (_squareSize / 2); 

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

    //move the gantry to the piece being captured
    electromagnet.off();
    moveToSquare(file, rank, RECTANGULAR);
    delay(500);
    
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
    moveToSquare(adj_file, adj_rank, CAPTURE, file, rank);
    delay(500);

    int targetX = (7 - adj_file) * _squareSize + _x_borderSize + (_squareSize / 2); 
    int targetY = (7 - adj_rank) * _squareSize + _y_borderSize + (_squareSize / 2); 

    switch (side) {
        case 0: // left side
            move_half_square(POSITIVE_X);
            delay(500);
            if (gantry.getY() < targetY ) {
                move_half_square(POSITIVE_Y);
            } else {
                move_half_square(NEGATIVE_Y);
            }
            break;
        case 1: // right side
            move_half_square(NEGATIVE_X);
            delay(500);
            if (gantry.getY() < targetY) {
                move_half_square(POSITIVE_Y);
            } else {
                move_half_square(NEGATIVE_Y);
            }
            break;
        case 2: // bottom side
            move_half_square(NEGATIVE_Y);
            delay(500);
            if (gantry.getX() < targetX) {
                move_half_square(POSITIVE_X);
            } else {
                move_half_square(NEGATIVE_X);
            }
            break;
        case 3: // top side
            move_half_square(POSITIVE_Y);
            delay(500);
            if (gantry.getX() < targetX) {
                move_half_square(POSITIVE_X);
            } else {
                move_half_square(NEGATIVE_X);
            }
            break;
        default:
            break;
    }
    delay(500);

    // // Re-center the piece on the cell
    // if (side % 2 ==0) { // left or right side
    //     if (adj_file < 4) {
    //         move_half_square(TOP_RIGHT); 
    //     } else {
    //         move_half_square(TOP_LEFT); 
    //     }
    // } else { // top or bottom side
    //     if (adj_rank < 4) {
    //         move_half_square(BOTTOM_LEFT); 
    //     } else {
    //         move_half_square(BOTTOM_RIGHT); 
    //     }
    // }
    // if (adj_file < 4 && adj_rank < 4) {
    //     move_half_square(BOTTOM_RIGHT); 
    // } else if (adj_file >= 4 && adj_rank < 4) {
    //     move_half_square(BOTTOM_LEFT); 
    // } else if (adj_file < 4 && adj_rank >= 4) {
    //     move_half_square(TOP_RIGHT); 
    // } else {
    //     move_half_square(TOP_LEFT); 
    // }
    delay(500);
    electromagnet.off();

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



