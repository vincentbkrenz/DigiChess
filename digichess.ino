
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

  // for (int i = 0; i < 8; i++)
  // {
  //   for(int j = 0; j < 8; j++) {
  //   board.get_electromagnet()->on();
  //   board.moveToSquare(i,j, Board::MOVE_TYPE::RECTANGULAR);
  //   delay(500);
  //   }
  // }

  // board.get_electromagnet()->on();
  // board.get_gantry()->moveTo(0, 500, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(1000);
  // for (int i = 0; i < maxX; i += 10)
  // {
  //   board.get_gantry()->moveTo(i, 500, Gantry::Movement::MOVE_RECTANGULAR);
  //   delay(500);
  //   Serial.println(i);
  // }
    // board.get_electromagnet()->on();
    // board.moveToSquare(0, 0, Board::MOVE_TYPE::RECTANGULAR);
    // delay(500);
    // board.moveToSquare(7, 7, Board::MOVE_TYPE::DIAGONAL);

  // for (int i = maxX; i >= 0; i -= 100) {
    
  //     board.get_gantry()->moveTo(i, 0);
  //     Serial.println(i);
  //     delay(1200);
  // }
  // board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(500);
  // board.get_gantry()->moveTo(maxX, maxY, Gantry::Movement::MOVE_DIAGONAL);
  // delay(500);
  
  // board.get_electromagnet()->on(); 
  // delay(1000);
  // board.get_electromagnet()->off(); 
  // delay(1000);

  // board.get_electromagnet()->on();
  // board.moveToSquare(4, 4, Board::MOVE_TYPE::RECTANGULAR);
  // delay(1000);
  // board.moveToSquare(7, 0, Board::MOVE_TYPE::AVOID, 4, 4);


  // board.movePiece("e2e4", Board::MOVE_TYPE::AVOID);
  // delay(1000);
  // board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_RECTANGULAR);
  // delay(1000);

  board.get_gantry()->moveTo(0, 0);
  board.get_gantry()->moveTo(maxX, 0);
  board.get_gantry()->moveTo(maxX, maxY);
  board.get_gantry()->moveTo(0, maxY);

}

// // void loop() {
// //     board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_RECTANGULAR);
// //     delay(500);
// //     board.get_gantry()->moveTo(1900, 0, Gantry::Movement::MOVE_RECTANGULAR);
// //     delay(5000);
// // }

void loop() {
  
}


#endif // CHESS_DEBUG