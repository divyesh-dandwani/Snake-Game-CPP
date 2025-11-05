# 🐍 Snake Game in C++

A **terminal-based Snake Game** built in **C++**, featuring difficulty selection, random obstacle clusters, smooth movement, and player-controlled speed — all running directly in the Windows Command Prompt.

---

## 🎮 Features
- Choose from **Easy / Medium / Hard** difficulty levels.  
- **Dynamic obstacle clusters** make every game unique.  
- **WASD** and **Arrow keys** supported for movement.  
- **+ / -** to control snake speed during gameplay.  
- **Score display**, **restart**, and **quit** options.  
- Smooth, flicker-free console rendering.

---

## ✅ Acceptance Criteria
- Smooth snake movement with keyboard controls (**W/A/S/D** or **Arrow keys**).  
- Snake grows and score updates correctly after eating food.  
- Food never spawns on snake body or obstacles.  
- Accurate collision detection (self, walls, or obstacles).  
- Game Over screen displays:
  - Final score  
  - Restart / Exit option  

---

## ⚙️ Technical Overview
- Written in **C++** using only standard libraries and `windows.h`.  
- Uses **vectors** for snake body and obstacle management.  
- **Procedural generation** for clustered obstacles and food placement.  
- Frame timing handled with **chrono** for consistent movement.  
- Designed for **pure terminal gameplay** — no external IDE graphics.

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| **W / ↑** | Move Up |
| **A / ←** | Move Left |
| **S / ↓** | Move Down |
| **D / →** | Move Right |
| **+ / -** | Speed Up / Slow Down |
| **R** | Restart Game |
| **Q** | Quit Game |

---

## 🚀 How to Run
```bash
g++ snake.cpp -o snake
snake
