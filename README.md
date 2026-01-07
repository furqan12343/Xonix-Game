# 🎮 Xonix Game – SFML (C++)

A complete implementation of the classic **Xonix arcade game** developed in **C++ using SFML**, with a strong focus on **Data Structures, Algorithms, and Object-Oriented Programming**.

This project demonstrates how fundamental computer science concepts can be applied in a **real-time, interactive game environment**, making it suitable for **academic submissions, learning purposes, and portfolio projects**.

---

## 📌 Project Overview

Xonix is a territory-capturing arcade game where the player moves across a grid-based field, creating boundaries to capture areas while avoiding enemies.  
This implementation recreates the original gameplay mechanics while maintaining **clean, modular, and efficient C++ code**.

The game logic is designed using **core data structures**, efficient algorithms, and a structured game loop powered by **SFML**.

---

## 🧠 Data Structures & Algorithms Used

- **2D Arrays / Grids**  
  Used to represent the game field, boundaries, player trails, and captured regions

- **Structures & Classes (OOP)**  
  Encapsulation of game entities such as player, enemies, and game state

- **Vectors**  
  Dynamic management of multiple enemies

- **Queue-Based Flood Fill Algorithm**  
  Used for territory capture and area filling logic

- **Collision Detection Algorithms**  
  Real-time collision checks between player, enemies, and trails

- **Game Loop Algorithm**  
  Efficient update–process–render cycle

- **State Management**  
  Handling game states such as running and game over

---

## 🕹️ Gameplay Features

- Grid-based movement system
- Area capturing by enclosing regions
- Multiple enemies with dynamic movement
- Real-time collision detection
- Score calculation based on captured area
- Life system and game-over logic
- Smooth rendering and keyboard input handling using SFML

---

## 🛠️ Technologies Used

- **Programming Language:** C++
- **Graphics Library:** SFML
- **Programming Paradigm:** Object-Oriented Programming (OOP)
- **Core Focus:** Data Structures & Algorithms

---

## 🚀 How to Build and Run

### 🔹 Prerequisites
- C++ Compiler (g++ / clang / MSVC)
- SFML (version 2.5 or later)

---

### 🔹 Build on Linux / macOS

```bash
git clone https://github.com/your-username/xonix-game.git
cd xonix-game

g++ *.cpp -o xonix \
-lsfml-graphics -lsfml-window -lsfml-system

./xonix
