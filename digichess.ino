
#define CHESS_DEBUG true

#if CHESS_DEBUG

#define private public  // hack to expose private members
#include "ChessEngine.h"
#undef private

#include "board.h"
#include "gantry.h"
#include "constants.h"
#include "electromagnet.h"
#include <Arduino.h>

Board board;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  delay(100); 
  Serial.println("Start");
  board.get_engine()->setSeed(micros());
  //board.reset_wiggle();
}

void loop() {
  // Reseed each move for variability
  //engine.setSeed(micros());

  // Choose search depth based on side to move: White (k==0x08) => 4, Black => 3
  int depth = (board.get_engine()->k == 0x08) ? 3 : 4;
  bool gameOver = false;
  while (!gameOver) {
    gameOver = !(board.get_engine()->playComputerMove(depth));
    String move = board.get_engine()->printMoveAndBoard();
    //Board::MOVE_TYPE moveType = board.getMoveType(move);
    //board.movePiece(move, moveType);
    //delay(800);
  }

  // // Debug: print last 5 moves array
  // Serial.println("Last 5 moves:");
  // int count = engine.histCount;
  // int start = count > 5 ? count - 5 : 0;
  // for (int i = start; i < count; ++i) {
  //   Serial.print(i);
  //   Serial.print(": ");
  //   for (int j = 0; j < 4; ++j) Serial.print(engine.hist[i][j]);
  //   Serial.println();
  // }

  while(true);
  //engine.waitForEnter();
}


#else //(!CHESS_DEBUG)


#include "board.h"


void run_loop(Board& board);

void setup() {

  
  Serial.begin(9600);
 // while(!Serial);
  delay(500);
  Serial.println("start");
  Board board;
    
  //board.get_gantry()->setCurrentPosition(0, 0); 
  delay(1000);
  //board.get_electromagnet()->on(); 

  //board.reset_wiggle();

  while(true){
    run_loop(board);
  }
    

}



// // void loop() 
// // {
 
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::TOP_LEFT); //wong: moves +y
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::TOP_RIGHT); //actually top left
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::BOTTOM_LEFT); //actually bottom right
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::BOTTOM_RIGHT); //doesnt work-> goes +x (left)
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::POSITIVE_X); //works moves left
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500); 
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::NEGATIVE_X); //works properly -> moves right
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::POSITIVE_Y); //works properly
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
//   board.move_half_square(Board::HALF_SQUARE_DIRECTION::NEGATIVE_Y); //works properly
//   delay(500);
//   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
//   delay(500);
// }

// // void loop() {
// //   delay(2000);
// //   for (int i = 7; i >= 0; i--) {
// //     for (int j = 7; j >= 0; j--) {
// //       board.moveToSquare(i, j, Board::MOVE_TYPE::RECTANGULAR);
// //       delay(500);
// //     }
// //   }
// //     board.moveToSquare(7, 7, Board::MOVE_TYPE::RECTANGULAR);
// // }

void run_loop(Board& board) {

String moves[] = {
  "e2e4", "e7e5",
  "f2f4", "e5f4",
  "f1c4", "d8h4",
  "e1f1", "b7b5",
  "c4b5", "g8f6",
  "g1f3", "h4h6",
  "d2d3", "f6h5",
  "f3h4", "h5g3",
  "h2g3", "h6b6",
  "b1c3", "f8c5",
  "d3d4", "c5d4",
  "d1f3", "b6b2",
  "c1b2", "c8a6",
  "f1e1", "a6e2",
  "c3d5", "e2f3",
  "h1f1", "f3g4",
  "f1f4", "g4h3",
  "f4f7", "h3g1",
  "f7d7", "e8f8",
  "d7d8"
};

 for (int i = 0; i < 39; i++) {
    board.movePiece(moves[i], board.getMoveType(moves[i]));
 }

  
  while (true);

}



void loop() {
  
}


#endif // CHESS_DEBUG