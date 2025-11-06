Overview
The Snake Game - Cross-Platform is a fun and interactive implementation of the classic Snake game. Developed as a college project, the game is built using C++ and utilizes terminal-based graphics with emojis for an enhanced user experience.
This game works on multiple platforms, providing a simple yet engaging way to enjoy the traditional Snake game with some modern twists, including randomly placed obstacles, special power fruits, and a pause feature.
Players control the snake using arrow keys, trying to consume food while avoiding collisions with walls, obstacles, and the snake’s own body. As the snake eats food, it grows longer, and the game becomes more challenging. The goal is to achieve the highest possible score without losing the game.
Key features include a dynamic gameplay environment with random obstacles, power fruits, a pause/resume function, a customizable grid size, and smooth, responsive controls.
Features
Cross-Platform Compatibility:
Works across various platforms using C++ and terminal-based graphics.
Emoji-Based Graphics:
Features emojis like 🐍 for the snake, 🍎 for the food, 🧱 for the walls, and 🪨 for the obstacles for a visually fun experience.
Randomly Placed Obstacles:
Adds an extra layer of difficulty as the snake must avoid randomly generated obstacles.
Dynamic Gameplay:
The snake grows longer as it eats food, increasing the challenge as the game continues.
🟢 New: Power Fruit Feature:
After every few apples (randomly between 4–6), a special power fruit (🍇 or any chosen emoji) appears.
When eaten, it doubles the snake’s length instantly and gives extra points.
After being consumed, normal apples (🍎) return as usual.
⏸️ New: Pause Feature:
Players can press 'P' anytime to pause or resume the game — perfect for breaks during gameplay.
Real-Time Controls:
Uses arrow keys for movement with smooth, instant response.
Game Over Mechanism:
Ends when the snake collides with a wall, obstacle, or itself, displaying restart and quit options.
Score Tracking:
Tracks both current and maximum scores during the session.
Simple Restart & Quit Options:
After game over, players can press R to restart or Q to quit.
System Architecture
The Snake Game - Cross-Platform follows a simple, modular architecture for smooth and efficient gameplay.
Game Initialization (setup())
Initializes snake, food, obstacles, and scores.
Starts the snake with three body segments.
Randomly places obstacles and food avoiding snake overlap.
Input Handling (input())
Captures real-time key input.
Handles:
Arrow keys → Movement
P → Pause/Resume
Q → Quit
R → Restart
Game Logic (logic())
Controls snake movement, growth, and collisions.
Detects:
Wall/obstacle/body collisions → Game Over
Apple consumption → Increases score and length
Power Fruit consumption → Doubles snake’s size and adds bonus score
Rendering (draw())
Redraws the game board each frame.
Displays:
🧱 Walls
🟩 Snake
🍎 Apples
🍇 Power Fruits
🪨 Obstacles
Shows current direction, scores, and game status.
Random Food & Obstacle Placement (placeFood())
Ensures food and obstacles are placed randomly without overlapping snake or obstacles.
Includes random trigger for power fruits after every few apples.
Terminal Input/Output Handling
Uses the termios library for non-buffered input.
Controls screen clearing and redrawing using escape sequences for a smooth visual experience.
Flow Overview
Game Setup: Initialize snake, food, obstacles, and other variables.
Main Loop:
Process user input.
Update movement and logic.
Detect collisions.
Render new frame.
Control speed using small delays.
Pause Feature:
Press P to pause and resume gameplay anytime.
Power Fruit Mechanic:
Appears randomly after several apples and disappears after eaten.
Doubles snake’s size and increases score.
Technical Implementation
Programming Language
Built entirely in C++ for performance and low-level control.
Terminal Input/Output
Emoji-based graphics for visual appeal.
Non-buffered input for real-time control.
Game Logic
Snake stored as a vector of coordinate pairs.
Head moves according to current direction.
Power fruit doubles snake length instantly.
Game over on collision.
Rendering
Grid display with:
🧱 Walls
🟩 Snake
🍎 Normal Food
🍇 Power Fruit
🪨 Obstacles
Displays live score and direction.
Game Loop & Timing
Controlled using usleep(150000) for balanced speed.
Memory Management
Uses dynamic vectors for efficient storage of snake and obstacle positions.
Installation & Compilation
Prerequisites
C++ Compiler (g++)
Terminal environment (Linux/Mac recommended)
Steps
Clone or download project files.
Open terminal in the project directory.
Compile using:
g++ -o snake_game main.cpp
Run using:
./snake_game
Usage
Controls
Arrow Keys: Move the snake
P: Pause/Resume
Q: Quit the game
R: Restart after Game Over
Objective
Eat 🍎 and 🍇 fruits while avoiding 🪨 and 🧱.
Grow your snake and aim for the highest score!
Design Patterns & Best Practices
Design Patterns
Singleton Pattern:
Game state is initialized once and remains consistent.
Strategy Pattern:
Snake movement is dynamically controlled based on user input.
Best Practices
Modular Code:
Functions like setup(), input(), logic(), and draw() ensure clarity.
Constants:
Width, height, and speed are defined as constants.
Efficient Memory Management:
Uses vectors for dynamic data.
Separation of Concerns:
Game logic is separated from rendering.
Intuitive Flow:
Smooth, easy-to-understand gameplay loop.
Future Enhancements
Graphics and GUI:
Upgrade from terminal to GUI using SFML or SDL.
Multiplayer Mode:
Two-player competitive mode.
Advanced Power-ups:
Speed boost, shield, or shrinking features.
Customizable Grid:
User-defined grid size and difficulty.
High Score System:
Persistent local or cloud-based scores.
Sound Effects:
Add background music and effects for immersion.
Team Contributors
👩‍💻 Megha 202512054
👩‍💻 Bhavika 202512053
👩‍💻 Hardi 202512085
👩‍💻 Zalak 202512091
We worked collaboratively as a team on all aspects of the project — from game logic, UI design, and input handling, to performance optimization and feature enhancements like the Pause and Power Fruit features.
