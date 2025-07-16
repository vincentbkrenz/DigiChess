// #include "better_chess.h"


// void setup() {
//   Serial.begin(9600);
//   while (!Serial) ;
//   delay(500);

//   Serial.println("*** MicroMaxChess ***");
//   Serial.println("Press 'c' + ENTER for Computer vs Computer");
//   Serial.println("or any other key + ENTER for Human vs Computer:");
//   while (!Serial.available()) ;
//   char m = Serial.read();
//   // flush any extra chars (e.g. newline)
//   while (Serial.available()) Serial.read();
//   autoPlay = (m == 'c' || m == 'C');

//   if (autoPlay) {
//     Serial.println("Auto-play mode selected.");
//     Serial.print("Enter White search depth (e.g. 3): ");
//     whiteDepth = readNumber();
//     Serial.print("Enter Black search depth (e.g. 3): ");
//     blackDepth = readNumber();
//   }
//   else {
//     Serial.println("Human vs Computer mode.");
//     Serial.print("Enter Computer (Black) search depth (e.g. 3): ");
//     blackDepth = readNumber();
//   }

//   lastH[0] = 0;
//   inputString = "";
//   stringComplete = false;
//   serialBoard();
// }

// void loop() {
//   if (autoPlay) {
//     // -- White (computer) move --
//     playComputerMove(whiteDepth);
//     printMoveAndBoard();
//     Serial.print("Last move: ");
//     Serial.println(getLastMove());
//     waitForEnter();

//     // -- Black (computer) move --
//     playComputerMove(blackDepth);
//     printMoveAndBoard();
//     Serial.print("Last move: ");
//     Serial.println(getLastMove());
//     waitForEnter();

//     mn++;
//   }
//   else {
//     // -- get human move --
//     while (!stringComplete) {
//       getserialchar();
//     }
//     inputString.trim();
//     Serial.print(mn); Serial.print(". ");
//     Serial.print(inputString.substring(0, 4));
//     c[0] = inputString.charAt(0);
//     c[1] = inputString.charAt(1);
//     c[2] = inputString.charAt(2);
//     c[3] = inputString.charAt(3);
//     c[4] = 0;
//     inputString = "";
//     stringComplete = false;

//     // -- apply human move --
//     Serial.print(" Think ");
//     K = *c - 16 * c[1] + 799;
//     L = c[2] - 16 * c[3] + 799;
//     N = 0; T = 0x3F;
//     bkp();
//     int r = D(-I, I, Q, O, 1, 3);
//     if (!(r > -I + 1)) {
//       Serial.println("Lose ");
//       gameOver();
//     }
//     strcpy(lastH, c);
//     mn++;

//     // -- computer move (Black) --
//     playComputerMove(blackDepth);
//     printMoveAndBoard();

//   }
// }


// #include "ChessEngine.h"
// ChessEngine engine;

// void setup() {
//   Serial.begin(9600);
//   while(!Serial);
//   delay(500);
//   Serial.println("Start");
//   Serial.println("Press ENTER when ready to start...");
//   while (Serial.read() != '\n') { /* wait */ }
//   // seed at the moment the user hit ENTER
//   engine.setSeed( (uint32_t)micros() );
// }


// void loop() {
//   bool white = true;
//   if (white) {  
//     engine.playComputerMove(3);     // e.g. use depth 3
//   }
//   else {
//     engine.playComputerMove(4);
//   }
//   engine.printMoveAndBoard();
//   engine.waitForEnter();
//   white = !white;
// }


// #include "ChessEngine.h"
// ChessEngine engine;

// void setup() {
//   Serial.begin(9600);
//   while(!Serial);
//   Serial.println("Start");
//   // Try toggling between a fixed seed vs. a jittery seed:
//   // engine.setSeed(1234);             // predictable
//   engine.setSeed( micros() );       // unpredictable
// }

// void loop() {
//   bool white = true;
//   if (white) {  
//     engine.playComputerMove(3);     // e.g. use depth 3
//   }
//   else {
//     engine.playComputerMove(4);
//   }
//   engine.printMoveAndBoard();
//   engine.waitForEnter();
//   white = !white;
// }

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
  //engine.waitForEnter();
}
