# STM32 Car

A compact, modular, and extensible embedded project for building a small robotic car using an STM32 microcontroller. This repository contains firmware, h

ardware notes, wiring guidance, and build/flash instructions to get the vehicle driving, sensing, and responding to commands.

https://github.com/user-attachments/assets/7eb9cc81-aa76-42e0-a95a-5bcbff496e82

---

## Table of contents

- [Project overview](#project-overview)
- [Features](#features)
- [Hardware requirements / Bill of Materials (BoM)](#hardware-requirements--bill-of-materials-bom)
- [Typical wiring and pinout](#typical-wiring-and-pinout)
- [Software architecture](#software-architecture)
- [Development environment & prerequisites](#development-environment--prerequisites)
- [Build instructions](#build-instructions)
- [Flashing / Programming](#flashing--programming)
- [Runtime usage and controls](#runtime-usage-and-controls)
- [Configuration and customization](#configuration-and-customization)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## Project overview

STM32 Car is firmware and documentation intended to run on an STM32 microcontroller connected to motors, sensors. The project focuses on being:

- Easy to flash and test
- Modular so features (remote control) can be toggled/extended
- Portable between different STM32 MCUs with small pin mapping changes

This repository provides a starting point: motor control (PWM + H-bridge), sensor integration (ultrasonic/IR).

---

## Features

- Motor control using PWM (supports two DC motors via an H-bridge like L298N, TB6612, etc.)
- Basic obstacle detection (ultrasonic / IR)
- Command reading from IR Remote using an IR Receiver
- Configurable PID/simple speed control loop
- Modular source layout to add sensors / new behaviors easily

---

## Hardware requirements / Bill of Materials (BoM)

Minimum:

- STM32 development board (e.g., STM32L4xx, STM32F1xx "Blue Pill", STM32F4xx Nucleo, or similar)
- 4x DC motors (with wheels)
- Motor driver (L298N)
- Power source for motors (battery pack appropriate for your motors)
- 1x Ultrasonic distance sensor (HC-SR04) or IR distance sensor (optional)
- 1x IR Remote + 1x IR Receiver
- Wires, protoboard and 16x mounting screws (once I have time I will share them in detail here)

---

## Typical wiring and pinout

Always share grounds between motor power and MCU power. Use appropriate level shifting for UART if needed.

---

## Software architecture

- /Core or /Src: main application, HAL or LL drivers, peripheral initialization
- /Inc: header files for pin mappings and configuration constants
- /Drivers: motor control layer, sensor drivers (HC-SR04, IR line)
- /App: high-level behaviors (manual control, line follow, obstacle avoidance)
- Makefile / .project: project build files (if present)

The firmware uses an application loop:
1. Read sensors (distance)
2. Run control logic (obstacle check)
3. Update motors (PWM & direction)
4. Repeat

The code is written to be modular; to add sensing or features, add a driver and call it from the App layer.

---

## Development environment & prerequisites

You can build this project using several toolchains. Pick one you are comfortable with.

Suggested:

- Visual Studio Code (make sure you have the extension STM32 for vscode already installed)
OR
- STM32CubeIDE (import existing project or create a new one and copy source)
  - STM32CubeMX may help generate clock/peripheral initialization

- Alternatively:
  - arm-none-eabi-gcc toolchain
  - Makefile (if included)
  - PlatformIO (with the appropriate board/platform)
- Flashing tools:
  - ST-LINK (STLink Utility, st-flash from OpenOCD/stlink)
  - OpenOCD + GDB
  - DFU (if using a DFU-capable bootloader)

Useful software:
- Python (optional for scripts)
- minicom / picocom / Tera Term / PuTTY (for serial console)
- Git

---

## Build instructions

If using VScode:
1. Make sure `STM32CubeIde for Visual Studio Code` and `CMake Tools` is installed on your IDE
2. optional: select your target (usually target is selected automatically)
3. ctrl + shift + p   and then enter `Cmake: Build`

If you use STM32CubeIDE:
1. Open STM32CubeIDE.
2. Import -> Existing Projects into Workspace -> select this repo folder.
3. Build the project (Project > Build Project).

Note: Replace MCU/clock configuration if your board differs. The project leaves pin mappings in a header file for easy editing.

---

## Flashing / Programming

- Make sure `STM32CubeIde for Visual Studio Code` is installed in your VScode
- Once installed you can start debugging using stlink (through VScode). 

---

## Runtime usage and controls

- On power-up, firmware initializes peripherals and generates a small buzzer sound sequence (in main_task task in `App/tasks.hpp`).
- Implemented Modes so far:
  - Manual control: receive simple commands via IRRemote (all mapped accordingly in `Drivers/drive_sys.cpp`

---

## Configuration and customization

- Pin mapping: edit `.mxproject` file to set MCU pins used by motors, sensors, and UART through stm32cubemx.
- Tuning:
  - PID values and speed limits are in `App/drive_sys.hpp` or similar.
  - Adjust sensor thresholds for distance and line reading in `Drivers/HCSR04.hpp`.

---

## Troubleshooting

- Motor does not spin:
  - Verify motor power and motor driver supply.
  - Confirm PWM pins are configured and timers enabled AND CLOCK FREQUENCY IS 80MHz.
- Sensors give no reading:
  - Confirm correct wiring and logic levels.
  - Use an oscilloscope or logic analyzer on the TRIG/ECHO lines for HC-SR04.
- Firmware does not flash:
  - Check ST-LINK connection and that the target MCU isn't held in reset.
  - Ensure correct flash address and file type (ELF vs BIN).

---

## Contributing

Contributions are welcome. Please:
- Open issues describing bugs or feature requests.
- Make small, focused pull requests.
- Follow the existing code style and document any hardware changes in the README or a new wiring diagram file.

---

## License

This repository is released under the MIT License. See LICENSE file for details. (If you prefer a different license, replace it and update the license header.)

---


