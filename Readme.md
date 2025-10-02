# Snake Game in C++

A console-based Snake Game implemented in C++. This project simulates the classic snake game where the player controls a snake to eat food, grow in size, and avoid collisions with itself. It includes normal and special food items, score tracking, and dynamic gameplay.

---

## Table of Contents
- [Features](#features)
- [File Structure](#file-structure)
- [Installation](#installation)
- [How to Play](#how-to-play)
- [Game Mechanics](#game-mechanics)
- [Dependencies](#dependencies)

---

## Features
- Fully playable Snake Game in the console.
- Arrow keys and WASD controls for snake movement.
- Normal food increases score and snake length.
- Special food appears periodically and temporarily and grants bonus points.
- Real-time collision detection with itself.
- Simple console-based graphics for easy compilation and execution.

---

## File Structure
SnakeGame/
├── snake.cpp   # Main game code
├── Snake.h     # Header file for snake and game functions (if used)
|── README.md   # Project documentation

- The Folder also has a snake.exe file present in .gitignore which is expected to be made by the user.


---

## Installation

```bash
1. Clone the repository:
    - git clone <your-repo-url>

2. Select the folder:    
    - cd SnakeGame

3. Complie the code using C++:
    - g++ snake.cpp -o snake.exe

4. Run the Game:
    # Windows  
    snake.exe

    # Linux / Mac
    ./snake
```
---
## How to Play
 
- Press any key to start the game.

- Use the arrow keys to move the snake:

      - Up: ↑
      - Down: ↓
      - Left: ←
      - Right: →
- You can also use WASD keys to control the movement:

      - Up: W
      - Left: A
      - Down: S
      - Right: D

      WASD are basically the letters in your keyboard.      

- Eat the normal food (o) to grow in length and increase your score.

- Eat special food (#) when it appears for bonus points.

- Avoid colliding with the snake’s own body.
---

## Game Mechanics
- Snake Movement: The snake moves continuously in the current direction.

- Food Spawning:
```
  - Normal food appears randomly on the board
  - Special food appears at every 10th second for a limited time
```
- Scoring System:
```bash
  - Normal food: +1 points
  - Special food: +10 points (appears temporarily)
```
- Game Over:
```
    - Snake hits the wall
    - Snake collides with itself
```
---
## Dependencies

- C++ Compiler: g++, clang++, or any compiler supporting C++11 or higher.

- Windows Only: Uses <conio.h> and <windows.h> for input handling.

- Cross-platform adaptation may require modifying input and timing functions.