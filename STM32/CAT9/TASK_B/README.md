# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 9 (CAT 9)

**DATE:**

**NAME OF EXP:** STM32 UART-Based Command Interface for LED Control

**HARDWARE SETUP / PLATFORM:** STM32CubeMX + Keil MDK + STM32 Blue Pill Development Board (STM32F103C8T6) + PC Serial Terminal

---

## TASK 9B – String-Based Application Logic

### 1. Circuit Diagram

The experiment uses UART communication to receive commands from a PC terminal and control the onboard LED of the STM32 Blue Pill development board.

- **Development Board:** STM32 Blue Pill
- **Microcontroller IC:** STM32F103C8T6
- **UART Peripheral:** USART1
- **TX Pin:** PA9
- **RX Pin:** PA10
- **LED GPIO Pin:** PC13
- **Communication Interface:** USB-to-TTL Serial Converter
- **Terminal Software:** Serial Terminal (e.g., PuTTY / Tera Term)
- **Baud Rate:** 115200
- **Data Bits:** 8
- **Stop Bits:** 1
- **Parity:** None

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The experiment demonstrates string-based command processing using UART communication and the STM32 HAL library.

USART1 is configured in transmit and receive mode to establish serial communication between the STM32 microcontroller and a PC terminal.

The program continuously receives characters from the terminal using `HAL_UART_Receive()` and stores them in a character array named `cmd`.

When the Enter key (carriage return or newline) is pressed, the received command is terminated with a null character (`'\0'`) and compared with predefined commands using `strcmp()`.

The system supports the following commands:

- **LED ON:** Turns ON the onboard LED by setting PC13 LOW.
- **LED OFF:** Turns OFF the onboard LED by setting PC13 HIGH.
- **LED TOGGLE:** Changes the current state of the onboard LED.

After processing a command, the program sends a corresponding confirmation message to the PC terminal using `HAL_UART_Transmit()`.

The command buffer is cleared using `memset()` before accepting the next command.

**Pseudocode:**

1. Initialize the HAL library using `HAL_Init()`.
2. Configure the system clock.
3. Initialize GPIO peripherals.
4. Configure USART1 for UART communication.
5. Set the baud rate to 115200.
6. Configure 8 data bits, 1 stop bit, and no parity.
7. Configure PC13 as a push-pull output to control the onboard LED.
8. Declare a character variable `ch` and a command buffer `cmd`.
9. Initialize the command buffer using `memset()`.
10. Continuously perform the following operations:
    - Receive one character from the PC terminal using `HAL_UART_Receive()`.
    - If the received character is a carriage return or newline:
      - Terminate the command string with `'\0'`.
      - Compare the command using `strcmp()`.
      - If the command is `LED ON`, turn ON the LED.
      - If the command is `LED OFF`, turn OFF the LED.
      - If the command is `LED TOGGLE`, toggle the LED state.
      - Transmit the corresponding confirmation message.
      - Reset the buffer index to zero.
      - Clear the command buffer using `memset()`.
    - Otherwise, store the received character in the command buffer if space is available.
11. Repeat the process to accept subsequent commands.

**Code Base:**

The experiment uses the following STM32 HAL functions and C library functions:

- `HAL_Init()` – Initializes the HAL library.
- `SystemClock_Config()` – Configures the system clock.
- `MX_GPIO_Init()` – Initializes GPIO pins.
- `MX_USART1_UART_Init()` – Initializes USART1 with the required UART configuration.
- `HAL_UART_Receive()` – Receives command characters from the PC terminal.
- `HAL_UART_Transmit()` – Sends confirmation messages to the PC terminal.
- `HAL_GPIO_WritePin()` – Controls the LED ON and OFF states.
- `HAL_GPIO_TogglePin()` – Toggles the current LED state.
- `strcmp()` – Compares the received command with predefined command strings.
- `memset()` – Clears the command buffer before receiving a new command.

### 3. Inference

The experiment successfully implements a UART-based command interface for controlling the onboard LED of the STM32 Blue Pill.

The microcontroller receives string commands from the PC terminal, identifies the requested operation, and performs the corresponding LED control action.

The `LED ON` command turns the LED ON, `LED OFF` turns it OFF, and `LED TOGGLE` changes its current state.

The experiment demonstrates UART communication, string handling, command comparison, buffer management, and GPIO-based LED control using STM32 HAL functions.