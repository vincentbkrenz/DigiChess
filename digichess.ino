
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

// ————— repetition tracking —————
#define MAX_HIST 16
static uint16_t posHist[MAX_HIST];
static uint8_t  histPtr = 0;

// simple rolling hash of the 8×8 portion of the 0x88 board
uint16_t hashBoard() {
  auto *b = board.get_engine()->b;
  uint16_t h = 0;
  for (uint8_t r = 0; r < 8; ++r) {
    for (uint8_t f = 0; f < 8; ++f) {
      h = h * 31 + b[r * 16 + f];
    }
  }
  return h;
}

// record this position, return true if it’s now the 3rd time we’ve seen it
bool recordAndCheckRepetition() {
  uint16_t h = hashBoard();
  uint8_t seen = 0;
  for (uint8_t i = 0; i < MAX_HIST; ++i) {
    if (posHist[i] == h && ++seen >= 2) {
      return true;
    }
  }
  posHist[histPtr] = h;
  histPtr = (histPtr + 1) % MAX_HIST;
  return false;
}
// ——————————————————————————————

void setup() {
  Serial.begin(9600);
  //while (!Serial) { delay(1); }
  Serial.println("Start");
  board.reset_wiggle();

  // one initial seed
  uint32_t noise = micros();
  board.get_engine()->setSeed(noise);

  // init repetition buffer
  memset(posHist, 0xFF, sizeof(posHist));
  histPtr = 0;
}

void loop() {
  bool gameOver = false;

  int turns = 0;
  while (!gameOver && turns < 15) {
    // ——— reseed before every half‑move ———
    uint32_t noise = micros();  // or analogRead(someFloatingPin)
    board.get_engine()->setSeed(noise);

    // choose depth: White→4, Black→3
    int depth = (board.get_engine()->k == 0x08) ? 4 : 4;

    // play one engine move
    gameOver = !(board.get_engine()->playComputerMove(depth));
    String move = board.get_engine()->printMoveAndBoard();
    board.movePiece(move, board.getMoveType(move));

    turns++;

    // repetition check
    if (!gameOver && recordAndCheckRepetition()) {
      Serial.println("Draw by repetition");
      gameOver = true;
    }
  }

  board.reset_board();

  // halt forever
  while (true) {
    delay(1000);
  }
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

  
  board.reset_board();

}



void loop() {
  
}


#endif // CHESS_DEBUG