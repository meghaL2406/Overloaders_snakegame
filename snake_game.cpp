#include <conio.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;
void GameBoard::render(const string& playerName) {
    SetCursorPosition(0, 0);
    updateGrid();
    // Top wall
    for (int i = 0; i < width + 2; i++) cout << "-";
    cout << endl;
    for (int i = 0; i < height; i++) {
        cout << "|";
        for (int j = 0; j < width; j++) {
            cout << grid[i][j];
        }
        cout << "|" << endl;
    }
    // Bottom wall
    for (int i = 0; i < width + 2; i++) cout << "-";
    cout << endl;
    cout << playerName << "'s Score: " << score << endl;
}
// required header file

// Forward declarations
class Snake;
class Food;
class GameBoard;

class Snake {
public:
    enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
    struct Cell {
        int x, y;
    };
    vector<Cell> body;
    Direction dir;
    Snake(int startX, int startY) {
        dir = STOP;
        for (int i = 0; i < 3; ++i) {
            body.push_back({startX - i, startY});
        }
    }
    void setDirection(Direction d) { dir = d; }
    void move() {
        if (dir == STOP) return;
        // Move tail
        for (int i = body.size() - 1; i > 0; --i) {
            body[i] = body[i - 1];
        }
        // Move head
        switch (dir) {
        case LEFT:  body[0].x--; break;
        case RIGHT: body[0].x++; break;
        case UP:    body[0].y--; break;
        case DOWN:  body[0].y++; break;
        default: break;
        }
    }
    void grow() {
        body.push_back(body.back());
    }
    bool isCollision(int x, int y) {
        for (size_t i = 0; i < body.size(); ++i) {
            if (body[i].x == x && body[i].y == y) return true;
        }
        return false;
    }
    int headX() const { return body[0].x; }
    int headY() const { return body[0].y; }
};

class Food {
public:
    int x, y;
    Food(int gridW, int gridH) { x = rand() % gridW; y = rand() % gridH; }
    void respawn(int gridW, int gridH, const std::vector<Snake::Cell>& snakeBody) {
        while (true) {
            x = rand() % gridW;
            y = rand() % gridH;
            bool onSnake = false;
            for (const auto& cell : snakeBody) {
                if (cell.x == x && cell.y == y) { onSnake = true; break; }
            }
            if (!onSnake) break;
        }
    }
};

class GameBoard {
public:
    int width, height;
    int score, highScore;
    bool isGameOver;
    Snake snake;
    Food food;
    std::vector<std::vector<char>> grid;
        GameBoard(int w, int h)
                : width(w), height(h), score(0), highScore(0), isGameOver(false),
                    snake(w / 2, h / 2), food(w, h), grid(h, std::vector<char>(w, ' ')) {}
    void render(const string& playerName);
    void update() {
        // Move snake
        snake.move();
        // Check wall collision
        if (snake.headX() < 0 || snake.headX() >= width || snake.headY() < 0 || snake.headY() >= height) {
            isGameOver = true;
            if (score > highScore) highScore = score;
            return;
        }
        // Check self collision
        for (size_t i = 1; i < snake.body.size(); ++i) {
            if (snake.body[i].x == snake.headX() && snake.body[i].y == snake.headY()) {
                isGameOver = true;
                if (score > highScore) highScore = score;
                return;
            }
        }
        // Check food collision
        if (snake.headX() == food.x && snake.headY() == food.y) {
            score += 10;
            snake.grow();
            food.respawn(width, height, snake.body);
        }
    }
    void handleInput() {
        if (_kbhit()) {
            int ch = _getch();
            switch (ch) {
            case 'a':
            case 75: // Left arrow
                if (snake.dir != Snake::RIGHT) snake.setDirection(Snake::LEFT);
                break;
            case 'd':
            case 77: // Right arrow
                if (snake.dir != Snake::LEFT) snake.setDirection(Snake::RIGHT);
                break;
            case 'w':
            case 72: // Up arrow
                if (snake.dir != Snake::DOWN) snake.setDirection(Snake::UP);
                break;
            case 's':
            case 80: // Down arrow
                if (snake.dir != Snake::UP) snake.setDirection(Snake::DOWN);
                break;
            case 'x':
                isGameOver = true;
                break;
            }
        }
    }
    void reset() {
        score = 0;
        snake = Snake(width / 2, height / 2);
        food.respawn(width, height, snake.body);
    }
    void updateGrid() {
        // Clear grid
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                grid[i][j] = ' ';
        // Place food
        grid[food.y][food.x] = '#';
        // Place snake
        for (size_t i = 0; i < snake.body.size(); ++i) {
            int sx = snake.body[i].x;
            int sy = snake.body[i].y;
            if (sx >= 0 && sx < width && sy >= 0 && sy < height)
                grid[sy][sx] = (i == 0) ? 'O' : 'o';
        }
    }
};
// Function to set cursor position in console
void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// height and width of the boundary
const int width = 80;
const int height = 20;

// Function to initialize game variables
// (Removed: handled by GameBoard class)

// Remove the standalone GameRender function, as rendering is handled by GameBoard::render.

// (Removed: UpdateGame function is obsolete. Game logic is handled by GameBoard::update.)

// Function to set the game difficulty level
int SetDifficulty()
{
    int dfc, choice;
    cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: hard "
            "\nNOTE: if not chosen or pressed any other "
            "key, the difficulty will be automatically set "
            "to medium\nChoose difficulty level: ";
    cin >> choice;
    switch (choice) {
    case '1':
        dfc = 50;
        break;
    case '2':
        dfc = 100;
        break;
    case '3':
        dfc = 150;
        break;
    default:
        dfc = 100;
    }
    return dfc;
}

// (Removed: UserInput function is obsolete. Input is handled by GameBoard::handleInput.)

// Main function / game looping function
int main()
{
    string playerName;
    cout << "\n==============================\n";
    cout << "      SNAKE GAME (IT603)      \n";
    cout << "==============================\n";
    cout << "Controls: W/A/S/D or Arrow Keys to move\n";
    cout << "Eat '#' to grow. Avoid walls and yourself!\n";
    cout << "Press 'X' anytime to exit.\n";
    cout << "\nEnter your name: ";
    cin >> playerName;
    int baseDelay = 100; // Will be improved later

    // Hide the blinking cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

// (Removed: UserInput function is obsolete. Input is handled by GameBoard::handleInput.)
