#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <chrono>
#include <poll.h>

using namespace std;

struct Point {
    int x, y;
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

// ===== Terminal Setup for macOS =====
void setBufferedInput(bool enable) {
    static bool enabled = true;
    static struct termios old;
    struct termios newt;

    if (enable && !enabled) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        enabled = true;
    } else if (!enable && enabled) {
        tcgetattr(STDIN_FILENO, &newt);
        old = newt;
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

// ===== Snake Game =====
class SnakeGame {
private:
    int width, height;
    deque<Point> snake;
    Point food;
    char dir;
    bool gameOver;
    int score;

public:
    SnakeGame(int w = 30, int h = 20) {
        width = w;
        height = h;
        srand(time(0));
        reset();
    }

    void reset() {
        snake.clear();
        snake.push_back({width / 2, height / 2});
        dir = 'R';
        score = 0;
        gameOver = false;
        generateFood();
    }

    void generateFood() {
        while (true) {
            int fx = rand() % (width - 2) + 1;
            int fy = rand() % (height - 2) + 1;
            bool onSnake = false;
            for (auto &s : snake)
                if (s.x == fx && s.y == fy)
                    onSnake = true;
            if (!onSnake) {
                food = {fx, fy};
                return;
            }
        }
    }

    void draw() {
        system("clear");
        cout << "===== 🐍 SNAKE GAME =====\n";
        cout << "Score: " << score << "\n\n";
        cout << "Controls:\n";
        cout << "  ↑  = Move Up\n";
        cout << "  ↓  = Move Down\n";
        cout << "  ←  = Move Left\n";
        cout << "  →  = Move Right\n";
        cout << "  Q  = Quit | R = Restart\n";
        cout << "==========================\n\n";

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
                    cout << "#";
                else if (x == food.x && y == food.y)
                    cout << "*";
                else if (x == snake.front().x && y == snake.front().y)
                    cout << "O";
                else {
                    bool body = false;
                    for (size_t i = 1; i < snake.size(); i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            cout << "o";
                            body = true;
                            break;
                        }
                    }
                    if (!body)
                        cout << " ";
                }
            }
            cout << "\n";
        }
    }

    void input() {
        if (kbhit()) {
            char c = getch();
            if (c == '\x1B') { // Arrow keys start with ESC
                char seq1 = getch();
                char seq2 = getch();
                if (seq1 == '[') {
                    switch (seq2) {
                        case 'A': if (dir != 'D') dir = 'U'; break; // Up
                        case 'B': if (dir != 'U') dir = 'D'; break; // Down
                        case 'C': if (dir != 'L') dir = 'R'; break; // Right
                        case 'D': if (dir != 'R') dir = 'L'; break; // Left
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

        // Collisions
        if (head.x == 0 || head.x == width - 1 || head.y == 0 || head.y == height - 1)
            gameOver = true;
        for (size_t i = 1; i < snake.size(); i++)
            if (snake[i] == head) gameOver = true;

        snake.push_front(head);

        // Eat food
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
            this_thread::sleep_for(chrono::milliseconds(400)); // 🐢 Slower speed
        }
        setBufferedInput(true);
        system("clear");
        cout << "\n💀 GAME OVER 💀\n";
        cout << "Your Score: " << score << "\n";
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
    SnakeGame game(30, 20);
    game.run();
    return 0;
}
