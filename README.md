# **🌀 BEEPM — The osu! Stream BPM Tester**

A work-in-progress rhythm analysis tool designed for osu!standard players who want to understand and improve their tapping consistency, stream control, and stamina.

## What is BEEPM?

BEEPM is a console-based stream BPM and rhythm analyzer for osu! players.

It lets you:

* Perform tap-based BPM tests to measure your streaming speed.

* Track timing intervals and compute an average BPM based on actual tap consistency.

* Visualize your performance with a simple ASCII graph showing BPM fluctuation over time.


## ⚡ Current Features

### 🧭 Core Functionality

* Real-time BPM calculation using high-precision timing between keypresses.

*  Supports typical osu! stream input (Z/X).

* Adjustable test length (20–1000 taps).

* Displays detailed timing per tap (Δt in seconds).

### 📊 BPM Graph Visualization

* After each test, BEEPM renders a text-based BPM graph in the console.

* Visually shows fluctuations in your BPM — smooth = stable tapping, spikes = inconsistency.

### 🧮 Stable Interval Math

* Calculates BPM using tap intervals for higher accuracy.

* Uses an adjustable TPB (Taps Per Beat) factor (4 by default for 1/4 osu! streams).

### 💬 Interactive Console

* Clean ASCII art main menu.

* Input validation for safe user experience.

* UTF-8 enabled for wide symbol support.


## 🧰 Building & Running

**Requirements**
* Windows OS (uses <conio.h> and SetConsoleTitle).
* C++17 or later.
* Any compiler supporting the standard (MSVC, MinGW, etc).

**Build**
> g++ -std=c++17 -O2 beepm.cpp -o beepm

Run
> ./beepm
