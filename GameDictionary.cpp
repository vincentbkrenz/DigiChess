#include "GameDictionary.h"
#include "constants.h"
#include <Arduino.h>

// ---------- FLASH GAME DATA ----------
// Each game is a null-terminated list of move strings
const char* const game1[] = {
    "e2e4", "d7d6",
    "d2d4", "g8f6",
    "b1c3", "g7g6",
    "c1e3", "f8g7",
    "d1d2", "c7c6",
    "f2f3", "b7b5",
    "g1e2", "b8d7",
    "e3h6", "g7h6",
    "d2h6", "b8b7",
    "a2a3", "e7e5",
    "e1c1", "d8e7",
    "c1b1", "a7a6",
    "c3e1", "e8c8",
    "e1b3", "e5d4",
    "d4d5", "c6c5",
    "d1d4", "c5d4",
    "g2g3", "b8b8",
    "b3a5", "a8b8",
    "h3h3", "d7d5",
    "h6f4", "b8a7",
    "h1e1", "d5d4",
    "e2d5", "b7d5",
    "e4d5", "e7d6",
    "d1d4", "c4d4",
    "e7e7", "b6a5",
    "d4d4", "a5a4",
    "c3c3", "d6d5",
    "a1a7", "b7b7",
    "b7b7", "c4c4",
    "f6f6", "c8a3",
    "a6a6", "b4b4",
    "c3c3", "c3c3",
    "a1a4", "d2d2",
    "b2b2", "d1d1",
    "f1f1", "d2d2",
    "d7d7", "d7d7",
    "c4c4", "b4b4",
    "h8h8", "d3d3",
    "a8a8", "c3c3",
    "a4a4", "e1e1",
    "f4f4", "f5f5",
    "c1c1", "d2d2",
    "a7a7", nullptr
};

const char* const game2[] = { "d2d4", "d7d5", "c1f4", "g8f6", nullptr };
const char* const game3[] = { "c2c4", "e7e6", "d2d4", "d7d5", "b1c3", nullptr };

const char* const* const allGames[] = {
    game1,
    game2,
    game3
};
const size_t NUM_GAMES = sizeof(allGames) / sizeof(allGames[0]);

// ---------- Game Implementation ----------
Game::Game() : head(nullptr), current(nullptr) {}

Game::~Game() {
    MoveNode* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

void Game::addMove(const String& move) {
    MoveNode* newNode = new MoveNode(move);
    if (!head) {
        head = newNode;
        current = head;
    } else {
        MoveNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

bool Game::nextMove() {
    if (current && current->next) {
        current = current->next;
        return true;
    }
    return false;
}

void Game::reset() {
    current = head;
}

String Game::getCurrentMove() const {
    return current ? current->move : "";
}

bool Game::hasNext() const {
    return current && current->next;
}

// ---------- GameDictionary Implementation ----------
GameDictionary::GameDictionary() : head(nullptr), current(nullptr) {
    loadFromFlash();  // Initialize all games from flash
}

GameDictionary::~GameDictionary() {
    GameNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp->game;
        delete temp;
    }
}

void GameDictionary::addGame(Game* game) {
    GameNode* newNode = new GameNode(game);
    if (!head) {
        head = newNode;
        current = head;
    } else {
        GameNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

bool GameDictionary::nextGame() {
    if (current && current->next) {
        current = current->next;
        return true;
    } else if (current) {
        // Wrap around to the head
        current = head;
        return true;
    }
    return false;
}

bool GameDictionary::previousGame() {
    if (!current || !head) return false;

    if (current == head) {
        // Wrap around to the last node
        GameNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        current = temp;
        return true;
    }

    // Otherwise, find the node before current
    GameNode* prev = head;
    while (prev->next != current && prev->next != nullptr) {
        prev = prev->next;
    }

    if (prev) {
        current = prev;
        return true;
    }

    return false;
}


void GameDictionary::reset() {
    current = head;
}

Game* GameDictionary::getCurrentGame() const {
    return current ? current->game : nullptr;
}

bool GameDictionary::hasNext() const {
    return current && current->next;
}

// ---------- Load from Flash ----------
void GameDictionary::loadFromFlash() {
    for (size_t i = 0; i < NUM_GAMES; ++i) {
        Game* game = new Game();
        const char* const* moveList = allGames[i];
        for (size_t j = 0; moveList[j] != nullptr; ++j) {
            game->addMove(String(moveList[j]));
        }
        addGame(game);
    }
}
