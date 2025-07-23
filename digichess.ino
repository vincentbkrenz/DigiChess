
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

  delay(500);
  //engine.waitForEnter();
}


#else //(!CHESS_DEBUG)


#include "board.h"


void run_loop(Board& board);

void setup() {

  
  Serial.begin(9600);
  //while(!Serial);
  delay(500);
  Serial.println("start");
  Board board;
    
  //board.get_gantry()->setCurrentPosition(0, 0); 
  delay(1000);
  //board.get_electromagnet()->on(); 

  board.reset_wiggle();

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

  // board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(2000);
  // board.get_gantry()->moveTo(maxX, 0, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(2000);
  // board.get_gantry()->moveTo(maxX, maxY, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(2000);
  // board.get_gantry()->moveTo(0, maxY, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(2000);
  // for (int i = 0; i < maxX; i += 10) {
  //   board.get_gantry()->moveTo(i, 500, Gantry::Movement::MOVE_RECTANGULAR);
  //   Serial.println(i);
  //   delay(300);
  // }
 
board.movePiece("c2c4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("d7d5", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("b2b3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("c8e6", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d2d3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("b8c6", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("b1c3", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("g8f6", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("c1f4", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("f6h5", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("f4e3", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d5d4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g2g4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("h5f6", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("g4g5", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f6g4", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("e3d2", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d4c3", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d2c3", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("h7h6", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f2f3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g4e3", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d1c1", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("e3f1", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("e1f1", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("a7a5", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("a1b1", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("c6d4", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("b3b4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("h6g5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("c1g5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("a5b4", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("b1b4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("c7c6", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f3f4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g7g6", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g5e5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d4c2", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("e5h8", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("c2b4", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("c3b4", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("a8a2", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g1f3", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("b7b5", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f3d4", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("a2a1", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("b4e1", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d8a5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("h8h4", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("e6d7", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("c4b5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("c6b5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("h1g1", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f8g7", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d4f3", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("g7f6", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("h4h6", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("d7f5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("g1h1", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f5g4", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("f1f2", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("a5a7", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("e2e3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("a1a3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f3e5", Board::MOVE_TYPE::AVOID);
delay(800);
board.movePiece("f6e5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("f4e5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("a3d3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("e1b4", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("a7a2", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f2g3", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("a2b2", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("h6h8", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("e8d7", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("e5e6", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g4e6", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("h8b2", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d3e3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("g3f4", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("e3b3", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("b2d2", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("d7e8", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("b4c5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("f7f5", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("h1e1", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("e8f7", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d2e2", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("e6d5", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("e2e7", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("f7g8", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("e7d8", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("g8h7", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("d8d7", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("h7h6", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("c5f8", Board::MOVE_TYPE::DIAGONAL);
delay(800);
board.movePiece("h6h5", Board::MOVE_TYPE::RECTANGULAR);
delay(800);
board.movePiece("d7h7", Board::MOVE_TYPE::RECTANGULAR);
delay(800);

while(true);



   
  
  

}



void loop() {
  
}


#endif // CHESS_DEBUG