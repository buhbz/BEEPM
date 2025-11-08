# 🌀 BEEPM — The osu! Stream BPM Tester

A rhythm analysis and training tool designed for **osu!standard** players who want to understand and improve their **tapping consistency**, **stream control**, and **stamina**.

---

## 💡 What is BEEPM?

**BEEPM** is a console-based stream BPM and rhythm analyzer for osu! players, speed-tappers, and rhythm enthusiasts.

It allows you to:
- Perform tap-based BPM and consistency tests to measure your stream speed and accuracy.
- Track timing intervals between taps and compute average BPM and consistency (UR).
- Visualize rhythm stability over time with an ASCII-based BPM graph.
- Save and review your progress with detailed logging.

---

## ⚡ Current Features

### 🧭 Core Functionality
- **Real-time BPM calculation** using high-precision timing between keypresses.
- **Fully configurable input keys** (default: `Z` / `X`), stored in `config.toml`.
- **Adjustable Taps-Per-Beat (TPB)** setting for custom rhythm patterns (default: `4` for 1/4 osu! streams).
- **Three training modes:**
  - **Normal Mode** – Tap a fixed number of times (20–1000) to measure speed bursts.
  - **Timed Mode** – Tap for a set duration (5–600s) to build endurance.
  - **Endurance Mode** – Maintain a chosen BPM as long as possible before dropping below it.
- **Detailed statistics** per test:
  - BPM  
  - Unstable Rate (UR)  
  - Consistency percentage  
  - Stream Skill rating (SR)

---

### 📊 BPM Graph Visualization
- After each session, BEEPM renders a clean ASCII graph showing **BPM stability**.
- Visually indicates rhythm fluctuations:
  - Smooth = consistent tapping  
  - Jagged = unstable tempo or fatigue onset  

---

### 📈 Progress Tracking
- Automatically saves all session data to `results.csv`:
  - Timestamp  
  - Mode  
  - BPM, UR, Consistency  
  - Score  
  - Tap count  
  - Duration  
- Built-in **Previous Scores** viewer lets you review past performance directly in-app.

---

### ⚙️ Settings System
- Configurable via `config.toml`:
  - Rebind tap keys.  
  - Adjust TPB (1–8).  
- In-app **Settings Editor** for modifying and saving configuration.
- Safe saving mechanism with temporary file protection.

---

### 💬 Interactive Console
- Clean, modern ASCII-art menu with six options:
  1. Start (Normal Mode)
  2. Timed Mode
  3. Endurance Mode
  4. Previous Scores
  5. Settings
  6. Exit
- Validates all inputs to prevent errors.

---

## 🧠 How It Helps You Improve
BEEPM gives you **real, quantifiable feedback** on your rhythm performance:
- **BPM** – measures raw speed.
- **UR (Unstable Rate)** – shows precision; lower is better.
- **Consistency %** – stability metric based on timing deviation.
- **SR (Stream Skill Rating)** – overall performance score factoring in speed, stability, and endurance.

By tracking your results over time, you can:
- Identify your **speed limits** and **fatigue thresholds**.
- Train **tempo control** and **finger independence**.
- Objectively monitor your improvement.

---

## 🧰 Building & Running

### 🪟 Requirements
- Windows OS (uses `<conio.h>` and `SetConsoleTitle`).
- C++17 or newer.
- Any compiler supporting the standard (MSVC, MinGW, etc.).

### 🏗️ Build
```bash
g++ -std=c++17 -O2 main.cpp -o beepm
