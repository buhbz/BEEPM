# **🌀 BEEPM — The osu! Stream BPM Tester**

A work-in-progress rhythm analysis tool designed for osu!standard players who want to understand and improve their tapping consistency, stream control, and stamina.


## ⚡ Current Features

###🧭 Core Functionality

*Real-time BPM calculation using high-precision timing between keypresses.

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
