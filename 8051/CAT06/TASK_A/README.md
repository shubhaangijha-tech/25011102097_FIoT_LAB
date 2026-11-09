# EXPERIMENT DETAILS

- **Experiment:** Continuous Assessment 5 (CAT 5)
- **Date:**
- **Name of Experiment:** UART Initialization and Character Transmission Using 8051
- **Hardware Setup / Platform:** AT89S52 Microcontroller + Keil µVision (Bare-Metal Embedded C)
- **Crystal Frequency:** 11.0592 MHz
- **Communication Interface:** USB-to-TTL Serial Converter
- **Terminal Software:** PuTTY

---

## TASK 6A: UART Initialization and Character Transmission

### 1. Circuit Diagram & Hardware Description

The experiment implements serial communication using the built-in UART peripheral of the AT89S52 microcontroller. The UART is configured for asynchronous serial communication at 9600 baud, allowing the microcontroller to continuously transmit the character 'A' to a PC terminal through a USB-to-TTL serial converter.

- **Microcontroller:** AT89S52 (8051)
- **Communication Peripheral:** UART
- **UART Mode:** Mode 1 (8-bit UART)
- **Communication Type:** Asynchronous Serial Communication
- **Baud Rate:** 9600 bps
- **Data Bits:** 8 bits
- **Stop Bits:** 1
- **Parity:** None
- **UART Transmit Pin:** TXD (P3.1)
- **UART Receive Pin:** RXD (P3.0)
- **Timer Used:** Timer 1
- **Timer Mode:** Mode 2 (8-bit Auto-Reload)
- **Timer Reload Value:** `0xFD`
- **Serial Communication Interface:** USB-to-TTL Converter
- **PC Terminal:** PuTTY
- **Development Environment:** Keil µVision using Embedded C (Keil C51).

### 2. Core Logic / Pseudocode / Code Base

#### Working Principle

The program configures the UART peripheral of the 8051 microcontroller for serial communication using Timer 1 as the baud rate generator.

The UART is configured to operate in Mode 1, which supports 8-bit asynchronous serial communication. The microcontroller continuously transmits the character 'A' through the TXD pin.

1. **Timer Configuration:** The TMOD register is loaded with `0x20` to configure Timer 1 in Mode 2 (8-bit auto-reload mode).
2. **Baud Rate Configuration:** The TH1 register is loaded with `0xFD` to generate a baud rate of approximately 9600 bps with an 11.0592 MHz crystal.
3. **UART Configuration:** The SCON register is loaded with `0x50` to configure UART Mode 1 and enable reception.
4. **Timer Activation:** The TR1 bit is set to 1 to start Timer 1.
5. **Character Transmission:** The character 'A' is loaded into the SBUF register, initiating serial transmission.
6. **Transmission Monitoring:** The program continuously checks the TI (Transmit Interrupt) flag to determine whether transmission is complete.
7. **Flag Reset:** Once TI becomes 1, it is cleared by setting TI to 0.
8. **Continuous Transmission:** The process repeats inside an infinite loop, continuously transmitting the character 'A' to the PC terminal.

#### UART Configuration

| Parameter | Configuration |
|---|---|
| UART Mode | Mode 1 |
| Communication | Asynchronous |
| Data Bits | 8 |
| Baud Rate | 9600 bps |
| Stop Bits | 1 |
| Parity | None |
| Timer | Timer 1 |
| Timer Mode | Mode 2 |
| TH1 Value | `0xFD` |
| SCON Value | `0x50` |

#### Timer and Baud Rate Calculation

**Given:**

- Crystal Frequency = 11.0592 MHz
- Timer 1 Mode = 2 (8-bit Auto-Reload)
- TH1 = `0xFD`
- SMOD = 0 (default)

**Timer Reload Value:**

\[
TH1=0xFD=253
\]

**Number of Timer Counts:**

\[
N=256-253=3
\]

**Baud Rate Calculation:**

For UART Mode 1 with Timer 1 operating in Mode 2:

\[
Baud Rate=\frac{2^{SMOD}}{32}
\times
\frac{f_{osc}}{12(256-TH1)}
\]

Since SMOD = 0:

\[
Baud Rate=
\frac{11.0592\times10^6}{384\times3}
\]

\[
\boxed{Baud Rate=9600\ bps}
\]

Therefore, the selected timer configuration generates the required 9600 baud communication rate.

#### Pseudocode

1. Include the 8051 register definition header file (`reg51.h`).
2. Define the UART initialization function `uart_init()`.
3. Configure Timer 1 in Mode 2 using `TMOD = 0x20`.
4. Load TH1 with `0xFD` to configure the baud rate.
5. Configure the serial control register using `SCON = 0x50`.
6. Start Timer 1 by setting `TR1 = 1`.
7. Call the UART initialization function from the main function.
8. Enter an infinite loop:
   - Load the character 'A' into the SBUF register.
   - Wait until the TI flag becomes 1.
   - Clear the TI flag.
9. Repeat the process continuously to transmit characters to the PC terminal.

#### Code Base

The program is developed using Embedded C in Keil µVision for the AT89S52 microcontroller. The `reg51.h` header file provides access to the UART and timer hardware registers.

- The `uart_init()` function initializes the UART peripheral and configures Timer 1 as the baud rate generator.
- The TMOD register is assigned `0x20` to select Timer 1 in Mode 2 (8-bit auto-reload mode).
- The TH1 register is loaded with `0xFD` to generate a baud rate of 9600 bps.
- The SCON register is assigned `0x50`, selecting UART Mode 1 and enabling the receiver.
- The TR1 bit is set to 1 to start Timer 1.
- In the `main()` function, `uart_init()` is called before entering the infinite loop.
- The character 'A' is loaded into the SBUF register to initiate serial transmission.
- The TI flag is continuously monitored using a polling loop until transmission is complete.
- Once transmission finishes, TI is cleared to prepare for the next transmission.
- The entire process repeats continuously, allowing the PC terminal to receive a continuous stream of characters.

### 3. Inference

**Execution Behavior:** The UART peripheral is successfully configured for asynchronous serial communication at 9600 baud using Timer 1 in Mode 2. The microcontroller continuously transmits the character 'A' through its TXD pin.

**Timer Configuration:** Timer 1 operates in 8-bit auto-reload mode and generates the baud rate required for serial communication.

**Transmission Monitoring:** The TI flag is used to identify the completion of each transmitted character. Polling ensures that the next character is transmitted only after the previous transmission is completed.

**Technical Learning Outcomes:** The experiment demonstrates UART initialization, serial communication configuration, baud rate calculation, Timer 1 auto-reload operation, SBUF register usage, and transmit flag monitoring using Embedded C on the AT89S52 microcontroller.