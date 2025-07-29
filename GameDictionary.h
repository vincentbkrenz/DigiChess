#ifndef GAME_DICTIONARY_H
#define GAME_DICTIONARY_H

#include <Arduino.h>
#include "constants.h"

// =====================
// Move Node
// =====================
struct MoveNode {
    String move;
    MoveNode* next;
    MoveNode(const String& m) : move(m), next(nullptr) {}
};

// =====================
// Game Class
// =====================
class Game {
  private:
    MoveNode* head;
    MoveNode* current;

  public:
    Game();
    ~Game();
    void addMove(const String& move);
    bool nextMove();
    void reset();
    String getCurrentMove() const;
    bool hasNext() const;
    MoveNode* getHead() const { return head; }
};

// =====================
// Game Node
// =====================
struct GameNode {
    Game* game;
    GameNode* next;
    GameNode(Game* g) : game(g), next(nullptr) {}
};

// =====================
// Game Dictionary Class
// =====================
class GameDictionary {
  private:
    GameNode* head;
    GameNode* current;

    void loadFromFlash();  // New: load flash-stored games

  public:
    GameDictionary();
    ~GameDictionary();

    void addGame(Game* game);
    bool nextGame();
    bool previousGame();  
    void reset();
    Game* getCurrentGame() const;
    bool hasNext() const;
};

#endif // GAME_DICTIONARY_H
