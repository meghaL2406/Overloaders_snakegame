
// lab4/tests.cpp
// Lab 4 – Part B/C/D/E test suite
// Build command (from repo root, Windows MinGW):
//   g++ -std=c++14 -DTESTS_MAIN -I. -o lab4/tests.exe p1_seamed.cpp lab4/tests.cpp
// Run: lab4\tests.exe

// TESTS_MAIN is defined via -DTESTS_MAIN on the compiler command line

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

#include "lab4/game_state.h"   // GameState struct + declarations

using namespace std;

// ── Minimal test harness ──────────────────────────────────────────────────────
static int passed = 0, failed_count = 0;

#define EXPECT_TRUE(expr) \
    do { if (!(expr)) { \
        cerr << "FAIL  " << __FILE__ << ":" << __LINE__ << "  " #expr "\n"; \
        ++failed_count; \
    } else { cout << "PASS  " #expr "\n"; ++passed; } } while(0)

#define EXPECT_EQ(a,b) \
    do { auto _a=(a); auto _b=(b); if (_a!=_b) { \
        cerr << "FAIL  " << __FILE__ << ":" << __LINE__ \
             << "  expected " << _b << " got " << _a << "\n"; \
        ++failed_count; \
    } else { cout << "PASS  " #a " == " #b "\n"; ++passed; } } while(0)

// ── Helper: build a minimal testable GameState ────────────────────────────────
static GameState makeState() {
    GameState gs;
    gs.snake = {{10, 10}, {9, 10}, {8, 10}};
    gs.fruit = {40, 20};              // far away
    gs.isPowerFruit = false;
    gs.obstacles.clear();
    gs.dirX = 1; gs.dirY = 0;        // moving right
    gs.gameOver = false;
    gs.paused = false;
    gs.score = 0;
    gs.maxScore = 0;
    gs.appleCount = 0;
    // Deterministic rng: always returns 0 → fruit placed at (1,1)
    gs.rng = [](int) -> int { return 0; };
    return gs;
}

// ── Rule 1: Self-collision kills the snake ────────────────────────────────────
// The snake head steps onto its own body segment → gameOver becomes true.
void test_self_collision_kills() {
    cout << "\n[Rule 1] Self-collision kills the snake\n";

    GameState gs = makeState();
    // Snake at (10,10)→(11,10)→(12,10), moving right.
    // Next head position = 11,10 which is already occupied by body[1].
    gs.snake = {{10, 10}, {11, 10}, {12, 10}};
    gs.dirX = 1; gs.dirY = 0;

    EXPECT_TRUE(!gs.gameOver);          // guard: not dead yet
    logic(gs);
    EXPECT_TRUE(gs.gameOver);           // must die on self-collision
}

// ── Rule 2: Eating a normal fruit adds exactly 1 to score ────────────────────
void test_eating_normal_fruit_adds_one() {
    cout << "\n[Rule 2] Eating normal fruit increments score by exactly 1\n";

    GameState gs = makeState();
    // Fruit is directly in front of head (head at 10,10 moving right → next=11,10)
    gs.fruit = {11, 10};
    gs.isPowerFruit = false;
    // rng always returns 20 → placeFruit puts fruit at (21, 21) after eating
    gs.rng = [](int) -> int { return 20; };

    int before = gs.score;
    logic(gs);
    EXPECT_EQ(gs.score, before + 1);
    EXPECT_TRUE(!gs.gameOver);
}

// ── Rule 3: Wall collision kills the snake ────────────────────────────────────
void test_wall_collision_kills() {
    cout << "\n[Rule 3] Wall collision kills the snake\n";

    GameState gs = makeState();
    // Head at (48,10), moving right → next = (49,10) = width-1 = wall
    gs.snake = {{48, 10}, {47, 10}, {46, 10}};
    gs.dirX = 1; gs.dirY = 0;

    EXPECT_TRUE(!gs.gameOver);
    logic(gs);
    EXPECT_TRUE(gs.gameOver);
}

// ── Rule 4: Fruit is never placed on a snake cell ────────────────────────────
// placeFruit() loops until it finds a valid cell.
// We inject an rng that returns a snake-occupied cell first, then a safe cell.
void test_fruit_never_placed_on_snake() {
    cout << "\n[Rule 4] placeFruit never places fruit on the snake\n";

    GameState gs = makeState();
    gs.snake = {{10, 10}, {9, 10}, {8, 10}};

    // rng call 1 (x) = 9 → x = 9+1 = 10 = head.x  ← collision, retry
    // rng call 2 (y) = 9 → y = 9+1 = 10 = head.y  ← same cell as head
    // rng call 3 (x) = 30 → x = 31  ← safe
    // rng call 4 (y) = 30 → y = 31  ← safe
    int call = 0;
    gs.rng = [&](int) -> int {
        ++call;
        return (call <= 2) ? 9 : 30;
    };

    placeFruit(gs);

    bool onSnake = false;
    for (auto &s : gs.snake)
        if (s == gs.fruit) onSnake = true;

    EXPECT_TRUE(!onSnake);
    cout << "   fruit placed at (" << gs.fruit.first << "," << gs.fruit.second << ")\n";
}

// ── Rule 5: Power fruit adds exactly 3 to score ──────────────────────────────
void test_power_fruit_adds_three() {
    cout << "\n[Rule 5] Eating power fruit adds exactly 3 to score\n";

    GameState gs = makeState();
    gs.fruit = {11, 10};         // directly ahead
    gs.isPowerFruit = true;
    gs.rng = [](int) -> int { return 20; }; // placeFruit → (21,21)

    int before = gs.score;
    logic(gs);
    EXPECT_EQ(gs.score, before + 3);
    EXPECT_TRUE(!gs.isPowerFruit);   // flag must be cleared after eating
    EXPECT_TRUE(!gs.gameOver);
}

// ── Entry point ───────────────────────────────────────────────────────────────
int main() {
    cout << "=== Lab 4 Test Suite ===\n";

    test_self_collision_kills();
    test_eating_normal_fruit_adds_one();
    test_wall_collision_kills();
    test_fruit_never_placed_on_snake();
    test_power_fruit_adds_three();

    cout << "\n================================\n";
    cout << "Passed: " << passed << "  Failed: " << failed_count << "\n";
    return (failed_count == 0) ? 0 : 1;
}
