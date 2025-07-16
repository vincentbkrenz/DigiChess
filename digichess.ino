#include "board.h"

Board board;

void setup() {
    
  Serial.begin(9600);
  //while(!Serial);
  delay(500);
  Serial.println("start");
  board.get_gantry()->setCurrentPosition(0, 0); 
  delay(2000);
  //board.get_electromagnet()->on(); 
    

}


// void loop() 
// {
 
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

// void loop() {
//   delay(2000);
//   for (int i = 7; i >= 0; i--) {
//     for (int j = 7; j >= 0; j--) {
//       board.moveToSquare(i, j, Board::MOVE_TYPE::RECTANGULAR);
//       delay(500);
//     }
//   }
//     board.moveToSquare(7, 7, Board::MOVE_TYPE::RECTANGULAR);
// }

void loop() {
  delay(2000);
  board.movePiece("e2e4", Board::MOVE_TYPE::AVOID);
  delay(2000);
  board.moveToSquare(7, 7, Board::MOVE_TYPE::RECTANGULAR);
  delay(2000);
  board.movePiece("b1c3", Board::MOVE_TYPE::L_SHAPE);
  delay(2000);
  board.movePiece("g1a7", Board::MOVE_TYPE::DIAGONAL);
  delay(2000);
  board.moveToSquare(7, 7, Board::MOVE_TYPE::RECTANGULAR);
  // board.get_electromagnet()->on(); 
  // delay(1000);
  // board.get_electromagnet()->off(); 
  // delay(1000);
}

// void loop() {
//     board.get_gantry()->moveTo(0, 0, Gantry::Movement::MOVE_RECTANGULAR);
//     delay(500);
//     board.get_gantry()->moveTo(1900, 0, Gantry::Movement::MOVE_RECTANGULAR);
//     delay(5000);
// }
