#include "board.h"

Board board;

void setup() {
    board.get_gantry()->setCurrentPosition(500, 500); 
    delay(100);
    

}

void loop() 
{
 
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::TOP_LEFT); //wong: moves +y
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::TOP_RIGHT); //actually top left
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::BOTTOM_LEFT); //actually bottom right
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::BOTTOM_RIGHT); //doesnt work-> goes +x (left)
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::POSITIVE_X); //works moves left
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1); 
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::NEGATIVE_X); //works properly -> moves right
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::POSITIVE_Y); //works properly
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
  board.move_half_square(Board::HALF_SQUARE_DIRECTION::NEGATIVE_Y); //works properly
  delay(1);
  board.get_gantry()->moveTo(500, 500, Gantry::Movement::MOVE_STRAIGHT);
  delay(1);
}




