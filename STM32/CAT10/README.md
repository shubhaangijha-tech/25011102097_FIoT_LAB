# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 10  (CAT 10) 

**DATE:**  
**NAME OF EXP:** STM32-Based HTTP Server Using ESP8266 and Node-RED  
**HARDWARE SETUP / PLATFORM:** STM32 Blue Pill (STM32F103C8T6) + ESP8266/Wemos Wi-Fi Module + UART Communication + Node-RED + STM32CubeIDE / Keil MDK

---

## TASK 10A – UART Communication Between STM32 and ESP8266

### 1. Circuit Diagram

The STM32 Blue Pill communicates with the ESP8266 Wi-Fi module using USART2. USART1 is used to display the responses received from the ESP8266 through a serial monitor.

**Hardware Connections:**

| STM32 Blue Pill | ESP8266 / Wemos |
|---|---|
| PA2 (USART2 TX) | RX |
| PA3 (USART2 RX) | TX |
| GND | GND |

For serial monitoring, USART1 is configured using PA9 (TX) and PA10 (RX).

## Circuit Diagram
![Circuit Diagram](circuit.png)

### 2. Core Logic / Pseudocode / Code Base

**Working Principle:**

1. Initialize the STM32 HAL library and system clock.
2. Initialize USART1 and USART2.
3. Transmit the `AT` command to the ESP8266 through USART2.
4. Receive the response from the ESP8266.
5. Forward the received response through USART1 for monitoring.
6. Repeat the process for subsequent AT commands.

**Pseudocode:**

1. Initialize HAL and configure the system clock.
2. Initialize GPIO, USART1 and USART2.
3. Send an AT command through USART2.
4. Receive the ESP8266 response.
5. Display the response through USART1.
6. Repeat for the required AT commands.

**Code Base:**

| Function / Construct | Description |
|---|---|
| `HAL_UART_Transmit()` | Transmits AT commands to the ESP8266 and forwards received responses to the serial monitor. |
| `HAL_UART_Receive()` | Receives responses from the ESP8266. |
| `MX_USART1_UART_Init()` | Initializes USART1 at 115200 baud rate for serial monitoring. |
| `MX_USART2_UART_Init()` | Initializes USART2 at 115200 baud rate for ESP8266 communication. |
| `strlen()` | Determines the length of the AT command before transmission. |
| `memset()` | Clears the receive buffer before receiving new responses. |

### 3. Inference

UART communication between the STM32 Blue Pill and ESP8266 is established using AT commands. The ESP8266 responses are received through USART2 and forwarded through USART1 for monitoring.

---

## TASK 10B – Wi-Fi Configuration of ESP8266

### 1. Core Logic / Pseudocode / Code Base

**Working Principle:**

The ESP8266 is configured in station mode so that it can connect to an existing Wi-Fi network. DHCP is enabled to obtain network configuration automatically. The `AT+CWJAP` command connects the module to the specified Wi-Fi network, and `AT+CIFSR` retrieves its assigned IP address.

**AT Commands Used:**

| AT Command | Purpose |
|---|---|
| `AT` | Checks communication with ESP8266. |
| `AT+CWMODE=1` | Configures ESP8266 in station mode. |
| `AT+CWDHCP=1,1` | Enables DHCP for the station interface. |
| `AT+CWJAP="SNUC","snu12345"` | Connects ESP8266 to the Wi-Fi network. |
| `AT+CIFSR` | Retrieves the IP address assigned to ESP8266. |

**Pseudocode:**

1. Configure ESP8266 in station mode.
2. Enable DHCP.
3. Connect to the Wi-Fi network using its SSID and password.
4. Wait for the connection to be established.
5. Retrieve and display the assigned IP address.

**Code Base:**

The AT commands are transmitted using `HAL_UART_Transmit()`. The corresponding ESP8266 responses are collected using `HAL_UART_Receive()` and displayed through USART1.

### 2. Inference

The ESP8266 successfully connects to the configured Wi-Fi network in station mode. Its assigned IP address can be used by a browser or Node-RED to access the HTTP server.

---

## TASK 10C – Configure ESP8266 as a TCP Server

### 1. Core Logic / Pseudocode / Code Base

**Working Principle:**

After connecting to Wi-Fi, the ESP8266 is configured to accept multiple TCP connections. The server is started on port 80, which is commonly used for HTTP communication.

**AT Commands Used:**

```text
AT+CIPMUX=1
AT+CIPSERVER=1,80
```

- `AT+CIPMUX=1` enables multiple TCP connections.
- `AT+CIPSERVER=1,80` starts the TCP server on port 80.

**Pseudocode:**

1. Enable multiple connection mode.
2. Start the TCP server on port 80.
3. Wait for incoming network connections.
4. Receive network data from connected clients.

**Code Base:**

The commands are transmitted through USART2 using `HAL_UART_Transmit()`. The ESP8266 responses are received using `HAL_UART_Receive()`.

### 2. Inference

The ESP8266 is configured as a TCP server listening on port 80. It can accept HTTP requests from external clients such as a web browser and Node-RED.

---

## TASK 10D – HTTP Request Reception and Response Construction

### 1. Core Logic / Pseudocode / Code Base

**Working Principle:**

When a browser sends an HTTP request to the ESP8266 server, the module forwards the received network data to the STM32 through UART using an `+IPD` notification.

The STM32 identifies the incoming request, extracts the connection ID and constructs an HTTP response containing a status line, headers and an HTML body.

**HTTP Request Reception:**

A browser sends a request similar to:

```text
GET / HTTP/1.1
Host: 192.168.1.105
```

The ESP8266 forwards the received data in an `+IPD` frame.

Example:

```text
+IPD,0,XXX:GET / HTTP/1.1
```

The STM32 searches for the `+IPD,` notification using `strstr()` and extracts the connection ID using `sscanf()`.

**HTTP Response Construction:**

The HTML body generated by the STM32 contains:

```html
<html>
<body>
<h1>STM32 IoT Node</h1>
<p>Hi sJHA from STM32</p>
<p>Captured data = ...</p>
</body>
</html>
```

The captured data value is generated using `rand()` in the program.

The HTTP response is constructed using `sprintf()`:

```text
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: <body_length>
Connection: close

<html>...</html>
```

**Sending HTTP Response Using CIPSEND:**

The transmission takes place in two steps.

**Step 1 – Send CIPSEND Command**

```c
sprintf(at_cmd, "AT+CIPSEND=%d,%d\r\n",
        link_id, total_len);
```

The STM32 sends the command through USART2 and waits for the ESP8266 `>` prompt.

**Step 2 – Transmit HTTP Response**

After receiving the prompt, the STM32 transmits the HTTP response string through USART2 using `HAL_UART_Transmit()`.

After transmission, the connection is closed using:

```text
AT+CIPCLOSE=<link_id>
```

**Pseudocode:**

1. Continuously monitor USART2 for incoming ESP8266 data.
2. Search the received data for the `+IPD,` notification.
3. Extract the TCP connection ID.
4. Generate a captured-data value.
5. Construct the HTML body.
6. Calculate the body length.
7. Construct the complete HTTP response with status line and headers.
8. Calculate the total HTTP response length.
9. Send the `AT+CIPSEND` command with connection ID and response length.
10. Wait for the ESP8266 `>` prompt.
11. Transmit the HTTP response.
12. Close the TCP connection using `AT+CIPCLOSE`.

**Code Base:**

| Function / Construct | Description |
|---|---|
| `HAL_UART_Receive()` | Monitors incoming ESP8266 data through USART2. |
| `strstr()` | Searches the received buffer for the `+IPD,` notification. |
| `sscanf()` | Extracts the TCP connection ID from the received notification. |
| `rand()` | Generates a random captured-data value. |
| `sprintf()` | Constructs the HTML body, HTTP response and AT commands. |
| `strlen()` | Determines the length of the AT commands. |
| `HAL_GetTick()` | Provides the tick count used for timeout-based prompt detection. |
| `HAL_Delay()` | Introduces a delay before closing the TCP connection. |
| `AT+CIPSEND` | Instructs the ESP8266 to prepare for transmitting the specified number of bytes. |
| `AT+CIPCLOSE` | Closes the specified TCP connection. |

### 2. Inference

The STM32 successfully processes HTTP requests forwarded by the ESP8266 through UART. It constructs an HTTP response containing the appropriate headers and HTML content and transmits it through the ESP8266 to the requesting client.

---

## TASK 10E – Node-RED as a Second HTTP Client

### 1. Node-RED Flow

Node-RED is configured to act as another HTTP client of the STM32-based HTTP server. It sends an HTTP request to the IP address assigned to the ESP8266.

**Node-RED Flow:**

```text
Inject Node
     |
     v
HTTP Request Node
     |
     v
Debug Node
```

**Node Configuration:**

| Node | Configuration |
|---|---|
| Inject | Triggers the HTTP request. |
| HTTP Request | Sends a GET request to `http://<ESP8266_IP>/`. |
| Debug | Displays the HTTP response received from the server. |

**Working Principle:**

1. The Inject node triggers the flow.
2. The HTTP Request node sends a GET request to the ESP8266 IP address on port 80.
3. The ESP8266 forwards the HTTP request to the STM32 through UART.
4. The STM32 processes the request and constructs the HTTP response.
5. The ESP8266 transmits the response to Node-RED.
6. The Debug node displays the received HTTP response.

### 2. Inference

Node-RED acts as a second HTTP client and communicates with the STM32-based HTTP server through the ESP8266. The HTTP response generated by the STM32 is received and displayed in the Node-RED Debug panel.

---

## OVERALL INFERENCE

The experiment demonstrates the implementation of an STM32-based HTTP server using the ESP8266 Wi-Fi module. The STM32 Blue Pill communicates with the ESP8266 through UART AT commands, configures it as a TCP server and receives incoming HTTP requests through `+IPD` notifications. It processes the requests, constructs HTTP responses containing HTML content and transmits them through the ESP8266 to external clients.

The HTTP server is accessible through both a web browser and Node-RED. The experiment demonstrates UART communication, Wi-Fi configuration, TCP server setup, HTTP request processing, response construction and communication between embedded devices and external clients.