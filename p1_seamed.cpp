#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif

#include "lab4/game_state.h"

using namespace std;

// ===== Keyboard setup for Linux/Mac =====
#ifndef _WIN32
int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
int getch() {
    termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

void clearScreen() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
#else
    cout << "\033[H\033[2J";
#endif
}

void getTerminalSize(int &cols, int &rows) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    cols = w.ws_col;
    rows = w.ws_row;
#endif
}

void placeFruit(GameState &gs) {
    while (true) {
        gs.fruit.first  = gs.rng(width  - 2) + 1;
        gs.fruit.second = gs.rng(height - 2) + 1;
        bool valid = true;
        for (auto &s : gs.snake)
            if (s == gs.fruit)
                valid = false;
        for (auto &o : gs.obstacles)
            if (o == gs.fruit)
                valid = false;
        if (valid)
            break;
    }
}

void setup(GameState &gs) {
    gs.snake.clear();
    int startX = width / 2;
    int startY = height / 2;
    gs.snake.push_back({startX, startY});
    gs.snake.push_back({startX - 1, startY});
    gs.snake.push_back({startX - 2, startY});

    gs.dirX = 1;
    gs.dirY = 0;
    gs.score = 0;
    gs.appleCount = 0;
    gs.gameOver = false;
    gs.paused = false;
    gs.currentDir = "→";
    gs.isPowerFruit = false;

    gs.obstacles.clear();
    for (int i = 0; i < 5; i++) {
        gs.obstacles.push_back({gs.rng(width - 2) + 1, gs.rng(height - 2) + 1});
    }

    placeFruit(gs);
}

void draw(const GameState &gs) {
    clearScreen();

    int termCols, termRows;
    getTerminalSize(termCols, termRows);

    int totalGameWidth = width * 2;
    int totalGameHeight = height + 5;
    int paddingLeft = max(0, (termCols - totalGameWidth) / 2);
    int paddingTop  = max(0, (termRows - totalGameHeight) / 2);

    for (int i = 0; i < paddingTop; i++) cout << "\n";

    cout << string(paddingLeft, ' ') << "🐍 SNAKE GAME (with Power Fruit) 🐍\n";
    cout << string(paddingLeft, ' ') << "Score: " << gs.score
         << " | Max Score: " << gs.maxScore << "\n\n";

    for (int y = 0; y < height; y++) {
        cout << string(paddingLeft, ' ');
        for (int x = 0; x < width; x++) {
            bool printed = false;

            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                cout << "🟫";
                continue;
            }

            if (x == gs.fruit.first && y == gs.fruit.second) {
                cout << (gs.isPowerFruit ? "🍇" : "🍎");
                printed = true;
            }

            if (!printed) {
                for (auto &o : gs.obstacles)
                    if (x == o.first && y == o.second) {
                        cout << "🪨";
                        printed = true;
                        break;
                    }
            }

            if (!printed) {
                for (auto &s : gs.snake)
                    if (x == s.first && y == s.second) {
                        cout << "🟩";
                        printed = true;
                        break;
                    }
            }

            if (!printed)
                cout << "  ";
        }
        cout << "\n";
    }

    cout << "\n" << string(paddingLeft, ' ');

    if (gs.paused)
        cout << "⏸️ Game Paused — Press P to Resume\n";
    else if (!gs.gameOver)
        cout << "🎮 Controls: Arrow Keys | P → Pause | Current: " << gs.currentDir << "\n";
    else {
        cout << "💀 GAME OVER 💀\n";
        cout << string(paddingLeft, ' ') << "Press R → Restart | Q → Quit\n";
    }

    cout << flush;
}

void input(GameState &gs) {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
#else
    if (kbhit()) {
        int ch = getch();
#endif

        if (ch == 'p' || ch == 'P') { gs.paused = !gs.paused; return; }
        if (ch == 'q' || ch == 'Q') { gs.gameOver = true; return; }
        if ((ch == 'r' || ch == 'R') && gs.gameOver) { setup(gs); return; }

#ifdef _WIN32
        if (ch == 0 || ch == 224) ch = _getch();
#else
        if (ch == 27 && kbhit()) {
            getch();
            ch = getch();
            if (ch == 'A') ch = 72;
            if (ch == 'B') ch = 80;
            if (ch == 'C') ch = 77;
            if (ch == 'D') ch = 75;
        }
#endif

        if (gs.paused) return;

        switch (ch) {
            case 72: if (gs.dirY == 0) { gs.dirX = 0; gs.dirY = -1; gs.currentDir = "↑"; } break;
            case 80: if (gs.dirY == 0) { gs.dirX = 0; gs.dirY =  1; gs.currentDir = "↓"; } break;
            case 77: if (gs.dirX == 0) { gs.dirX = 1; gs.dirY =  0; gs.currentDir = "→"; } break;
            case 75: if (gs.dirX == 0) { gs.dirX = -1; gs.dirY = 0; gs.currentDir = "←"; } break;
        }
    }
}

// logic() now operates on explicit GameState — the seam.
void logic(GameState &gs) {
    if (gs.paused || gs.gameOver) return;

    int x = gs.snake.front().first  + gs.dirX;
    int y = gs.snake.front().second + gs.dirY;

    // Wall collision
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) {
        gs.gameOver = true;
        if (gs.score > gs.maxScore) gs.maxScore = gs.score;
        return;
    }

    // Obstacle collision
    for (auto &o : gs.obstacles)
        if (o.first == x && o.second == y) {
            gs.gameOver = true;
            if (gs.score > gs.maxScore) gs.maxScore = gs.score;
            return;
        }

    // Self collision
    for (auto &s : gs.snake)
        if (s.first == x && s.second == y) {
            gs.gameOver = true;
            if (gs.score > gs.maxScore) gs.maxScore = gs.score;
            return;
        }

    gs.snake.insert(gs.snake.begin(), {x, y});

    // Fruit eaten
    if (x == gs.fruit.first && y == gs.fruit.second) {
        if (gs.isPowerFruit) {
            gs.score += 3;
            gs.snake.push_back(gs.snake.back());
            gs.snake.push_back(gs.snake.back()); // Grow 2 extra
            gs.isPowerFruit = false;
        } else {
            gs.score += 1;
            gs.appleCount++;
            if (gs.appleCount >= 3 && (gs.rng(4) == 0)) {
                gs.isPowerFruit = true;
                gs.appleCount = 0;
            }
        }
        placeFruit(gs);
    } else {
        gs.snake.pop_back();
    }
}

#ifndef TESTS_MAIN
int main() {
    srand(time(0));
    GameState gs;
    setup(gs);

    while (true) {
        draw(gs);
        input(gs);
        logic(gs);

#ifdef _WIN32
        Sleep(150);
#else
        usleep(150000);
#endif
    }

    return 0;
}
#endif
