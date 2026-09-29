# Lab 4 — Group Overloaders

|   |   |
|---|---|
| Repository | https://github.com/meghaL2406/Overloaders_snakegame |
| Base tag | `lab4-base` at commit `e508ed3` |
| Pull request | lab4 → master |

---

## 1. Five rules — [5]

Written before opening the source. Behaviour, with an observable outcome.

| # | Rule |
|---|---|
| 1 | The snake dies (game ends) when its head moves into any cell already occupied by its own body. |
| 2 | Eating a normal fruit (🍎) increases the score by exactly 1. |
| 3 | The snake dies (game ends) when its head hits a wall (the border of the grid). |
| 4 | Fruit is never placed on a cell that the snake currently occupies. |
| 5 | Eating a power fruit (🍇) increases the score by exactly 3 (not 1). |

---

## 2. What you could test, and what stopped you — [10]

No source changes in this part. Every `file:line` below is a line in `lab4-base`.

| # | Rule | Test written? | Blocking dependency (`file:line` + what it is) |
|---|---|---|---|
| 1 | Snake dies on self-collision | ❌ No | `p1.cpp:258` — `for (auto &s : snake)` iterates the **global** `snake` vector inside `logic()`, which is a global function bound to global state. There is no way to pass a custom snake vector without modifying the source. Additionally `p1.cpp:293` — `main()` runs an infinite `while(true)` loop containing `logic()`, so `logic()` cannot be called in isolation from a test. |
| 2 | Eating normal fruit adds 1 to score | ❌ No | `p1.cpp:29` — `score` is a module-level global; there is no way for a test to observe the return value of `logic()`. `p1.cpp:283` — `placeFruit()` calls `rand()` at `p1.cpp:93`, which is not injectable, so after eating we cannot control where the next fruit lands, making the test state non-deterministic. |
| 3 | Wall collision kills snake | ❌ No | `p1.cpp:241–248` — the wall-collision check is inside `logic()`, which accesses global `snake`, `dirX`, `dirY`, `gameOver`. No seam exists to set up a specific snake position and then call only that portion of logic. |
| 4 | Fruit never placed on snake | ❌ No | `p1.cpp:93` — `placeFruit()` calls `rand() % (width-2)` directly. `rand()` is not injectable, so a test cannot force the first draw to land on the snake body and verify that the loop retries. |
| 5 | Power fruit adds 3 to score | ❌ No | `p1.cpp:278` — the power-fruit branch inside `logic()` reads global `isPowerFruit` and global `score`. Same isolation problem as Rule 2: `logic()` cannot be called without the game loop, and `rand()` inside `placeFruit()` (called at `p1.cpp:283`) is not injectable. |

> **Rules testable without modifying the source: 0 / 5**

The single structural blocker is that all game state lives in module-level globals (`p1.cpp:21–31`), and `logic()` + `placeFruit()` call `rand()` directly (`p1.cpp:93`, `p1.cpp:126`, `p1.cpp:278`). There is no way to isolate any behaviour without either (a) running the full game loop (which requires `getch()` / `kbhit()`), or (b) modifying the source.

---

## 3. Coverage — [6]

Build command used:
```
g++ -std=c++14 -DTESTS_MAIN -I. --coverage -O0 -g \
    -o lab4/tests_cov.exe p1_seamed.cpp lab4/tests.cpp
./lab4/tests_cov.exe
gcov -b p1_seamed.cpp
```

Results (for `p1_seamed.cpp`, the seamed file used in Part D):

| Metric | Value |
|--------|-------|
| Lines executed | 28.15 % of 135 |
| Branches executed | 22.98 % of 235 |

The low percentages reflect lines that belong to `draw()`, `input()`, `clearScreen()`, `getTerminalSize()`, `setup()`, and `main()` — none of which are called by the test suite. Only `logic()` and `placeFruit()` are exercised.

**Which rule runs during a test but is never checked by an assertion?**

**Rule 4 — Fruit never placed on a snake cell.**

`test_fruit_never_placed_on_snake()` calls `placeFruit(gs)` and the loop at `p1_seamed.cpp:92–104` *does execute*. The rng is injected to return a snake-occupied cell first, so the `if (valid) break;` branch at line 102 is skipped on the first iteration. But the test never asserts anything about the *number of retries* or that the first candidate was indeed rejected. The only assertion is:

```cpp
EXPECT_TRUE(!onSnake);   // lab4/tests.cpp:95
```

The **missing assertion** is:
```cpp
EXPECT_TRUE(call > 2);   // verify the loop retried at least once
```
Without it, the test would still pass even if `placeFruit()` never looped — it only verifies the end-state, not the guard behaviour.

---

## 4. The seam, in two commits — [10]

**Rule made testable:** *The snake dies when its head enters its own body* (Rule 1).

This was the hardest to test because `logic()` depended on five module-level globals (`snake`, `dirX`, `dirY`, `gameOver`, `score`) and called `rand()` via `placeFruit()`. There was no entry point for a test to set up state, call `logic()`, and observe the outcome.

### Seam kind
**Object seam** — a `GameState` struct is introduced that bundles all mutable game state, including an injectable `rng` field (`std::function<int(int)>`). `logic()`, `placeFruit()`, `setup()`, `draw()`, and `input()` all accept a `GameState&` parameter instead of reading globals.

### Enabling point
The decision about which random values to use now lives in `GameState::rng`, which is set once in `main()` to `[](int n){ return rand() % n; }` (identical production behaviour) and overridden in tests with a deterministic lambda. For `logic()` itself, the enabling point is its signature: `void logic(GameState &gs)` — a test can construct any `gs`, call `logic(gs)`, and inspect `gs.gameOver`.

### What production code gave up
**Nothing visible to the player.** The game loop in `main()` constructs a `GameState gs`, calls `setup(gs)`, and then calls `draw(gs)`, `input(gs)`, `logic(gs)` in the same order as before. The only change in `main()` is the presence of the `GameState` local variable. No timing, no output, no keyboard handling changed. The game plays identically.

**Commit 1** (`seam: introduce GameState struct; logic() takes explicit state`) — adds `lab4/game_state.h`, rewrites `p1_seamed.cpp` with the struct and parameter-passing. No test file is touched. Running the game binary produces identical behaviour.

**Commit 2** (`test: five rules exercised via GameState seam`) — adds `lab4/tests.cpp`. No source file is modified.

---

## 5. The double — [4]

The object passed through the seam for Rules 2, 4, and 5 is `GameState::rng`, a `std::function<int(int)>` set in each test to a deterministic lambda (e.g., `[](int) -> int { return 20; }`).

**This is a stub.**

`rng` is *asked a question*: "give me a number in [0, n)". The collaborator returns a value, and the caller (`placeFruit`, `logic`) uses that value to make a decision. The test never asserts that `rng` was called a specific number of times or with specific arguments — it only uses the returned value to deterministically control where the fruit lands. A spy or mock would record the calls and assert on them; a stub simply returns a canned answer. Because `rand()` is a pure value-supplier (it returns an `int` and has no side effects visible to the caller), the double only needs to supply values, not verify interactions — which is exactly what a stub does.

---

## 6. Two smells in the new test suite — [5]

### Smell 1 — Assertion Roulette
**File:line:** `lab4/tests.cpp:102` (inside `test_power_fruit_adds_three`)

```cpp
EXPECT_EQ(gs.score, before + 3);
EXPECT_TRUE(!gs.isPowerFruit);
EXPECT_TRUE(!gs.gameOver);
```

Three assertions in one test function, none labelled beyond the macro string. If the second one fires (`!gs.isPowerFruit`), the error message only shows the expression — it does not explain *why* the power-fruit flag not being cleared matters or which step caused the failure.

**One-line fix:** Split into three separate test functions, or add a message string to each assertion:
```cpp
EXPECT_TRUE(!gs.isPowerFruit);  // isPowerFruit flag must reset after eating
```

---

### Smell 2 — General Fixture
**File:line:** `lab4/tests.cpp:26` (`makeState()` helper function)

`makeState()` constructs a snake at `{10,10}`, `{9,10}`, `{8,10}` with `dirX=1`, fruit at `{40,20}`, score=0, appleCount=0, obstacles empty, and a constant rng. Every single test calls `makeState()` then immediately *overrides* most of those fields (e.g., `test_self_collision_kills` replaces `snake`, `test_fruit_never_placed_on_snake` replaces `rng` with a stateful closure). The fixture is doing more than any one test needs.

**One-line fix:** Replace `makeState()` with the minimal inline construction each test actually needs — or have multiple smaller helpers (`makeStateForSelfCollision()`, `makeStateForFruitPlacement()`) that set only the fields their test uses.

---

*End of report.*
