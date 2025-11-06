# 🐍 Snake Game — Cross-Platform Terminal Edition

---

## 📑 Index

1. [Introduction](#introduction)
2. [Project Objective](#project-objective)
3. [Key Features](#key-features)
4. [System Design](#system-design)
5. [Class Structure](#class-structure)
6. [Technical Implementation](#technical-implementation)
7. [Controls](#controls)
8. [Installation & Execution](#installation--execution)
9. [Performance and Optimization](#performance-and-optimization)
10. [Future Enhancements](#future-enhancements)
11. [Team Members](#team-members)

---

## 🧩 Introduction

The **Snake Game** is a cross-platform terminal-based game written entirely in **C++**, designed to run smoothly on **Windows**, **Linux**, and **macOS**.
It uses **ANSI escape codes** and system-level console functions for real-time color, emoji display, and keyboard input handling.
Players can select from multiple **themes**, enjoy smooth animations, and compete for high scores — all within the terminal.

---

## 🎯 Project Objective

The main goal of this project is to develop a **simple yet visually engaging Snake Game** that demonstrates:

* **Object-oriented programming (OOP)** in C++.
* **Cross-platform compatibility** using conditional compilation.
* **Terminal graphics** with smooth frame rendering and color effects.
* **Efficient data structures** for dynamic movement and collision handling.

---

## ✨ Key Features

* 🎨 **Multiple Color Themes:** Neon, Jungle, Ice, Retro, and more.
* 🍎 **Emoji Food:** Random food emojis (🍎, 🍉, 🍒, etc.) based on theme.
* 🧱 **Random Blocks:** Obstacles appear at different positions each time.
* 🐍 **Dynamic Snake Growth:** The snake grows every time it eats food.
* 💥 **Collision Detection:** Detects wall, self, and block collisions.
* 💾 **Live Score Display:** Real-time score updates on screen.
* 🌈 **Rainbow Mode:** Animated rainbow colors for the snake.
* ⚡ **Speed Control:** Increase or decrease snake speed during gameplay.
* 🖥️ **Cross-Platform Input:** Works with `conio.h` (Windows) and `termios` (Linux/macOS).

---

## 🧠 System Design
Architecture Overview
┌──────────────────────────────┐
│          Game Loop           │
│   (Timing, Rendering, Score) │
└─────────────┬────────────────┘
              │
 ┌────────────┼─────────────┐
 │            │             │
 ▼            ▼             ▼
Snake     Food System    Renderer
(Body)    (Randomize)    (Draws UI)

Component Description
| Component         | Function                                          |
| ----------------- | ------------------------------------------------- |
| **Snake**         | Maintains position, direction, and body growth    |
| **Food System**   | Randomly generates food avoiding snake and blocks |
| **Renderer**      | Handles visuals, themes, and terminal output      |
| **Input Manager** | Reads non-blocking keyboard input                 |
| **Theme Manager** | Manages colors and UI styles                      |


The system is divided into five key components:

1. **Terminal Handling:**
   Controls cursor movement, screen clearing, and color management.

2. **Theme Management:**
   Defines the color palette and food emoji list for each theme.

3. **Game Logic:**
   Manages movement, growth, collisions, and score updates.

4. **Rendering Engine:**
   Handles visual elements — snake, walls, blocks, and UI.

5. **User Input System:**
   Uses non-blocking keyboard input for smooth gameplay.

---

## 🏗️ Class Structure

### **1. struct Point**

```cpp
struct Point {
    int x, y;
};
```

Used for storing coordinates of:

* Snake body segments
* Food
* Blocks

---

### **2. class Terminal**

Handles all **console-based operations** such as positioning, clearing, and color control.

**Key Methods:**

* `clear()` → Clears the terminal screen.
* `gotoXY(int x, int y)` → Moves the cursor to specific coordinates.
* `setColor(int fg, int bg = 0)` → Sets text color (foreground & background).
* `hideCursor()` / `showCursor()` → Controls cursor visibility.
* `kbhit()` / `getChar()` → Handles non-blocking input.
* `sleep_ms(int ms)` → Adds delay for smooth frame timing.



### **3. class Theme**

Stores visual settings for each environment (color scheme + food emojis).

```cpp
class Theme {
public:
    string name;
    int wallColor, blockColor, headColor, bodyColor, foodColor, uiColor;
    vector<string> foodEmojis;
};
```

**Example Themes:**

* Neon Tech
* Inferno
* Jungle
* Ice
* Retro



### **4. Game Logic (Main Section)**

Main functions in your loop include:

* `generateBlocks()` – Randomly creates wall-like obstacles.
* `drawBlocks()` – Renders blocks at fixed or random coordinates.
* `generateFood()` – Spawns food avoiding snake and obstacles.
* `drawBox()` – Draws game border using Unicode line characters.
* `selectTheme()` – Lets player choose the preferred theme.
* `showTitle()` – Displays animated title screen.
* `showScoreboard()` – Displays score, theme, and speed on screen.


## 💻 Technical Implementation

| Component                  | Technology / Logic Used                                       |
| -------------------------- | ------------------------------------------------------------- |
| **Language**               | C++ (C++11 Standard)                                          |
| **Input**                  | Non-blocking keyboard input using `getChar()` and `kbhit()`   |
| **Graphics**               | ASCII characters, Unicode borders, ANSI color codes           |
| **Randomization**          | `rand()` and `srand(time(0))` for food and obstacle placement |
| **Cross-Platform Support** | `#ifdef _WIN32` and `<termios.h>` for input handling          |
| **Game Loop**              | Continuous frame update with speed control                    |
| **Collision Detection**    | Checks snake head position vs wall, self, and blocks          |



## 🎮 Controls

| Key       | Action                    |
| --------- | ------------------------- |
| **W / ↑** | Move Up                   |
| **S / ↓** | Move Down                 |
| **A / ←** | Move Left                 |
| **D / →** | Move Right                |
| **+ / -** | Increase / Decrease Speed |
| **R**     | Toggle Rainbow Mode       |
| **Q**     | Quit the Game             |



## ⚙️ Installation & Execution

### **Requirements**

* Any C++11-compatible compiler (GCC, Clang, or MSVC)
* UTF-8 supported terminal

### **Steps to Run**

#### **Windows**

```bash
g++ -std=c++11 snake.cpp -o snake.exe
snake.exe
```

#### **Linux / macOS**

```bash
g++ -std=c++11 snake.cpp -o snake
./snake
```



## ⚡ Performance and Optimization

* Minimal flickering via cursor positioning instead of full-screen refresh.
* Non-blocking input ensures real-time movement.
* Efficient vector management for snake body tracking.
* Frame delay optimized for CPU efficiency and smooth animation.



## 🔮 Future Enhancements

* Add a pause/resume option.
* Save and display top scores.
* Add multiple difficulty levels.
* Include background music (via external library).
* Add AI or multiplayer mode.



## 👥 Team Members

| Name                 | ID        | 
| -------------------- | --------- | 
| **Diya Jain**        | 202512014 | 
| **Divyesh Dandwani** | 202512043 | 
| **Vivek Dhanwani**   | 202512045 | 
| **Harshil Dodwani**  | 202512044 | 



