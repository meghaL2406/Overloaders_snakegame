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

using namespace std;

const int width = 50;
const int height = 30;

vector<pair<int, int>> snake;
pair<int, int> fruit;
bool isPowerFruit = false; // 🍇 flag
vector<pair<int, int>> obstacles;

int dirX = 1, dirY = 0;
bool gameOver = false;
bool paused = false;
int score = 0, maxScore = 0;
string currentDir = "→";
int appleCount = 0; // Track apples eaten

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

void placeFruit() {
    while (true) {
        fruit.first = rand() % (width - 2) + 1;
        fruit.second = rand() % (height - 2) + 1;
        bool valid = true;
        for (auto &s : snake)
            if (s == fruit)
                valid = false;
        for (auto &o : obstacles)
            if (o == fruit)
                valid = false;
        if (valid)
            break;
    }
}

void setup() {
    snake.clear();
    int startX = width / 2;
    int startY = height / 2;
    snake.push_back({startX, startY});
    snake.push_back({startX - 1, startY});
    snake.push_back({startX - 2, startY});

    dirX = 1;
    dirY = 0;
    score = 0;
    appleCount = 0;
    gameOver = false;
    paused = false;
    currentDir = "→";
    isPowerFruit = false;

    obstacles.clear();
    for (int i = 0; i < 5; i++) {
        obstacles.push_back({rand() % (width - 2) + 1, rand() % (height - 2) + 1});
    }

    placeFruit();
}

void draw() {
    clearScreen();

    int termCols, termRows;
    getTerminalSize(termCols, termRows);

    int totalGameWidth = width * 2;
    int totalGameHeight = height + 5;
    int paddingLeft = max(0, (termCols - totalGameWidth) / 2);
    int paddingTop = max(0, (termRows - totalGameHeight) / 2);

    for (int i = 0; i < paddingTop; i++) cout << "\n";

    cout << string(paddingLeft, ' ') << "🐍 SNAKE GAME (with Power Fruit) 🐍\n";
    cout << string(paddingLeft, ' ') << "Score: " << score << " | Max Score: " << maxScore << "\n\n";

    for (int y = 0; y < height; y++) {
        cout << string(paddingLeft, ' ');
        for (int x = 0; x < width; x++) {
            bool printed = false;

            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                cout << "🟫";
                continue;
            }

            if (x == fruit.first && y == fruit.second) {
                cout << (isPowerFruit ? "🍇" : "🍎");
                printed = true;
            }

            if (!printed) {
                for (auto &o : obstacles)
                    if (x == o.first && y == o.second) {
                        cout << "🪨";
                        printed = true;
                        break;
                    }
            }

            if (!printed) {
                for (auto &s : snake)
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

    if (paused)
        cout << "⏸️ Game Paused — Press P to Resume\n";
    else if (!gameOver)
        cout << "🎮 Controls: Arrow Keys | P → Pause | Current: " << currentDir << "\n";
    else {
        cout << "💀 GAME OVER 💀\n";
        cout << string(paddingLeft, ' ') << "Press R → Restart | Q → Quit\n";
    }

    cout << flush;
}

void input() {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
#else
    if (kbhit()) {
        int ch = getch();
#endif

        if (ch == 'p' || ch == 'P') { paused = !paused; return; }
        if (ch == 'q' || ch == 'Q') { gameOver = true; return; }
        if ((ch == 'r' || ch == 'R') && gameOver) { setup(); return; }

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

        if (paused) return;

        switch (ch) {
            case 72: if (dirY == 0) { dirX = 0; dirY = -1; currentDir = "↑"; } break;
            case 80: if (dirY == 0) { dirX = 0; dirY = 1; currentDir = "↓"; } break;
            case 77: if (dirX == 0) { dirX = 1; dirY = 0; currentDir = "→"; } break;
            case 75: if (dirX == 0) { dirX = -1; dirY = 0; currentDir = "←"; } break;
        }
    }
}

void logic() {
    if (paused || gameOver) return;

    int x = snake.front().first + dirX;
    int y = snake.front().second + dirY;

    // Collisions
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) {
        gameOver = true;
        if (score > maxScore) maxScore = score;
        return;
    }

    for (auto &o : obstacles)
        if (o.first == x && o.second == y) {
            gameOver = true;
            if (score > maxScore) maxScore = score;
            return;
        }

    for (auto &s : snake)
        if (s.first == x && s.second == y) {
            gameOver = true;
            if (score > maxScore) maxScore = score;
            return;
        }

    snake.insert(snake.begin(), {x, y});

    // Fruit eaten
    if (x == fruit.first && y == fruit.second) {
        if (isPowerFruit) {
            score += 3;
            snake.push_back(snake.back());
            snake.push_back(snake.back()); // Grow 2 extra
            isPowerFruit = false;
        } else {
            score += 1;
            appleCount++;
            // Random chance for next fruit to be power fruit
            if (appleCount >= 3 && (rand() % 4 == 0)) { // 1 in 4 chance after 3 apples
                isPowerFruit = true;
                appleCount = 0;
            }
        }
        placeFruit();
    } else {
        snake.pop_back();
    }
}

int main() {
    srand(time(0));
    setup();

    while (true) {
        draw();
        input();
        logic();

#ifdef _WIN32
        Sleep(150);
#else
        usleep(150000);
#endif
    }

    return 0;
}    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}
#endif

// Clear screen & move cursor to top (cross-platform)
void clearScreen() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
#else
    cout << "\033[H\033[2J";
#endif
}

// Get terminal width and height
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

// Random food placement avoiding snake/obstacles
void placeFood() {
    while (true) {
        food.first = rand() % (width - 2) + 1;
        food.second = rand() % (height - 2) + 1;
        bool valid = true;

        for (auto &s : snake)
            if (s.first == food.first && s.second == food.second)
                valid = false;
        for (auto &o : obstacles)
            if (o.first == food.first && o.second == food.second)
                valid = false;

        if (valid)
            break;
    }
}

void setup() {
    snake.clear();

    int startX = width / 2;
    int startY = height / 2;
    snake.push_back({startX, startY});
    snake.push_back({startX - 1, startY});
    snake.push_back({startX - 2, startY});

    dirX = 1;
    dirY = 0;
    score = 0;
    gameOver = false;
    paused = false;
    currentDir = "→";

    obstacles.clear();
    for (int i = 0; i < 5; i++) {
        int ox = rand() % (width - 2) + 1;
        int oy = rand() % (height - 2) + 1;
        obstacles.push_back({ox, oy});
    }

    placeFood();
}

void draw() {
    clearScreen();

    int termCols, termRows;
    getTerminalSize(termCols, termRows);

    // Centering calculation
    int totalGameWidth = width * 2;  
    int totalGameHeight = height + 5;
    int paddingLeft = max(0, (termCols - totalGameWidth) / 2);
    int paddingTop = max(0, (termRows - totalGameHeight) / 2);

    for (int i = 0; i < paddingTop; i++) cout << "\n";

    cout << string(paddingLeft, ' ') << "🐍 SNAKE (Emoji Edition) 🐍\n";
    cout << string(paddingLeft, ' ') << "Score: " << score << " | Max Score: " << maxScore << "\n\n";

    for (int y = 0; y < height; y++) {
        cout << string(paddingLeft, ' ');
        for (int x = 0; x < width; x++) {
            bool printed = false;

            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                cout << "🧱";
                continue;
            }

            if (x == food.first && y == food.second) {
                cout << "🍎";
                printed = true;
            }

            if (!printed) {
                for (auto &o : obstacles)
                    if (x == o.first && y == o.second) {
                        cout << "🪨";
                        printed = true;
                        break;
                    }
            }

            if (!printed) {
                for (auto &s : snake)
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

    if (paused)
        cout << "⏸️ GAME PAUSED — Press P to Resume\n";
    else if (!gameOver)
        cout << "🎮 Controls: Arrow Keys → Move | P → Pause | Current: " << currentDir << "\n";
    else {
        cout << "💀 GAME OVER 💀\n";
        cout << string(paddingLeft, ' ') << "Press R → Restart | Q → Quit\n";
    }

    cout << flush;
}

void input() {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
#else
    if (kbhit()) {
        int ch = getch();
#endif

        // 🔹 Handle normal keys first
        if (ch == 'p' || ch == 'P') { paused = !paused; return; }
        if (ch == 'q' || ch == 'Q') { gameOver = true; return; }
        if ((ch == 'r' || ch == 'R') && gameOver) { setup(); return; }

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

        if (paused) return; // no movement when paused

        switch (ch) {
            case 72: if (dirY == 0) { dirX = 0; dirY = -1; currentDir = "↑"; } break;
            case 80: if (dirY == 0) { dirX = 0; dirY = 1; currentDir = "↓"; } break;
            case 77: if (dirX == 0) { dirX = 1; dirY = 0; currentDir = "→"; } break;
            case 75: if (dirX == 0) { dirX = -1; dirY = 0; currentDir = "←"; } break;
        }
    }
}

void logic() {
    if (paused || gameOver) return;

    int x = snake.front().first + dirX;
    int y = snake.front().second + dirY;

    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) {
        gameOver = true;
        if (score > maxScore) maxScore = score;
        return;
    }

    for (auto &o : obstacles)
        if (o.first == x && o.second == y) {
            gameOver = true;
            if (score > maxScore) maxScore = score;
            return;
        }

    for (auto &s : snake)
        if (s.first == x && s.second == y) {
            gameOver = true;
            if (score > maxScore) maxScore = score;
            return;
        }

    snake.insert(snake.begin(), {x, y});

    if (x == food.first && y == food.second) {
        score++;
        placeFood();
    } else {
        snake.pop_back();
    }
}

int main() {
    srand(time(0));
    setup();

    while (true) {
        draw();
        input();
        logic();

#ifdef _WIN32
        Sleep(150);
#else
        usleep(150000);
#endif
    }

    return 0;
}
