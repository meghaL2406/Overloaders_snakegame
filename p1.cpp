#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <cstdlib>
#include <ctime>
#include <sys/select.h>   // ✅ Added for select()
#include <sys/time.h>     // ✅ Added for timeval

using namespace std;

const int width = 25;
const int height = 15;

vector<pair<int, int>> snake;
pair<int, int> food;
vector<pair<int, int>> obstacles;

int dirX = 1, dirY = 0;
bool gameOver = false;
int score = 0, maxScore = 0;
string currentDir = "➡️";

// Terminal input
void disableBufferedInput(bool enable)
{
    static struct termios oldt, newt;
    if (!enable)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

bool kbhit()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

// Random food placement avoiding obstacles/snake
void placeFood()
{
    while (true)
    {
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

void setup()
{
    snake.clear();

    // Start snake with 3 cells in a line (moving right)
    int startX = width / 2;
    int startY = height / 2;
    snake.push_back({startX, startY});
    snake.push_back({startX - 1, startY});
    snake.push_back({startX - 2, startY});

    dirX = 1;
    dirY = 0;
    score = 0;
    gameOver = false;
    currentDir = "➡️";

    obstacles.clear();
    for (int i = 0; i < 5; i++)
    {
        int ox = rand() % (width - 2) + 1;
        int oy = rand() % (height - 2) + 1;
        obstacles.push_back({ox, oy});
    }

    placeFood();
}

// Draw centered
void draw()
{
    system("clear");

    int termWidth = 80; // assumed average terminal width
    int margin = (termWidth - width * 2) / 2;
    if (margin < 0)
        margin = 0;

    string space(margin, ' ');

    cout << "\n\n";
    cout << space << "🐍 SNAKE (Emoji Edition) 🐍\n";
    cout << space << "Score: " << score << " | Max Score: " << maxScore << "\n\n";

    for (int y = 0; y < height; y++)
    {
        cout << space;
        for (int x = 0; x < width; x++)
        {
            bool printed = false;
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
            {
                cout << "🧱";
                continue;
            }

            if (x == food.first && y == food.second)
            {
                cout << "🍎";
                printed = true;
            }

            for (auto &o : obstacles)
            {
                if (x == o.first && y == o.second)
                {
                    cout << "🪨";
                    printed = true;
                    break;
                }
            }

            for (auto &s : snake)
            {
                if (x == s.first && y == s.second)
                {
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

    cout << "\n";

    if (!gameOver)
    {
        cout << space << "🎮 Controls: Arrow Keys → Move | Current: " << currentDir << "\n";
    }
    else
    {
        cout << space << "💀 GAME OVER 💀\n";
        cout << space << "Final Score: " << score << " | Max Score: " << maxScore << "\n";
        cout << space << "Press R → Restart | Q → Quit\n";
    }

    cout << "\n\n";
}

void input()
{
    if (kbhit())
    {
        char c;
        read(STDIN_FILENO, &c, 1);
        if (c == '\033')
        { // Arrow key prefix
            read(STDIN_FILENO, &c, 1);
            if (c == '[')
            {
                read(STDIN_FILENO, &c, 1);
                switch (c)
                {
                case 'A': // Up
                    if (dirY == 0)
                    {
                        dirX = 0;
                        dirY = -1;
                        currentDir = "⬆️";
                    }
                    break;
                case 'B': // Down
                    if (dirY == 0)
                    {
                        dirX = 0;
                        dirY = 1;
                        currentDir = "⬇️";
                    }
                    break;
                case 'C': // Right
                    if (dirX == 0)
                    {
                        dirX = 1;
                        dirY = 0;
                        currentDir = "➡️";
                    }
                    break;
                case 'D': // Left
                    if (dirX == 0)
                    {
                        dirX = -1;
                        dirY = 0;
                        currentDir = "⬅️";
                    }
                    break;
                }
            }
        }
        else if ((c == 'q' || c == 'Q') && gameOver)
        {
            disableBufferedInput(true);
            system("clear");
            cout << "\n\n👋 Thanks for playing! Final Max Score: " << maxScore << "\n\n";
            exit(0);
        }
        else if ((c == 'r' || c == 'R') && gameOver)
        {
            setup();
        }
    }
}

// Game logic
void logic()
{
    int x = snake.front().first + dirX;
    int y = snake.front().second + dirY;

    // Border hit
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1)
    {
        gameOver = true;
        if (score > maxScore)
            maxScore = score;
        return;
    }

    // Obstacle hit
    for (auto &o : obstacles)
        if (o.first == x && o.second == y)
        {
            gameOver = true;
            if (score > maxScore)
                maxScore = score;
            return;
        }

    // Self-collision
    for (auto &s : snake)
        if (s.first == x && s.second == y)
        {
            gameOver = true;
            if (score > maxScore)
                maxScore = score;
            return;
        }

    // Move snake
    snake.insert(snake.begin(), {x, y});

    if (x == food.first && y == food.second)
    {
        score++;
        placeFood();
    }
    else
    {
        snake.pop_back();
    }
}

int main()
{
    srand(time(0));
    disableBufferedInput(false);
    setup();

    while (true)
    {
        draw();
        input();
        if (!gameOver)
            logic();
        usleep(150000);
    }

    disableBufferedInput(true);
    return 0;
}
