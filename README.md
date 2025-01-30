# 🐍 Snake Game using Gyroscopic Input

## Overview
This project implements an **interactive snake game** on the **DE1-SoC board**, utilizing the **ADXL345 accelerometer** for **real-time tilt-based input**. The game logic is written in **C++** and runs on a **Linux OS** stored on a **MicroSD card**. The accelerometer data is retrieved using the **I2C protocol**, enabling the snake’s movement based on board orientation.

## 🎮 Features
- **Real-time gyroscope-based movement** using the ADXL345 accelerometer.
- **Embedded Linux environment** for game execution.
- **Terminal-based ASCII rendering** for game visualization.
- **Collision detection** with game boundaries and the snake's own body.
- **Dynamic food placement** for progressive gameplay.
- **Configurable game board size** (default: `60x20`).

## 🔧 Hardware & Software Components
### **Hardware:**
- **DE1-SoC Board** (Cyclone V FPGA + ARM Cortex-A9 HPS)
- **ADXL345 Accelerometer** (Tilt detection)
- **MicroSD Card** (Linux OS storage)

### **Software:**
- **C++** (Game logic implementation)
- **Linux OS** (Running on DE1-SoC board)
- **I2C Protocol** (For accelerometer communication)

## 🛠 Implementation Details
- The **HPS (Hard Processor System)** of the DE1-SoC board executes the game.
- The **ADXL345 accelerometer** measures **tilt along the X and Y axes** to determine movement.
- **I2C communication** retrieves acceleration data at **100 Hz**.
- The game logic:
  - Reads **accelerometer data** every frame.
  - Determines the **snake's movement direction** based on the **largest tilt axis**.
  - Updates the **snake’s position and checks for collisions**.
  - **Grows the snake** when food is consumed.
  - Renders the game in the **Linux terminal using ASCII characters**.

## 🖥️ Game Controls
- **Tilt the DE1-SoC board** to control the snake:
  - Tilt **left/right** → Move **left/right**.
  - Tilt **forward/backward** → Move **up/down**.

## 📂 Repository Structure
```
📦 Snake-Game-Gyro
 ┣ 📂 src/          # C++ Source code for the game
 ┣ 📂 sim/          # Simulation & debugging scripts
 ┣ 📂 docs/         # Report, images, and documentation
 ┣ 📜 README.md     # Project documentation
 ┣ 📜 LICENSE       # License information
```

## 🏃 Running the Game
### **1️⃣ Prerequisites**
- **DE1-SoC board** with **Linux installed on a MicroSD card**.
- **Cross-compilation setup** for C++ development (if developing on another machine).
- **Basic knowledge of I2C and Linux terminal commands**.

### **2️⃣ Setup**
1. **Clone the repository:**
   ```sh
   git clone https://github.com/your-repo/Snake-Game-Gyro.git
   cd Snake-Game-Gyro
   ```
2. **Transfer the code to the DE1-SoC board** via SCP or USB.
3. **Compile the game:**
   ```sh
   g++ -o snake_game src/snake_game.cpp -lwiringPi
   ```
4. **Run the game:**
   ```sh
   ./snake_game
   ```
   - Optionally, provide custom board dimensions:
     ```sh
     ./snake_game <width> <height>
     ```
   Example:
   ```sh
   ./snake_game 20 20
   ```

## 📊 Results & Analysis
- The **snake movement responds smoothly** to tilt changes.
- The **I2C protocol ensures minimal input latency** (~100 Hz data retrieval rate).
- The **terminal-based ASCII graphics** provide a functional and lightweight interface.
- **Collision detection and food consumption** work as expected.

## 🔍 Challenges & Improvements
### **Challenges Faced**
- **Initial plan** for a **graphical Space Invaders game** was **scrapped** due to Linux compatibility issues.
- **Lack of internet access** on DE1-SoC made installing dependencies difficult.
- **Kernel limitations** prevented use of modern graphical libraries.

### **Future Improvements**
- **Optimize rendering** using ANSI escape codes for better efficiency.
- **Enhance game difficulty** with different speeds and obstacles.
- **Add sound effects** via DE1-SoC's audio output.
- **Explore FPGA acceleration** for game logic execution.

## 👨‍💻 Contributors
- **Jose Cardenas**
- **David Rodriguez**

## 📜 References
- [DE1-SoC User Manual](https://www.terasic.com.tw)
- [ADXL345 Datasheet](https://www.analog.com)
- [Linux Kernel I2C Documentation](https://www.kernel.org)

---
