# 🐍 Snake Game - Cross-Platform

## 📝 Overview
The **Snake Game - Cross-Platform** is a fun and interactive implementation of the classic Snake game. Developed as a **college project**, the game is built using **C++** and utilizes **terminal-based graphics** with emojis for an enhanced user experience.

This game works on multiple platforms, providing a simple yet engaging way to enjoy the traditional Snake game with some modern twists, including randomly placed obstacles, special power fruits, and a pause feature.

Players control the snake using arrow keys (or customizable alphabet controls), trying to consume food while avoiding collisions with walls, obstacles, and the snake’s own body. As the snake eats food, it grows longer, and the game becomes more challenging. The goal is to achieve the **highest possible score** without losing.

---

## 🌟 Features

### 🧩 Cross-Platform Compatibility
Works seamlessly on Windows, Linux, and macOS using terminal-based graphics.

### 😄 Emoji-Based Graphics
Includes emojis for a lively feel: 🐍 (snake), 🍎 (fruit), 🍇 (power fruit), 🧱 (walls), and 🪨 (obstacles).

### 🪨 Randomly Placed Obstacles
Adds an extra challenge by placing random obstacles every new game.

### 🔁 Dynamic Gameplay
The snake grows longer as it eats, increasing difficulty as the game progresses.

### 🍇 Power Fruit Feature
- Appears **after every few apples (3–5 random range)**.  
- When eaten:
  - Adds **3 bonus points**.  
  - **Doubles the snake’s size instantly** (by pushing back extra body parts).  
- After consumption, normal apples 🍎 return.

### ⏸️ Pause Feature
- Press **'P'** to pause/resume anytime.  
- Lets players take breaks without losing progress.

### 🧮 Score Tracking
Displays **current score** and **maximum score** dynamically.

### ⚰️ Game Over Mechanism
Game ends if the snake collides with:
- A wall 🧱  
- An obstacle 🪨  
- Itself 🐍

After game over:
- **R** → Restart  
- **Q** → Quit  

---

## 🧠 System Architecture

### 1️⃣ Game Initialization (`setup()`)
- Initializes snake, food, obstacles, and scores.
- Places obstacles and food randomly, ensuring no overlap.

### 2️⃣ Input Handling (`input()`)
- Captures real-time input for movement and control.
- Supports:
  - **Arrow keys or alphabets** for movement.
  - **P** → Pause/Resume.
  - **R** → Restart.
  - **Q** → Quit.

### 3️⃣ Game Logic (`logic()`)
- Handles:
  - Snake movement and direction.
  - Fruit and power fruit consumption.
  - Collision detection (walls, obstacles, or self).

### 4️⃣ Rendering (`draw()`)
- Clears and redraws the game every frame.
- Displays:
  - 🧱 Walls  
  - 🟩 Snake  
  - 🍎 Apple  
  - 🍇 Power Fruit  
  - 🪨 Obstacles  
- Shows score and direction indicators.

---

## ⚙️ Technical Implementation

### 💻 Programming Language
- Built using **C++** for fast and efficient execution.

### 🎨 Terminal I/O
- Uses **termios** for non-buffered input (Linux/macOS).
- Uses **_kbhit()** and **_getch()** for Windows.

### 🕹️ Game Logic
- Snake stored as a **vector of coordinate pairs**.
- Movement controlled via direction vectors `(dirX, dirY)`.
- Power fruit logic increases size and score on consumption.

### 🖥️ Rendering
- Uses terminal escape sequences for clearing and redrawing.
- Emoji-based visuals enhance readability and fun.

### ⏱️ Game Loop & Speed
- Controlled with `usleep(150000)` (Linux/macOS) or `Sleep(150)` (Windows).
- Adjust value to **increase/decrease snake speed**:
  - Lower value → Faster snake.
  - Higher value → Slower snake.

---

## 🚀 Installation & Compilation

### 🔧 Prerequisites
- **C++ Compiler (g++)**
- **Terminal** (Linux/Mac) or **Command Prompt** (Windows)

### 🧩 Steps
1. Clone or download project files.  
2. Open terminal in the project directory.  
3. Compile using:
   ```bash
   g++ -o snake_game main.cpp
   ```
4. Run using:
   ```bash
   ./snake_game
   ```

---

## 🎮 Usage

### 🎯 Controls
| Key | Action |
|-----|---------|
| Arrow Keys | Move the Snake |
| P | Pause/Resume |
| R | Restart after Game Over |
| Q | Quit the Game |

### 🏆 Objective
Eat 🍎 and 🍇 while avoiding 🧱 and 🪨.  
Grow your snake 🐍 and aim for the **highest score**!

---

## 🧩 Design Patterns & Best Practices

### 💡 Design Patterns
- **Singleton Pattern:** Ensures single consistent game state.  
- **Strategy Pattern:** Movement dynamically changes based on user input.

### 🧰 Best Practices
- Modular functions (`setup()`, `input()`, `logic()`, `draw()`).
- Constants for width, height, and speed.  
- Efficient memory handling with `std::vector`.  
- Separation of logic and rendering for clean structure.

---

## 🔮 Future Enhancements
- 🕹️ **Graphical UI** using SFML or SDL.
- 👥 **Multiplayer Mode** (2-player).  
- ⚡ **New Power-ups** (speed boost, shield, shrink).  
- 🎵 **Sound Effects** for better immersion.  
- 💾 **Persistent High Scores** (local/cloud).  
- 🧱 **Customizable Grid Sizes** and difficulty settings.

---

## 👩‍💻 Team Contributors
| Name | Role |
|------|------|
| Hardi | Logic & Power Fruit Implementation |
| Zalak | Input Handling & Rendering |
| Megha | UI Design & Testing |
| Bhavika | Performance Optimization & Features |

> We worked collaboratively on every part of the project — from logic and UI design to optimizing performance and creating unique gameplay features like **Power Fruit** and **Pause Mode**.

---

### ❤️ Thank you for exploring our project!
