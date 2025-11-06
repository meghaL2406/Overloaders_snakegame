# 🐍 Snake Game (C++)

## 🎮 Overview
This is a console-based Snake Game built in **C++** that works on both **Windows** and **Linux/Mac**.  
The game includes a **Power Fruit (🍇)** feature, adjustable speed, and simple keyboard controls.

---

## ⚙️ Features
- Classic Snake movement with arrow keys.
- Randomly spawning **fruits** that increase the score and snake length.
- **Power Fruit (🍇)** that appears randomly and gives bonus points & extra growth.
- Adjustable **game speed**.
- Works on both Windows and POSIX (Linux/macOS) systems.

---

## 🧠 Logic Explanation

### 🐍 Snake Representation
- The snake is stored as a vector of coordinate pairs:
  ```cpp
  vector<pair<int, int>> snake;
  ```
  Each element represents one body segment (x, y).

### 🍎 Normal Fruit
- When the snake eats a normal fruit:
  ```cpp
  score++;
  snake.push_back(snake.back()); // grows by 1
  ```

### 🍇 Power Fruit Logic
- **Variables Used:**
  ```cpp
  bool isPowerFruit = false;
  int appleCount = 0;
  ```
- **When to spawn:**
  ```cpp
  appleCount++;
  if (appleCount >= 3 && (rand() % 4 == 0)) {
      isPowerFruit = true;
      appleCount = 0;
  }
  ```
  After every 3 apples, there's a 25% chance that the next fruit will be a Power Fruit.

- **When Power Fruit is eaten:**
  ```cpp
  score += 3;                    // Bonus points
  snake.push_back(snake.back()); // Double growth
  snake.push_back(snake.back());
  isPowerFruit = false;          // Reset to normal
  ```

### 🐢 Speed Control
- Snake speed is controlled by a delay in the main loop:
  ```cpp
  usleep(150000); // or Sleep(150) on Windows
  ```

- To adjust:
  - Increase value → **Slower** snake.
  - Decrease value → **Faster** snake.

- You can make the speed dynamic based on score:
  ```cpp
  int delay = 150000 - (score * 3000);
  if (delay < 50000) delay = 50000;
  ```

### 🎯 Objective
Eat as many fruits as possible without hitting walls or your own body.

---

## 💻 Controls
| Key | Action |
|-----|---------|
| ⬆️  | Move Up |
| ⬇️  | Move Down |
| ⬅️  | Move Left |
| ➡️  | Move Right |
| `X` | Exit Game |

---

## 🛠️ Compilation & Run

### On Windows (using MinGW or Dev-C++):
```bash
g++ snake.cpp -o snake.exe
snake.exe
```

### On Linux/Mac:
```bash
g++ snake.cpp -o snake
./snake
```

---

## 🏁 Future Enhancements
- Add levels or obstacles.
- Add pause/resume option.
- Implement score saving system.
- Sound effects and color display.

--

