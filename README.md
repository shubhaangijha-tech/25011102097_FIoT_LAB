# Foundations of IoT – Laboratory Assessment Repository

## Student & Course Information
| | |
|---|---|
| **Student Name** | SHUBHAANGI JHA |
| **Registration Number** | 25011102097 |
| **Course** | 25CS2741 – Fundamentals of Internet of Things + Lab |
| **Department** | Computer Science & Engineering (IoT) |
| **Institution** | Shiv Nadar University, Chennai |
| **Repository ID** | 25011102097_FIoT_LAB |

## 1. About
This repository holds all laboratory submissions for **25CS2741 – Fundamentals of IoT + Lab**. Experiments (CATs) are organized by hardware platform: the **8051 (AT89S52)** and the **STM32 ARM Cortex** family, and the course ends with a **Mini Project**.

Each experiment folder contains:
- `README.md` – aim, circuit description, logic/pseudocode, code documentation, and inferences
- `circuit.png` – labeled circuit schematic
- Source and project files (`.c`, `.h`, Keil `.uvproj`/`.uvprojx`, STM32CubeMX `.ioc`, `.hex`)
- `TASKA`, `TASKB`, … – subfolders for each subtask, where applicable

## 2. Experiments Index

### Platform 1: 8051 Microcontroller (AT89S52)
| Folder | Date     | Experiment Title |
|--------|----------|------------------|
| CAT02 | 24/07/2026 | 8051 Hardware Board Bring-Up and Bare-Metal LED Blink |
| CAT03 | 31/07/2026 | Timer-Based LED Pattern Generator & Button Controller |
| CAT04 | 07/08/2026 | HMI using Matrix Keypad, LEDs & 7-Segment Display |
| CAT05 | 14/08/2026 | Hardware Timer Interrupts: PWM Fading & Stepper Motor |
| CAT06 | 21/08/2026 | UART Driver and Serial Debugging |
| CAT07 | 28/08/2026 | 16x2 LCD Driver Development & Keypad Navigation |

### Platform 2: STM32 ARM Cortex Microcontroller
| Folder | Date | Experiment Title |
|--------|-------------|------------------|
| CAT08 | 24/09/2026 | STM32 GPIO Bring-Up and Switch-Controlled LED |
| CAT09 | 28/09/2026 | STM32 UART Command Controlled LED |
| CAT10 | 05/10/2026 | STM32–Wemos Wi-Fi Modem Interface Using AT Commands |
| CAT11 | 12/10/2026 | HTTP Server – STM32–Wemos Wi-Fi Modem Using AT Commands |
| CAT12 | 14/10/2026 | STM32F401RE Cortex-M4: GPIO & User Input Programming |
| CAT13 | 23/10/2026 | Sensor Interfacing (BME280) – STM32 – ESP8266 HTTP |
| CAT14 | 27/10/2026 | Sensor Interfacing (BME280) – MQTT – Node Analytics |
| CAT15 | TBD | Mini Project |

## 3. Repository Structure
```text
25011102070_FIoT_LAB/
├── README.md
├── .gitignore
│
├── 8051/
│   ├── CAT02/          # each CAT folder: README.md, circuit.png,
│   ├── CAT03/          # project files, and TASKA/, TASKB/, ... subfolders
│   ├── CAT04/
│   ├── CAT05/
│   ├── CAT06/
│   └── CAT07/
│
└── STM32/
    ├── CAT08/
    ├── CAT09/
    ├── CAT10/
    ├── CAT11/
    ├── CAT12/
    ├── CAT13/
    ├── CAT14/
    └── CAT15_MiniProject/
```

## 4. Hardware & Toolchains

**8051**
- Microcontroller: Atmel AT89S52
- Clock: 11.0592 MHz crystal
- Toolchain: Keil µVision with C51 compiler

**STM32**
- Blue Pill (STM32F103C8T6): Cortex-M3, 72 MHz
- Nucleo-64 (STM32F401RBT6): Cortex-M4 with FPU, 84 MHz, on-board ST-LINK
- Modem & Sensor: ESP8266 / Wemos D1 Mini (AT firmware), Bosch BME280
- Analytics: Node-RED (HTTP and MQTT), EMQX broker