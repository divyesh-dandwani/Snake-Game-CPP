# 🐍 Snake Game in C++

A classic Snake Game implemented in **C++** using **Object-Oriented Programming (OOP)** principles.  
The game features a grid-based play area, a growing snake, random food spawning, collision detection, score tracking, and restart options.  

---

## 🎮 Features
- Grid-based game board (NxN cells).
- Snake starts with a length of 3.
- Food spawns randomly (never on the snake’s body).
- Snake grows by 1 cell after eating food.
- Score increases with each food item consumed.
- Game Over on:
  - Snake colliding with itself.
  - Snake hitting the boundaries.
- Displays current score during the game.
- Shows **final score** and **highest score** on Game Over.
- Option to restart or exit after Game Over.

---

## ⚙️ Technical Details
- **Language**: C++
- **Concepts Used**:
  - **Classes & Objects** → Snake, Food, GameBoard
  - **Encapsulation** → Managing game logic in classes
  - **Data Structures**:
    - Queue / Linked List → Snake body
    - 2D Array → Grid
  - **Game Loop** → Continuous movement and input handling
- **Extensible Design**:
  - Can be extended with obstacles, multiple levels, or special food.

---

## ✅ Acceptance Criteria
- Smooth snake movement with keyboard controls (W/A/S/D or arrow keys).
- Snake grows and score updates correctly on eating food.
- Food never spawns on snake body.
- Correct collision detection (self + boundary).
- Game Over screen shows:
  - Final score
  - Highest score
  - Restart / Exit option

---

## 📂 Project Structure (Planned)
