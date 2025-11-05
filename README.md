# Overloaders_snakegame
Overview

The Snake Game - Cross-Platform is a fun and interactive implementation of the classic Snake game. Developed as a college project, the game is built using C++ and utilizes terminal-based graphics with emojis for an enhanced user experience. This game is designed to work on multiple platforms, providing a simple yet engaging way to enjoy the traditional Snake game with some modern twists, including randomly placed obstacles and food items.

The game allows players to control the snake using arrow keys, trying to consume food while avoiding collisions with walls, obstacles, and the snake’s own body. As the snake eats food, it grows longer, and the game becomes more challenging. The player’s goal is to achieve the highest possible score without losing the game.

Key features include a dynamic gameplay environment with randomly placed obstacles, a customizable grid size, and a smooth, responsive user interface.

Features

Cross-Platform Compatibility: The game is designed to work across various platforms using C++ and terminal-based graphics.

Emoji-Based Graphics: The game features emojis like 🐍 for the snake, 🍎 for the food, and 🧱 for the walls, creating a visually engaging and fun environment.

Randomly Placed Obstacles: Adds an extra layer of difficulty with obstacles that the snake must avoid.

Dynamic Gameplay: The snake grows longer as it eats food, increasing the difficulty and challenge as the game progresses.

Real-Time Controls: Uses arrow keys to control the snake's direction, with smooth and responsive movement.

Game Over Mechanism: The game ends when the snake collides with the wall, its own body, or an obstacle. The player is given the option to restart or quit.

Score Tracking: Keeps track of the player's current score and the highest score achieved during the session.

Simple Restart and Quit Options: After a game over, the player can restart the game or exit, making it easy to keep playing or end the session.

System Architecture

The Snake Game - Cross-Platform follows a simple, modular architecture, with various components working together to provide smooth gameplay. The architecture can be broken down into the following key components:

1. Game Initialization (setup())

Purpose: Initializes the game state, including the snake, food, obstacles, and score.

Details:

The snake is represented as a vector of coordinates, starting with three segments in a straight line.

Obstacles are randomly placed in the game area, avoiding snake positions.

A random food position is assigned, ensuring that it doesn't overlap with obstacles or the snake.

2. Input Handling (input())

Purpose: Captures user input from the keyboard to control the snake's movement.

Details:

The program uses a non-buffered input method to handle real-time keypress events (via kbhit() and read()).

Arrow keys are mapped to change the direction of the snake.

The game also listens for the 'Q' or 'R' keys to quit or restart after a game over.

3. Game Logic (logic())

Purpose: Contains the core game logic, such as snake movement, collision detection, and score updates.

Details:

The snake’s head moves in the specified direction (up, down, left, right).

Collision with the walls, obstacles, or the snake's own body results in a game over.

If the snake eats food, the score increases, and the snake grows by adding a new segment at the head.

If no food is eaten, the last segment of the snake is removed to simulate movement.

4. Rendering (draw())

Purpose: Responsible for drawing the game state to the terminal window.

Details:

Clears the terminal and redraws the entire game grid.

Displays the snake, food, obstacles, and walls using emoji-based graphics (e.g., 🟩 for snake, 🍎 for food, 🧱 for walls).

The score and game status (Game Over, Restart, Quit) are displayed at the top of the screen.

Controls are shown to guide the player.

5. Random Food and Obstacle Placement (placeFood())

Purpose: Ensures that food and obstacles are placed randomly on the grid, avoiding existing snake positions.

Details:

The function checks that the randomly generated food and obstacles do not overlap with the snake or other obstacles.

6. Terminal Input/Output Handling

Purpose: Manages terminal settings for user input and output.

Details:

Uses the termios library to disable buffered input, allowing for real-time keypress detection.

Controls screen clearing and redrawing using system("clear").

Flow Overview:

Game Setup: The game initializes the snake, food, obstacles, and other variables.

Main Loop: Continuously runs the game until the player loses or quits.

Checks user input for direction change.

Updates the snake’s position based on the current direction.

Handles collisions and food consumption.

Renders the updated game state to the screen.

Technical Implementation

The Snake Game - Cross-Platform is implemented in C++ and uses a terminal-based interface with emoji-based graphics. Here's an overview of the technical details:

1. Programming Language

Built in C++ for efficiency and low-level system control.

2. Terminal Input/Output

The game utilizes emoji graphics for the snake, food, obstacles, and walls, providing a visually engaging experience in the terminal.

Non-buffered input is used for real-time keyboard detection, allowing the player to control the snake instantly with arrow keys.

3. Game Logic

The snake is represented by a vector of (x, y) coordinates, with the head at the front. Movement is controlled by updating the head's position based on the player's input.

Game Over Conditions: Triggered by collisions with walls, the snake’s body, or obstacles.

Food Consumption: The snake grows and the score increases when it eats food. A new piece of food is placed at a random location.

4. Random Food and Obstacle Placement

Both food and obstacles are placed randomly within the grid, ensuring they don't overlap with the snake or other obstacles.

5. Input Handling

Key Detection: The kbhit() function detects keypresses in real time, processing arrow key inputs for movement and ‘Q’ or ‘R’ for quit and restart.

6. Rendering the Game

The game is drawn using a grid with characters: 🧱 for walls, 🟩 for the snake, 🍎 for food, and 🪨 for obstacles.

Score Display: Shows current and max scores at the top, with game over messages when applicable.

7. Game Loop and Timing

The main game loop handles drawing the game state, processing input, and updating logic with a short delay (usleep(150000)) to control the game speed.

8. Memory Management

Vectors are used to store the snake's body and obstacles. These dynamic containers manage memory automatically.

Installation & Compilation

Follow these steps to set up and run the Snake Game - Cross-Platform:

Prerequisites

C++ Compiler (e.g., g++).

Linux/Unix-based systems (MacOS or Linux) are recommended.

Steps

Clone or Download the project files.

Navigate to the Project Directory in your terminal.

Compile the Code:

g++ -o snake_game main.cpp


Run the Game:

./snake_game

Troubleshooting

Ensure g++ is installed.

On Windows, use WSL or Cygwin for terminal support.

Usage

Start the Game: After compiling, run the game using:

./snake_game


Game Controls:

Arrow Keys: Control the snake's movement (Up, Down, Left, Right).

Q or q: Quit the game.

R or r: Restart the game after a game over.

Objective: Navigate the snake to eat food (🍎) while avoiding obstacles (🪨) and the walls (🧱). The snake grows longer with each piece of food consumed. The game ends if the snake hits an obstacle, its own body, or the walls.

Score: Your current score and maximum score will be displayed at the top of the screen.

Design Patterns & Best Practices
Design Patterns

Singleton Pattern:

The game state (snake, food, obstacles) is initialized once and remains consistent throughout the game.

Strategy Pattern:

The snake’s movement direction is dynamically controlled based on user input (arrow keys), adapting in real-time.

Best Practices

Modular Code:

Functions like setup(), input(), draw(), and logic() ensure clean, maintainable, and reusable code.

Constants:

Important values like width, height, and score are declared as constants for easy modifications.

Efficient Memory Management:

Vectors are used to manage dynamic elements like the snake’s body and obstacles.

Separation of Concerns:

Game logic (movement, collisions) is separated from rendering (display), improving clarity and flexibility.

Intuitive Game Flow:

A straightforward design that makes the game easy to play and modify.
Future Enhancements

Graphics and GUI:

Transition from terminal-based emojis to a graphical user interface (GUI) using libraries like SFML or SDL for a more visually rich experience.

Multiplayer Mode:

Add a multiplayer feature where two players can control separate snakes and compete for food while avoiding each other.

Power-ups:

Introduce special power-ups (e.g., speed boosts, shields) to make gameplay more exciting and dynamic.

Customizable Grid:

Allow players to customize the grid size and difficulty level, providing more replayability.

High Score System:

Implement a persistent high score system that saves the best scores locally or on the cloud.

Sound Effects and Music:

Add background music and sound effects for a more immersive gaming experience.

Team Contributors

Hardi

Zalak

Megha

Bhavika

We worked collaboratively as a team on all aspects of the project, including game logic, user interface design, input handling, performance optimization, and overall implementation. Every team member contributed to each part of the game to ensure smooth development and a successful final product.
