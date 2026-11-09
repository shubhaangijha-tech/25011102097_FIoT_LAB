# EXPERIMENT DETAILS

- **Experiment:** Continuous Assessment 5 (CAT 5)
- **Date:**
- **Name of Experiment:** Timer 0 Configuration for Periodic LED Toggling
- **Hardware Setup / Platform:** AT89S52 Microcontroller + Keil µVision (Bare-Metal Embedded C)
- **Crystal Frequency:** 11.0592 MHz

---

## TASK 5A: Timer 0 Configuration for Periodic LED Toggling

### 1. Circuit Diagram & Hardware Description

The experiment implements a timer-based LED control system using Timer 0 of the AT89S52 microcontroller. Timer 0 is configured in 16-bit mode to generate periodic overflow events. These overflow events are monitored using the Timer 0 overflow flag to maintain a software counter and periodically toggle an LED.

- **Microcontroller:** AT89S52 (8051)
- **Timer Used:** Timer 0
- **Timer Mode:** Mode 1 (16-bit Timer Mode)
- **Clock Frequency:** 11.0592 MHz crystal oscillator.
- **Machine Cycle Frequency:** 921.6 kHz (11.0592 MHz / 12).
- **Machine Cycle Duration:** Approximately 1.085 µs.
- **LED Output:** LED connected to Port 1, Pin P1.1.
- **Timer Registers:** TMOD, TH0, TL0, TR0, and TF0.
- **Power Supply:** 5V DC regulated.
- **Development Environment:** Keil µVision using Embedded C (Keil C51).

---

### 2. Core Logic / Pseudocode / Code Base

#### Working Principle

The experiment uses Timer 0 in Mode 1 (16-bit timer mode) to generate periodic overflow events. The timer counts upward from a predefined initial value until it reaches its maximum value of 65535 and overflows to zero.

The overflow flag (TF0) indicates when the timer overflow occurs. The program continuously monitors this flag and performs the following operations whenever an overflow is detected:

1. **Timer Configuration:** The TMOD register is configured with `0x01` to select Timer 0 in Mode 1 (16-bit mode).
2. **Timer Initialization:** The TH0 and TL0 registers are loaded with the initial timer value `0xFA9C`.
3. **Timer Activation:** The TR0 bit is set to 1 to start Timer 0.
4. **Overflow Detection:** The program continuously checks the TF0 flag to detect timer overflow.
5. **Overflow Handling:** Once overflow is detected, TF0 is cleared and a software counter is incremented.
6. **Timer Reloading:** The initial timer values are loaded again into TH0 and TL0 to generate the next timing interval.
7. **LED Toggling:** When the software counter exceeds 533, the LED output is complemented, changing its logic state. The counter is then reset to zero.
8. **Continuous Operation:** The process repeats indefinitely, producing periodic LED toggling.

#### Timer Calculation

For a standard 8051 microcontroller operating with a 12-clock machine cycle:

**Given:**

- Crystal Frequency = 11.0592 MHz
- Timer 0 Mode = 1 (16-bit)
- Machine Cycle = 12 clock cycles

**Machine Cycle Duration:**

\[
T_{machine}=\frac{12}{11.0592\times10^6}
\]

\[
T_{machine}\approx1.085\ \mu s
\]

**Timer Initial Value:**

The code uses the preload value:

\[
TH0 = FA_H,\quad TL0 = 9C_H
\]

\[
FA9C_H = 64156
\]

**Number of Timer Counts:**

\[
N=65536-64156
\]

\[
N=1380
\]

**Timer Overflow Duration:**

\[
T=1380\times1.085\ \mu s
\]

\[
T\approx1.497\ ms
\]

The software counter increments after every overflow. Since the condition is `count > 533`, the LED toggles after 534 timer overflows.

\[
T_{toggle}=534\times1.497\ ms
\]

\[
T_{toggle}\approx799.6\ ms
\]

Therefore, the LED output changes state approximately every 0.8 seconds with the given timer preload and counter condition.

#### Pseudocode

1. Include the 8051 register definition header file (`reg51.h`).
2. Define the LED output pin as P1.1.
3. Initialize a software counter to zero.
4. Configure Timer 0 using the TMOD register in Mode 1 (16-bit mode).
5. Load the initial timer values into TH0 and TL0.
6. Start Timer 0 by setting TR0 to 1.
7. Initialize the LED output.
8. Enter an infinite loop:
   - Check whether the TF0 overflow flag is set.
   - If an overflow occurs:
     - Clear TF0.
     - Increment the software counter.
     - Check whether the counter exceeds 533.
     - If the condition is satisfied:
       - Toggle the LED using the complement operation.
       - Reset the software counter to zero.
     - Reload TH0 and TL0 with the initial timer values.
9. Repeat the process continuously to generate periodic LED output changes.

#### Code Base

The program is developed using Embedded C in Keil µVision for the AT89S52 microcontroller. The `reg51.h` header file provides access to the 8051 timer and GPIO registers.

- The LED is connected to P1.1 and controlled using an `sbit` declaration.
- An integer variable named `count` maintains the number of timer overflow events.
- The TMOD register is configured with `0x01`, selecting Timer 0 in Mode 1, which provides a 16-bit counting operation.
- The TH0 and TL0 registers are initialized with `0xFA` and `0x9C`, respectively, to establish the starting timer count.
- The TR0 bit is set to 1 to activate Timer 0.
- The program continuously monitors the TF0 flag to detect timer overflow.
- When TF0 becomes 1, the program clears the overflow flag and increments the software counter.
- A conditional statement checks whether the counter exceeds 533. When this condition is satisfied, the LED output is complemented using the `~` operator and the counter is reset to zero.
- The timer registers are reloaded with their initial values after every overflow to maintain a repeated timing interval.
- The entire process executes continuously inside an infinite loop, producing periodic changes in the LED output.

---

### 3. Inference

**Execution Behavior:** Timer 0 is successfully configured in 16-bit Mode 1 to generate repeated overflow events. The overflow flag is monitored, and a software counter is used to determine when the LED connected to P1.1 should change its output state.

**Technical Learning Outcomes:** The experiment demonstrates 8051 timer configuration, timer register initialization, overflow detection, software counter implementation, and GPIO control using Embedded C. It also provides an understanding of machine cycle calculations and timer preload values for generating specific time intervals.

**Timer and Software Coordination:** The experiment demonstrates how hardware timer overflow events can be combined with software counters to generate longer timing intervals and control external output devices without using high-level libraries.