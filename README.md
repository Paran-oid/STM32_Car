# STM32 Car

A compact, modular, and extensible embedded project for building a small robotic car using an STM32 microcontroller. This repository contains firmware, hardware notes, wiring guidance, and build/flash instructions to get the vehicle driving, sensing, and responding to commands.
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
- [Acknowledgements](#acknowledgements)

---

## Project overview

STM32 Car is firmware and documentation intended to run on an STM32 microcontroller connected to motors, sensors, and optional communication peripherals (Bluetooth, RF, or Wi-Fi). The project focuses on being:

- Easy to flash and test
- Modular so features (line following, obstacle avoidance, remote control) can be toggled/extended
- Portable between different STM32 MCUs with small pin mapping changes

This repository provides a starting point: motor control (PWM + H-bridge), sensor integration (ultrasonic/IR/line), serial logging, and a simple control loop.

---

## Features

- Motor control using PWM (supports two DC motors via an H-bridge like L298N, TB6612, etc.)
- Basic obstacle detection (ultrasonic / IR)
- Line following support (IR sensor array input)
- Serial debug output over UART (useful for telemetry & configuration)
- Example command interface (via UART / Bluetooth / serial)
- Configurable PID or simple speed control loop
- Modular source layout to add sensors / new behaviors easily

---

## Hardware requirements / Bill of Materials (BoM)

Minimum:

- STM32 development board (e.g., STM32F1xx "Blue Pill", STM32F4xx Nucleo, or similar)
- 2x DC motors (with wheels)
- Motor driver (L298N, TB6612, BTS7960, etc.)
- Power source for motors (battery pack appropriate for your motors)
- 1x Ultrasonic distance sensor (HC-SR04) or IR distance sensor (optional)
- 1x IR line sensor module (optional)
- Wires, protoboard or custom PCB, 4x mounting screws

Optional:

- Bluetooth module (HC-05/06) for remote control
- 5V voltage regulator if using higher voltage battery
- Encoders for closed loop motor control
- IMU (MPU6050) for orientation control

---

## Typical wiring and pinout

Note: This project intentionally leaves MCU-specific pin definitions in a header (e.g., `pins.h` or `board.h`) so you can adapt to your board. Below is a sample mapping that is commonly used with STM32 Blue Pill (STM32F103C8T6). Replace with correct pins for your hardware.

- Motors:
  - Motor A PWM -> TIMx_CHy (e.g., PA8 / TIM1_CH1)
  - Motor A Dir -> any GPIO (e.g., PB0)
  - Motor B PWM -> TIMx_CHy (e.g., PA9 / TIM1_CH2)
  - Motor B Dir -> any GPIO (e.g., PB1)
- Ultrasonic (HC-SR04):
  - TRIG -> GPIO OUT (e.g., PA1)
  - ECHO -> GPIO IN / TIM input capture (e.g., PA2)
- Line sensors:
  - Left -> ADC or digital input (e.g., PA3)
  - Center -> ADC or digital input (e.g., PA4)
  - Right -> ADC or digital input (e.g., PA5)
- UART console:
  - TX -> UART TX (e.g., PA9)
  - RX -> UART RX (e.g., PA10)
- Bluetooth (if used):
  - HC-05 RX <- MCU TX
  - HC-05 TX -> MCU RX

Always share grounds between motor power and MCU power. Use appropriate level shifting for UART if needed.

---

## Software architecture

- /Core or /Src: main application, HAL or LL drivers, peripheral initialization
- /Inc: header files for pin mappings and configuration constants
- /Drivers: motor control layer, sensor drivers (HC-SR04, IR line)
- /App: high-level behaviors (manual control, line follow, obstacle avoidance)
- /Tools: helper scripts (optional)
- Makefile / .project: project build files (if present)

The firmware uses an application loop:
1. Read sensors (distance, line sensors)
2. Run control logic (obstacle check, line following algorithm)
3. Update motors (PWM & direction)
4. Send telemetry / log via UART
5. Repeat

The code is written to be modular; to add sensing or features, add a driver and call it from the App layer.

---

## Development environment & prerequisites

You can build this project using several toolchains. Pick one you are comfortable with.

Suggested:

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

If you use STM32CubeIDE:
1. Open STM32CubeIDE.
2. Import -> Existing Projects into Workspace -> select this repo folder.
3. Build the project (Project > Build Project).

If the repository contains a Makefile and you have arm-none-eabi-gcc:
1. Install toolchain (e.g., via package manager or ARM website).
2. From repo root:
   - make all
   - or run the provided build script

If using PlatformIO:
1. Adjust platformio.ini to match your board.
2. pio run

Note: Replace MCU/clock configuration if your board differs. The project leaves pin mappings in a header file for easy editing.

---

## Flashing / Programming

Using st-flash (stlink):
- st-flash write build/firmware.bin 0x8000000

Using OpenOCD + arm-none-eabi-gdb:
1. openocd -f interface/stlink.cfg -f target/<your-mcu>.cfg
2. arm-none-eabi-gdb build/firmware.elf
3. (gdb) target remote localhost:3333
4. (gdb) load
5. (gdb) monitor reset halt
6. (gdb) continue

Using STM32CubeProgrammer:
- Open the .elf or .bin and flash via GUI or CLI.

After flashing, open a serial console at the configured baud rate (default: 115200 8N1) to observe boot logs and telemetry.

---

## Runtime usage and controls

- On power-up, firmware initializes peripherals and prints a boot banner on UART.
- Modes (examples):
  - Manual control: receive simple commands via UART or Bluetooth (e.g., "F" forward, "B" back, "L" left, "R" right, "S" stop)
  - Line follow: reads IR sensors and steers using a proportional or PID-based controller
  - Obstacle avoidance: stops or turns when ultrasonic distance < threshold
- Commands:
  - help -> prints available commands
  - mode <manual|line|auto> -> switches operation mode
  - speed <0-100> -> sets motor speed

Check the source `app/commands.c` (or similar) for exact command strings and behavior.

---

## Configuration and customization

- Pin mapping: edit `Inc/board.h` or `pins.h` to set MCU pins used by motors, sensors, and UART.
- Tuning:
  - PID values and speed limits are in `app/config.h` or similar.
  - Adjust sensor thresholds for distance and line reading in `drivers/sensor.h`.

If you provide your exact board (MCU part number) and wiring, I can update the pin mapping and an example CubeMX project for you.

---

## Troubleshooting

- Motor does not spin:
  - Verify motor power and motor driver supply.
  - Confirm PWM pins are configured and timers enabled.
- Sensors give no reading:
  - Confirm correct wiring and logic levels.
  - Use an oscilloscope or logic analyzer on the TRIG/ECHO lines for HC-SR04.
- Firmware does not flash:
  - Check ST-LINK connection and that the target MCU isn't held in reset.
  - Ensure correct flash address and file type (ELF vs BIN).
- Serial output garbled:
  - Check UART baud rate and TTL level converter (3.3V vs 5V).

---

## Contributing

Contributions are welcome. Please:
- Open issues describing bugs or feature requests.
- Make small, focused pull requests.
- Follow the existing code style and document any hardware changes in the README or a new wiring diagram file.

If you want help adding a feature (e.g., encoder support, PID tuning tools, ROS bridge), describe the feature and I can help implement or draft issues/PRs.

---

## License

This repository is released under the MIT License. See LICENSE file for details. (If you prefer a different license, replace it and update the license header.)

---

## Acknowledgements

- STM32Cube HAL and examples
- Community drivers for sensors (HC-SR04, IR arrays)
- Motor driver datasheets (L298N, TB6612)
- Add a wiring diagram (SVG) or a fritzing diagram.

Tell me which MCU board and motor driver you're using and I'll adapt the README and code examples to match.
