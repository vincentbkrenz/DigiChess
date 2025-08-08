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
void onLeftButton();
void onRightButton();

Board* board = nullptr;
LCD*   lcde  = nullptr;
bool activated = false;
bool paused = false;
bool manualReset = false;

void setup() {
  board = new Board();
  lcde  = new LCD();
  lcde->begin();

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(LEFT_BUTTON_PIN, onLeftButton, RISING);
  attachInterrupt(RIGHT_BUTTON_PIN, onRightButton, RISING);


  #if CALIBRATION
  lcde->clear();
  lcde->printCentered("DigiChess!", 0);
  lcde->printCentered("Vinny Krenz &", 1);
  lcde->printCentered("Andrew Kazour", 2);
  lcde->printCentered("<----  Calibrate", 3);
  while(digitalRead(LEFT_BUTTON_PIN)) {
    delay(5);
  }

  lcde->clear();
  lcde->printCentered("Calibrating...", 2);
  
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

  int index = -1;
  bool startMenu = true;
  paused = false;
  activated = false;
 
  while (startMenu) {
    if (index == -1) { //standard computer vs computer game
      lcde->clear();
      lcde->printCentered("Mode Select:", 0);
      lcde->printCentered("Computer Generated", 1);
      lcde->printCentered("        Next ------>", 2);
      lcde->printCentered("<--- Press to Select", 3);
      while(true) {
        if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
          index++;
          delay(250);
          break;
        }
        else if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
          startMenu = false;
          break;
        }
        delay(5);
      }
    }
    else {
      lcde->clear();
      lcde->printCentered("Mode Select:", 0);
      lcde->printCentered(gameNames[index], 1);
      lcde->printCentered("        Next ------>", 2);
      lcde->printCentered("<--- Press to Select", 3);
      while(true) {
        if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
          if (index == MAX_GAMES - 1) {
            index = -1;
          } else {
            index++;
          }
          delay(250);
          break;
        }
        else if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
          startMenu = false;
          break;
        }
        delay(5);
      }
    }
  }
  delay(2000);
  
  


  bool gameOver = false;
  bool repetition = false;
  String extra = "";
 

  int turns = 0;
  if (index == -1) {
    while (!gameOver) {

      activated = true;
      manualReset = false;
      paused = false;


      // ——— reseed before every half-move ———
      uint32_t noise = micros();
      board->get_engine()->setSeed(noise);

      int depth = (board->get_engine()->k == 0x08) ? 4 : 4;

      // play one engine move
      gameOver = !(board->get_engine()->playComputerMove(depth));
      String move = board->get_engine()->printMoveAndBoard();

      lcde->clear();
      lcde->printCentered("<----- Pause        ", 0);
      lcde->printCentered(move, 1);
      lcde->printCentered("Move: " + String(board->get_engine()->mn - 1), 2);
      lcde->printCentered((board->get_engine()->mn % 2) ? "Black's Turn" : "Red's Turn", 3);

      board->movePiece(move, board->getMoveType(move));
      turns++;

      // repetition check
      if (!gameOver && recordAndCheckRepetition()) {
        extra = "Draw by repetition";
        gameOver = true;
        repetition = true;
        break;
      }

      if (paused) {
        activated = false;
        lcde->clear();
        lcde->printCentered("Paused...", 0);
        lcde->printCentered("<----- Resume       ", 2);
        lcde->printCentered("       Reset ------>", 3);
        while(true) {
          if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
            delay(1000);
            activated = true;
            paused = false;
            break;
          } else if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
            delay(1000);
            activated = false;
            paused = false;
            manualReset = true;
            gameOver = true;
            break;
          }
        }
      }

    }
    
    if (!repetition) {
      extra = "Checkmate!";
    }
  } else { //preprogrammed game
    int move_number = 0;
    activated = true;
    manualReset = false;
    paused = false;
    gameOver = false;

    while(games[index][move_number] != "DONE" && !gameOver) {
      String move = games[index][move_number];
      lcde->clear();
      lcde->printCentered("<----- Pause        ", 0);
      lcde->printCentered(move, 1);
      lcde->printCentered("Move: " + String(move_number + 1), 2);
      lcde->printCentered((move_number % 2) ? "Red's Turn" : "Black's Turn", 3);
      board->movePiece(move, board->getMoveType(move));
      move_number++;

      if (paused) {
        activated = false;
        lcde->clear();
        lcde->printCentered("Paused...", 0);
        lcde->printCentered("<----- Resume       ", 2);
        lcde->printCentered("       Reset ------>", 3);
        while(true) {
          if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
            delay(1000);
            activated = true;
            paused = false;
            break;
          } else if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
            delay(1000);
            activated = false;
            paused = false;
            manualReset = true;
            gameOver = true;
            break;
          }
        }
      }

    }
    extra = "Checkmate!";
  }


  if (manualReset) {
    extra = "Reset";
  }
  lcde->clear();
  lcde->printCentered("Game OVER!", 0);
  lcde->printCentered(extra, 1);
  lcde->printCentered("<----  Restart", 3);
  while(digitalRead(LEFT_BUTTON_PIN)) {
    delay(5);
  }

  lcde->clear();
  lcde->printCentered("Resetting...", 2);
  board->reset_board();
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
  uint32_t noise = micros();
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

void onLeftButton() {
  if (activated) {
      paused = true;
  }
}

void onRightButton() {

}
