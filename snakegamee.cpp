// snake_graphics.cpp
#include <iostream>
#include <deque>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <chrono>
#include <poll.h>

using namespace std;

// ANSI color codes 🎨
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

struct Point {
    int x, y;
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

// ===== Terminal Setup for macOS / Linux =====
void setBufferedInput(bool enable) {
    static bool enabled = true;
    static struct termios oldt;
    struct termios newt;

    if (enable && !enabled) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        enabled = true;
    } else if (!enable && enabled) {
        tcgetattr(STDIN_FILENO, &newt);
        oldt = newt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        enabled = false;
    }
}

bool kbhit() {
    struct pollfd fds;
    fds.fd = STDIN_FILENO;
    fds.events = POLLIN;
    return poll(&fds, 1, 0) > 0;
}

char getch() {
    char c;
    if (read(STDIN_FILENO, &c, 1) < 0) return 0;
    return c;
}

// ===== Snake Game with Graphics =====
class SnakeGame {
private:
    int width, height;
    deque<Point> snake;
    Point food;
    vector<Point> obstacles;
    char dir;
    bool gameOver;
    int score;

public:
    SnakeGame(int w = 40, int h = 20) {
        width = w;
        height = h;
        srand((unsigned)time(0));
        reset();
    }

    void reset() {
        snake.clear();
        obstacles.clear();
        snake.push_back({width / 2, height / 2});
        dir = 'R';
        score = 0;
        gameOver = false;
        generateObstacles(8); // 🧱 8 random obstacles
        generateFood();
    }

    void generateFood() {
        while (true) {
            int fx = rand() % (width - 2) + 1;
            int fy = rand() % (height - 2) + 1;
            bool conflict = false;

            for (auto &s : snake)
                if (s.x == fx && s.y == fy)
                    conflict = true;

            for (auto &o : obstacles)
                if (o.x == fx && o.y == fy)
                    conflict = true;

            if (!conflict) {
                food = {fx, fy};
                return;
            }
        }
    }

    void generateObstacles(int count) {
        for (int i = 0; i < count; i++) {
            int ox = rand() % (width - 4) + 2;
            int oy = rand() % (height - 4) + 2;
            obstacles.push_back({ox, oy});
        }
    }

    void draw() {
        system("clear");
        cout << CYAN << "===== 🐍 SNAKE GAME 2.0 =====" << RESET << "\n";
        cout << "Score: " << YELLOW << score << RESET << "\n\n";
        cout << "Controls:\n";
        cout << "  ↑  = Move Up\n";
        cout << "  ↓  = Move Down\n";
        cout << "  ←  = Move Left\n";
        cout << "  →  = Move Right\n";
        cout << "  Q  = Quit | R = Restart\n";
        cout << "=============================\n\n";

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                    cout << MAGENTA << "#" << RESET;
                } else if (x == food.x && y == food.y) {
                    cout << GREEN << "*" << RESET;
                } else if (x == snake.front().x && y == snake.front().y) {
                    cout << CYAN << "O" << RESET;
                } else {
                    bool printed = false;
                    for (size_t i = 1; i < snake.size(); i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            cout << YELLOW << "o" << RESET;
                            printed = true;
                            break;
                        }
                    }
                    for (auto &obs : obstacles) {
                        if (obs.x == x && obs.y == y) {
                            cout << RED << "X" << RESET;
                            printed = true;
                            break;
                        }
                    }
                    if (!printed)
                        cout << " ";
                }
            }
            cout << "\n";
        }
    }

    void input() {
        if (kbhit()) {
            char c = getch();
            if (c == '\x1B') { // Arrow keys
                char seq1 = getch();
                char seq2 = getch();
                if (seq1 == '[') {
                    switch (seq2) {
                        case 'A': if (dir != 'D') dir = 'U'; break;
                        case 'B': if (dir != 'U') dir = 'D'; break;
                        case 'C': if (dir != 'L') dir = 'R'; break;
                        case 'D': if (dir != 'R') dir = 'L'; break;
                    }
                }
            } else if (c == 'q' || c == 'Q') {
                gameOver = true;
            } else if ((c == 'r' || c == 'R') && gameOver) {
                reset();
                run();
            }
        }
    }

    void logic() {
        Point head = snake.front();
        switch (dir) {
            case 'U': head.y--; break;
            case 'D': head.y++; break;
            case 'L': head.x--; break;
            case 'R': head.x++; break;
        }

        // Collisions: walls or obstacles
        if (head.x == 0 || head.x == width - 1 || head.y == 0 || head.y == height - 1)
            gameOver = true;

        for (size_t i = 1; i < snake.size(); i++)
            if (snake[i] == head)
                gameOver = true;

        for (auto &o : obstacles)
            if (o == head)
                gameOver = true;

        snake.push_front(head);

        if (head == food) {
            score += 10;
            generateFood();
        } else {
            snake.pop_back();
        }
    }

    void run() {
        setBufferedInput(false);
        while (!gameOver) {
            draw();
            input();
            logic();
            this_thread::sleep_for(chrono::milliseconds(150)); // ⏱ faster
        }
        setBufferedInput(true);
        system("clear");
        cout << RED << "\n💀 GAME OVER 💀\n" << RESET;
        cout << "Your Score: " << YELLOW << score << RESET << "\n";
        cout << "Press 'R' to Restart or any key to Quit.\n";
        char ch;
        cin >> ch;
        if (ch == 'r' || ch == 'R') {
            reset();
            run();
        }
    }
};

int main() {
    SnakeGame game(40, 20);
    game.run();
    return 0;
}