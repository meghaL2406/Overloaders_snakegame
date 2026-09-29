#pragma once
// game_state.h — shared declarations for the seamed snake game
// Both p1_seamed.cpp and lab4/tests.cpp include this header.

#include <vector>
#include <string>
#include <functional>
using namespace std;

const int width  = 50;
const int height = 30;

struct GameState {
    vector<pair<int,int>> snake;
    pair<int,int>         fruit;
    bool                  isPowerFruit = false;
    vector<pair<int,int>> obstacles;
    int                   dirX = 1, dirY = 0;
    bool                  gameOver = false;
    bool                  paused   = false;
    int                   score    = 0;
    int                   maxScore = 0;
    string                currentDir = "→";
    int                   appleCount = 0;

    // Injectable RNG: production code uses rand(), tests use a lambda.
    function<int(int)> rng = [](int n){ return rand() % n; };
};

// Public API — defined in p1_seamed.cpp
void placeFruit(GameState &gs);
void setup(GameState &gs);
void draw(const GameState &gs);
void input(GameState &gs);
void logic(GameState &gs);
