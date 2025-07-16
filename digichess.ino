
#define CHESS_DEBUG false

#if CHESS_DEBUG

#define private public  // hack to expose private members
#include "ChessEngine.h"
#undef private

ChessEngine engine;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  delay(100);  // allow USB serial to initialize
  engine.setSeed(micros());
}

void loop() {
  // Reseed each move for variability
  engine.setSeed(micros());

  // Choose search depth based on side to move: White (k==0x08) => 4, Black => 3
  int depth = (engine.k == 0x08) ? 4 : 3;
  engine.playComputerMove(depth);
  engine.printMoveAndBoard();

  // Debug: print last 5 moves array
  Serial.println("Last 5 moves:");
  int count = engine.histCount;
  int start = count > 5 ? count - 5 : 0;
  for (int i = start; i < count; ++i) {
    Serial.print(i);
    Serial.print(": ");
    for (int j = 0; j < 4; ++j) Serial.print(engine.hist[i][j]);
    Serial.println();
  }

  delay(500);
  engine.waitForEnter();
}


#else //(!CHESS_DEBUG)


#include "board.h"

Board board;

void setup() {
    
  Serial.begin(9600);
  while(!Serial);
  delay(500);
  Serial.println("start");
  
  board.get_gantry()->setCurrentPosition(0, 0); 
  delay(2000);
  //board.get_electromagnet()->on(); 
    

}


// // void loop() 
// // {
 
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::TOP_LEFT); //wong: moves +y
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::TOP_RIGHT); //actually top left
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::BOTTOM_LEFT); //actually bottom right
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::BOTTOM_RIGHT); //doesnt work-> goes +x (left)
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::POSITIVE_X); //works moves left
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500); 
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::NEGATIVE_X); //works properly -> moves right
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::POSITIVE_Y); //works properly
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// //   board.move_half_square(Board::HALF_SQUARE_DIRECTION::NEGATIVE_Y); //works properly
// //   delay(500);
// //   board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_STRAIGHT);
// //   delay(500);
// // }

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

void loop() {
  delay(2000);
  board.movePiece("e2e4", Board::MOVE_TYPE::AVOID);
  board.printState();
  delay(2000);
  board.moveToSquare(7, 7, Board::MOVE_TYPE::RECTANGULAR);
  delay(2000);
  board.movePiece("b1c3", Board::MOVE_TYPE::L_SHAPE);
  board.printState();
  delay(2000);
  board.movePiece("g1a7", Board::MOVE_TYPE::DIAGONAL);
  board.printState();
  delay(2000);
  board.moveToSquare(7, 7, Board::MOVE_TYPE::RECTANGULAR);
  
  // board.get_electromagnet()->on(); 
  // delay(1000);
  // board.get_electromagnet()->off(); 
  // delay(1000);
}

// // void loop() {
// //     board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_RECTANGULAR);
// //     delay(500);
// //     board.get_gantry()->moveTo(1900, 0, Gantry::Movement::MOVE_RECTANGULAR);
// //     delay(5000);
// // }


#endif // CHESS_DEBUG