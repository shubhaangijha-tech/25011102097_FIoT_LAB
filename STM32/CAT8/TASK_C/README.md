# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 8 (CAT 8)

**DATE:**

**NAME OF EXP:** STM32 GPIO Short Press and Long Press Detection – LED Control

**HARDWARE SETUP / PLATFORM:** STM32CubeMX + Keil MDK + STM32 Blue Pill Development Board (STM32F103C8T6)

---

## TASK 8C – Short Press vs Long Press

### 1. Circuit Diagram

The experiment uses an external push button connected to GPIO pin PA12 to control the onboard LED of the STM32 Blue Pill development board.

- **Development Board:** STM32 Blue Pill
- **Microcontroller IC:** STM32F103C8T6
- **Input GPIO Pin:** PA12
- **Output GPIO Pin:** PC13
- **Input Peripheral:** Push Button / Switch
- **Output Peripheral:** Onboard LED

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

The experiment demonstrates short-press and long-press detection using the STM32 HAL library.

The PA12 pin is configured as a digital input with an internal pull-up resistor, while PC13 is configured as a digital output to control the onboard LED.

The program uses `HAL_GetTick()` to measure how long the push button remains pressed. The press start time is recorded when the button is pressed, and the release time is used to calculate the press duration.

The button press is classified into two categories:

- **Short Press (< 1 second):**
  - If blinking mode is inactive, the LED toggles its current state.
  - If blinking mode is active, blinking stops and the LED is turned OFF.

- **Long Press (≥ 1 second):**
  - The LED enters continuous blinking mode.
  - The LED toggles every 500 milliseconds.

The program uses non-blocking time checks instead of blocking delays, allowing it to continuously monitor the button and control the LED.

**Pseudocode:**

1. Initialize the HAL library using `HAL_Init()`.
2. Configure the system clock.
3. Initialize GPIO peripherals.
4. Configure PA12 as an input with an internal pull-up resistor.
5. Configure PC13 as a push-pull output.
6. Initialize the following variables:
   - `press_time` – Stores the button press start time.
   - `blink_timer` – Stores the last LED blinking time.
   - `blinking` – Tracks whether blinking mode is active.
   - `is_pressed` – Tracks the current button press state.
7. Continuously read the state of PA12.
8. When the button is pressed:
   - Set `is_pressed` to 1.
   - Record the press start time using `HAL_GetTick()`.
9. When the button is released:
   - Calculate the press duration.
   - If the duration is less than 1000 milliseconds:
     - If blinking mode is active, stop blinking and turn OFF the LED.
     - Otherwise, toggle the LED state.
   - If the duration is 1000 milliseconds or more:
     - Enable blinking mode.
     - Initialize the blinking timer.
10. If blinking mode is active and 500 milliseconds have elapsed:
    - Toggle the LED.
    - Update the blinking timer.
11. Repeat the process continuously.

**Code Base:**

The experiment uses the following STM32 HAL functions:

- `HAL_Init()` – Initializes the HAL library.
- `SystemClock_Config()` – Configures the system clock.
- `MX_GPIO_Init()` – Initializes the GPIO pins.
- `HAL_GPIO_ReadPin()` – Reads the state of the push button connected to PA12.
- `HAL_GPIO_WritePin()` – Sets the LED state.
- `HAL_GPIO_TogglePin()` – Toggles the onboard LED state.
- `HAL_GetTick()` – Returns the system tick count in milliseconds and is used to calculate button press duration and blinking intervals.

### 3. Inference

The experiment successfully demonstrates short-press and long-press detection using a push button connected to the STM32 Blue Pill.

A short press toggles the LED state, while a long press activates continuous LED blinking at 500-millisecond intervals. When blinking mode is active, the next short press stops blinking and turns OFF the LED.

The experiment demonstrates event-based GPIO control, button press duration measurement, non-blocking time management, and LED control using STM32 HAL functions.