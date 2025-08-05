#define private public 
#include "ChessEngine.h"
#undef private

#include "board.h"
#include "gantry.h"
#include "constants.h"
#include "electromagnet.h"
#include "LCD.h"
#include <Arduino.h>

// ————— repetition tracking —————
#define MAX_HIST 16
static uint16_t posHist[MAX_HIST];
static uint8_t  histPtr = 0;

uint16_t hashBoard();
bool recordAndCheckRepetition();

Board* board = nullptr;
LCD*   lcde  = nullptr;

void setup() {
  board = new Board();
  lcde  = new LCD();
  lcde->begin();

  #if CALIBRATION
  board->reset_wiggle();
  #endif

  // one initial seed
  uint32_t noise = micros();
  board->get_engine()->setSeed(noise);

  // init repetition buffer
  memset(posHist, 0xFF, sizeof(posHist));
  histPtr = 0;
}

void loop() {
  bool gameOver = false;
  bool repetition = false;
  lcde->clear();
  lcde->printCentered("Digichess",       1);
  lcde->printCentered("Starting Game...", 2);

  int turns = 0;
  while (!gameOver) {
    // ——— reseed before every half-move ———
    uint32_t noise = micros();
    board->get_engine()->setSeed(noise);

    int depth = (board->get_engine()->k == 0x08) ? 4 : 4;

    // play one engine move
    gameOver = !(board->get_engine()->playComputerMove(depth));
    String move = board->get_engine()->printMoveAndBoard();

    lcde->clear();
    lcde->printCentered(move, 1);
    lcde->printCentered("Move: " + String(board->get_engine()->mn - 1), 2);
    lcde->printCentered((board->get_engine()->mn % 2) ? "Reds Turn" : "Blacks Turn", 3);

    board->movePiece(move, board->getMoveType(move));
    turns++;

    // repetition check
    if (!gameOver && recordAndCheckRepetition()) {
      lcde->clear();
      lcde->printCentered("Draw by repetition", 1);
      gameOver = true;
      repetition = true;
      break;
    }
  }

  if (!repetition) {
    lcd->clear();
    lcde->printCentered("Checkmate", 1);
  }
  lcde->printCentered("Restarting...", 2);
  delay(10000);

  // ——— tear down & rebuild for a fresh game ———
  // clear repetition history
  memset(posHist, 0xFF, sizeof(posHist));
  histPtr = 0;

  //save current position
  int cur_x = board->get_gantry()->getX();
  int cur_y = board->get_gantry()->getY();
  // destroy old board+engine and create fresh one with saved position
  delete board;
  board = new Board(cur_x, cur_y);

  // reseed for next game
  noise = micros();
  board->get_engine()->setSeed(noise);

}

// simple rolling hash of the 8×8 portion of the 0x88 board
uint16_t hashBoard() {
  auto *b = board->get_engine()->b;
  uint16_t h = 0;
  for (uint8_t r = 0; r < 8; ++r) {
    for (uint8_t f = 0; f < 8; ++f) {
      h = h * 31 + b[r * 16 + f];
    }
  }
  return h;
}

// record this position, return true if it's now the 3rd time we've seen it
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
