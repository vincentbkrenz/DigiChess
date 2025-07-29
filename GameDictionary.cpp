#include "GameDictionary.h"
#include "constants.h"
#include <Arduino.h>

// ---------- FLASH GAME DATA ----------
// Each game is a null-terminated list of move strings
const char* const game1[] = { "e2e4", "e7e5", "g1f3", "b8c6", nullptr };
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
