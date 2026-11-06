# EXPERIMENT DETAILS

- **Experiment:** Continuous Assessment / Lab Task
- **Date:** 20/07/2026
- **Name of Experiment:** Human-Machine Interface using Matrix Keypad, LEDs, and Seven-Segment Display
- **Hardware Setup / Platform:** AT89S52 Microcontroller + Keil µVision (Bare-Metal Embedded C)
- **Constraints:** Strictly NO Arduino APIs, HAL libraries, or RTOS services.

---

## TASK: Human-Machine Interface (HMI) Controller

### 1. Circuit Diagram & Hardware Description

The experiment implements a Human-Machine Interface (HMI) using a 4×3 matrix keypad as the input device, a seven-segment display for numeric feedback, and four LEDs for visual indication and pattern generation.

- **Microcontroller:** AT89S52 (8051)
- **Input Device:** 4×3 Matrix Keypad connected to Port 2.
  - Rows: P2.7, P2.6, P2.5, P2.4.
  - Columns: P2.3, P2.2, P2.1.
- **Display Output:** Seven-segment display connected to Port 0 (P0), using a predefined lookup table for digit representation.
- **LED Outputs:** Four LEDs connected to P1.1, P1.2, P1.3, and P1.4.
- **LED Configuration:** Active-LOW operation, where logic 0 turns an LED ON and logic 1 turns it OFF.
- **Power Supply:** 5V DC regulated.
- **Development Environment:** Keil µVision with Embedded C (Keil C51).

---

### 2. Core Logic / Pseudocode / Code Base

#### Working Principle

The system continuously scans the 4×3 matrix keypad to detect user input. The keypad scanning process activates one row at a time and checks the corresponding column pins to identify the pressed key.

Once a valid key press is detected, the corresponding numeric value is stored in a variable and displayed on the seven-segment display. The same key is passed to the LED control function, which executes the corresponding LED operation.

The system supports individual LED control, sequential LED movement, alternating LED patterns, binary counting, and simultaneous LED blinking.

**Functional Mapping Table (Key Press vs Output)**

| Key Press | Seven-Segment Display | LED Operation |
|:---:|:---:|:---|
| **1** | 1 | LED1 ON, remaining LEDs OFF |
| **2** | 2 | LED2 ON, remaining LEDs OFF |
| **3** | 3 | LED3 ON, remaining LEDs OFF |
| **4** | 4 | LED4 ON, remaining LEDs OFF |
| **5** | 5 | Running LED Pattern |
| **6** | 6 | Alternating LED Pattern |
| **7** | 7 | Binary Counter (0–15) |
| **8** | 8 | All LEDs Blink |
| **9** | 9 | All LEDs OFF |
| **0** | 0 | Reset System |

#### LED Operating Patterns

1. **Individual LED Control (Keys 1–4):**
   - Key 1 switches ON LED1 while the remaining LEDs stay OFF.
   - Key 2 switches ON LED2 while the remaining LEDs stay OFF.
   - Key 3 switches ON LED3 while the remaining LEDs stay OFF.
   - Key 4 switches ON LED4 while the remaining LEDs stay OFF.

2. **Running LED Pattern (Key 5):**
   - LEDs turn ON sequentially, one at a time.
   - The sequence follows LED1 → LED2 → LED3 → LED4.
   - After LED4, the sequence repeats continuously until another key is pressed.

3. **Alternating LED Pattern (Key 6):**
   - LED1 and LED3 turn ON while LED2 and LED4 remain OFF.
   - The pattern then reverses, switching ON LED2 and LED4 while turning OFF LED1 and LED3.
   - Both patterns alternate continuously until another key is pressed.

4. **Binary Counter (Key 7):**
   - A counter starts from 0 and increments up to 15.
   - The four LEDs represent a 4-bit binary number.
   - LED4 represents the least significant bit (LSB), while LED1 represents the most significant bit (MSB).
   - The counter resets to 0 after reaching 15 and continues counting until another key is pressed.

5. **All LED Blink (Key 8):**
   - All four LEDs turn ON simultaneously.
   - After a delay, all four LEDs turn OFF simultaneously.
   - This ON-OFF sequence repeats continuously until another key is pressed.

6. **All LEDs OFF (Key 9):**
   - All four LEDs are switched OFF simultaneously.
   - The system waits for the next keypad input.

7. **System Reset (Key 0):**
   - All four LEDs are switched OFF.
   - The binary counter is reset to 0.
   - The seven-segment display is updated to show 0.

#### Pseudocode

1. Include the 8051 register definition header file (`reg51.h`).
2. Define the row and column pins of the 4×3 matrix keypad using Port 2.
3. Define four LED output pins using Port 1.
4. Initialize the seven-segment lookup table containing the hexadecimal segment patterns for numeric and symbolic characters.
5. Initialize the LED outputs to OFF and display 0 on the seven-segment display.
6. Implement a software delay function for timing and keypad debouncing.
7. Implement the keypad scanning function:
   - Activate one row at a time by driving it LOW.
   - Keep the remaining rows HIGH.
   - Check the column pins for a LOW signal.
   - Identify the corresponding key based on the active row and detected column.
   - Introduce a delay for debouncing and wait for the key to be released.
   - Store the detected key and set the new-key flag.
8. Implement the seven-segment display function using the lookup table to convert the numeric value into its corresponding segment pattern.
9. Implement the LED control function using a switch-case structure to identify the pressed key.
10. Execute the corresponding operation:
    - Keys 1–4: Control individual LEDs.
    - Key 5: Execute the running LED pattern.
    - Key 6: Execute the alternating LED pattern.
    - Key 7: Execute the binary counting sequence.
    - Key 8: Blink all four LEDs.
    - Key 9: Switch OFF all LEDs.
    - Key 0: Reset the LEDs, counter, and display.
11. During continuous LED patterns, periodically scan the keypad to detect new input.
12. If another valid key is detected, terminate the currently running LED pattern and execute the newly selected operation.
13. Repeat the entire process continuously.

#### Code Base

The program is developed using Embedded C in Keil µVision for the AT89S52 microcontroller. The `reg51.h` header file provides access to the 8051 hardware registers.

- The keypad interface is implemented using individual `sbit` declarations for four row pins and three column pins on Port 2.
- Four LED pins are defined on Port 1 using `sbit` declarations. The LEDs operate using active-LOW logic.
- A software delay function is implemented using nested loops to generate timing intervals and provide keypad debouncing.
- A seven-segment lookup table named `seg[16]` stores the hexadecimal patterns required to display digits and selected hexadecimal characters.
- The `display_digit()` function sends the corresponding segment pattern to Port 0 based on the received numeric value.
- The `keypad_scan()` function performs row-by-row scanning of the matrix keypad. It identifies the pressed key by checking the column inputs corresponding to the currently activated row.
- Two variables, `key_value` and `new_key`, are used to store the detected key and indicate whether a new key has been pressed.
- The `led_control()` function uses a switch-case structure to execute the operation associated with each keypad input.
- The `running_led()` function sequentially activates the four LEDs, one at a time.
- The `alt_led()` function generates alternating LED combinations by switching between LED1 and LED3, and LED2 and LED4.
- The `bin_count()` function generates a 4-bit binary counting sequence from 0 to 15 using arithmetic operations to determine the state of each LED.
- The `blink_all()` function simultaneously switches all four LEDs ON and OFF at regular intervals.
- The continuous LED pattern functions periodically call `keypad_scan()` to check for new input. When another key is detected, the currently executing pattern terminates and control returns to the main program.
- The `main()` function initializes the LED outputs and seven-segment display, continuously scans the keypad, displays the detected digit, and passes the key value to the LED control function.

---

### 3. Inference

**Execution Behavior:** The system successfully implements a Human-Machine Interface using a 4×3 matrix keypad, four LEDs, and a seven-segment display. Different keypad inputs trigger corresponding LED operations, while the seven-segment display provides numeric feedback for the pressed key.

**Technical Learning Outcomes:** The experiment demonstrates matrix keypad interfacing, row-column scanning, software-based debouncing, seven-segment display encoding, GPIO control, bitwise and arithmetic operations, and modular Embedded C programming on the AT89S52 microcontroller.

**Peripheral Coordination:** The experiment demonstrates coordination between multiple input and output peripherals using direct register-level programming without relying on high-level libraries or operating system services.