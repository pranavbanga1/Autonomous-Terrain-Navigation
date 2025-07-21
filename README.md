# 🤖 Autonomous Terrain Navigation Robot

An Arduino-based robot designed to autonomously navigate complex terrains, detect and avoid obstacles, pick up and drop packages across multi-surface environments using a combination of PID control, ultrasonic and IR sensors, and servo-driven manipulation arms.

---

## 📌 Overview

This project was developed as part of METE 4100U: Mechatronics Design. The robot was built to autonomously navigate a multi-story cardboard building, retrieve a package from one location, and deliver it to another. It uses real-time sensor feedback and motion control for terrain traversal, object avoidance, and precision-based actuation.

## ✅ Proposed Solution

The robot performs end-to-end autonomous delivery by:

- Navigating from the upper floor to the maze below using terrain-adaptive control.
- Dropping the initial package autonomously in the lower maze.
- Identifying and retrieving a second package from the maze.
- Returning to the upper lobby and dropping the second package.

Key features include:

- 🔧 **Anti-phase motor control** via Shield2AMotor library  
- 🧠 **PD wall-following algorithm** for right wall tracking  
- 👁️ **IR + Ultrasonic sensors** for dynamic obstacle detection and package recognition  
- 🤲 **Servo-operated gripper system** for pick-up/drop actions


---

## 🛠️ Hardware Components

- Arduino Due  
- N20 Geared DC Motors  
- Cytron 2A Dual Channel Motor Driver  
- TCRT5000 IR Sensors  
- HC-SR04 Ultrasonic Sensor  
- FS90R Continuous Servo (Gripper & Base Arm)  
- 7.4V LiPo Battery

### 🖼️ Robot Design Gallery

<p align="center">
  <img width="300" height="205" src="https://github.com/user-attachments/assets/4db9fbe3-f8e8-4643-a363-39949ca4a16f" alt="Chasis1"/>
  <img width="300" height="205" src="https://github.com/user-attachments/assets/6b227cae-b3c5-4538-bc6d-450cf83563f7" alt="Chasis1CAD"/>
</p>

<p align="center">
  <img width="300" height="250" src="https://github.com/user-attachments/assets/0a5b6402-4e05-4270-9ec6-15753c4499e7" alt="Chasisnn"/>
  <img width="300" height="250" src="https://github.com/user-attachments/assets/5b3e9fab-c8bc-4094-a803-d51693a33d14" alt="Chassis 5"/>
</p>

<p align="center"><i>CAD Models and Real-world Prototypes of the Autonomous Delivery Robot</i></p>

  
---

## 📁 Repository Structure

- `PhaseC_Main.ino` – Full autonomous logic  
- `motor_check.ino` – Mid-level motor test  
- `sensor_check.ino` – IR & ultrasonic sensor test  
- `NewMain/` – STL Assembly  
- `ChasisD/` – Chassis Design and CAD  
- `README.md` – Project overview and documentation

---

## Wiring Schematic
<img width="823" height="733" alt="image" src="https://github.com/user-attachments/assets/e112e33a-165f-46ce-a5cc-29306ca8e1f1" />

## 🚦 Code Walkthrough

### `PhaseC_Main.ino`

- **Initialization**  
  - Sets up motors, sensors, and servo angles.
  - Gripper starts in closed position; arm in carry mode.

- **Wall Following**  
  - Uses ultrasonic sensor on the right side.
  - Applies PD control for maintaining a 5cm distance from wall.
  - Correction computed as:  
    `correction = Kp * error + Kd * (error - prevError)`

- **Obstacle Handling**  
  - Front IR sensor checks for walls and triggers `turnRight()` or `turnLeft()`.

- **Package Handling**  
  - Pickup based on IR threshold.
  - Uses servo arm and gripper for object manipulation.

- **Delivery Logic**  
  - Pre-loaded with 1st package.
  - Drops it after 30 obstacle checks (calibrated via test).
  - Then searches for and picks the 2nd package.
  - Navigates out and drops again after another set of counts.

### `motor_check.ino`  
- Verifies direction and speed control via motor signals.

### `sensor_check.ino`  
- Displays IR and ultrasonic values for calibration and threshold tuning.

---

## 🧪 Mid-Level Testing

- ✅ **Motor Check** – Ensured both motors respond to speed and direction signals via Shield2AMotor library.
- ✅ **Sensor Check** – Verified IR trigger thresholds and ultrasonic accuracy using real-world object distances.

  <img width="1099" height="443" alt="Output" src="https://github.com/user-attachments/assets/300b5ea7-7379-439d-b124-07c93f459b66" />


---

## ⚠️ Key Implementation Notes

- Always include the `Shield2AMotor` library for anti-phase motor control:
  ```cpp
  #include "Shield2AMotor.h"
  Shield2AMotor motor(LOCKED_ANTI_PHASE); // Alternative: SIGNED_MAGNITUDE
