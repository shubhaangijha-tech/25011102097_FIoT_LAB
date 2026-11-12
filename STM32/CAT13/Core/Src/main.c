/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bme280.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ESP_RX_BUFFER_SIZE 1200
#define ESP_REQUEST_BUFFER_SIZE 1024
#define ESP_RESPONSE_BUFFER_SIZE 512

#define WIFI_SSID "SNUC"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

extern I2C_HandleTypeDef hi2c1;

struct bme280_dev dev;
struct bme280_data comp_data;
struct bme280_settings settings;

int8_t rslt = BME280_OK;

float temperature, pressure, humidity;

uint8_t sensor_data_valid = 0;
uint8_t esp_ready = 0;

char esp_ip[16] = "0.0.0.0";

char esp_response[ESP_RESPONSE_BUFFER_SIZE];

char esp_rx_buffer[ESP_RX_BUFFER_SIZE];
uint16_t esp_rx_length = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                     uint32_t len, void *intf_ptr);

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                      uint32_t len, void *intf_ptr);

void user_delay_us(uint32_t period, void *intf_ptr);

static uint8_t ESP_WaitForResponse(const char *expected,
                                   uint32_t timeout);

static uint8_t ESP_SendCommand(const char *command,
                               const char *expected,
                               uint32_t timeout);

static uint8_t ESP_Init(void);

static void ESP_ProcessIncoming(void);

static void ESP_HandleHttpRequest(int link_id,
                                  const char *request);

static void ESP_SendHTTPResponse(int link_id,
                                 const char *json,
                                 int status_code,
                                 const char *status_text);

static void BME280_ReadSensor(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* BME280 I2C Read Function */

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                     uint32_t len, void *intf_ptr)
{
    uint8_t dev_addr = *(uint8_t*)intf_ptr;

    if (HAL_I2C_Mem_Read(&hi2c1, (dev_addr << 1),
                         reg_addr, I2C_MEMADD_SIZE_8BIT,
                         reg_data, len, HAL_MAX_DELAY) == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}

/* BME280 I2C Write Function */

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                      uint32_t len, void *intf_ptr)
{
    uint8_t dev_addr = *(uint8_t*)intf_ptr;

    if (HAL_I2C_Mem_Write(&hi2c1, (dev_addr << 1),
                          reg_addr, I2C_MEMADD_SIZE_8BIT,
                          (uint8_t*)reg_data, len,
                          HAL_MAX_DELAY) == HAL_OK)
    {
        return BME280_OK;
    }

    return BME280_E_COMM_FAIL;
}

/* BME280 Delay Function */

void user_delay_us(uint32_t period, void *intf_ptr)
{
    HAL_Delay((period / 1000) + 1);
}


/* ESP8266 UART Response Handler */

static uint8_t ESP_WaitForResponse(const char *expected,
                                   uint32_t timeout)
{
    uint8_t ch;
    uint16_t i = 0;

    memset(esp_response, 0, sizeof(esp_response));

    uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < timeout)
    {
        if (HAL_UART_Receive(&huart2, &ch, 1, 10) == HAL_OK)
        {
            if (i < sizeof(esp_response) - 1)
            {
                esp_response[i++] = ch;
                esp_response[i] = '\0';
            }

            if (strstr(esp_response, "ERROR") != NULL ||
                strstr(esp_response, "FAIL") != NULL)
            {
                return 0;
            }

            if (strstr(esp_response, expected) != NULL)
            {
                return 1;
            }
        }
    }

    return 0;
}


/* ESP8266 AT Command Handler */

static uint8_t ESP_SendCommand(const char *command,
                               const char *expected,
                               uint32_t timeout)
{
    HAL_UART_Transmit(&huart2,
                      (uint8_t *)command,
                      strlen(command),
                      HAL_MAX_DELAY);

    return ESP_WaitForResponse(expected, timeout);
}


/* ESP8266 Wi-Fi and HTTP Server Initialization */

static uint8_t ESP_Init(void)
{
    char command[150];

    if (!ESP_SendCommand("AT\r\n", "OK", 3000))
    {
        return 0;
    }

    if (!ESP_SendCommand("ATE0\r\n", "OK", 3000))
    {
        return 0;
    }

    if (!ESP_SendCommand("AT+CWMODE=1\r\n", "OK", 3000))
    {
        return 0;
    }

    snprintf(command, sizeof(command),
             "AT+CWJAP=\"%s\",\"%s\"\r\n",
             WIFI_SSID, WIFI_PASSWORD);

    if (!ESP_SendCommand(command, "OK", 30000))
    {
        return 0;
    }

    if (!ESP_SendCommand("AT+CIFSR\r\n", "OK", 5000))
    {
        return 0;
    }

    char *ip_start = strstr(esp_response, "+CIFSR:STAIP,\"");

    if (ip_start != NULL)
    {
        ip_start += strlen("+CIFSR:STAIP,\"");

        char *ip_end = strchr(ip_start, '"');

        if (ip_end != NULL)
        {
            size_t ip_length = ip_end - ip_start;

            if (ip_length < sizeof(esp_ip))
            {
                memcpy(esp_ip, ip_start, ip_length);
                esp_ip[ip_length] = '\0';
            }
        }
    }

    if (!ESP_SendCommand("AT+CIPMUX=1\r\n", "OK", 3000))
    {
        return 0;
    }

    if (!ESP_SendCommand("AT+CIPSERVER=1,80\r\n", "OK", 3000))
    {
        return 0;
    }

    return 1;
}


/* BME280 Sensor Reading Function */

static void BME280_ReadSensor(void)
{
    rslt = bme280_get_sensor_data(BME280_ALL,
                                  &comp_data,
                                  &dev);

    if (rslt == BME280_OK)
    {
        temperature = comp_data.temperature;

        pressure = comp_data.pressure / 100.0f;

        humidity = comp_data.humidity;

        sensor_data_valid = 1;
    }
    else
    {
        sensor_data_valid = 0;
    }
}


/* HTTP Response Transmission */

static void ESP_SendHTTPResponse(int link_id,
                                 const char *json,
                                 int status_code,
                                 const char *status_text)
{
    char http_response[512];
    char command[50];

    int content_length = strlen(json);

    int response_length = snprintf(
        http_response,
        sizeof(http_response),

        "HTTP/1.1 %d %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",

        status_code,
        status_text,
        content_length,
        json
    );

    if (response_length <= 0 ||
        response_length >= sizeof(http_response))
    {
        return;
    }

    snprintf(command, sizeof(command),
             "AT+CIPSEND=%d,%d\r\n",
             link_id,
             response_length);

    if (!ESP_SendCommand(command, ">", 5000))
    {
        return;
    }

    HAL_UART_Transmit(&huart2,
                      (uint8_t *)http_response,
                      response_length,
                      5000);

    if (ESP_WaitForResponse("SEND OK", 5000))
    {
        snprintf(command, sizeof(command),
                 "AT+CIPCLOSE=%d\r\n",
                 link_id);

        ESP_SendCommand(command, "OK", 2000);
    }
}


/* HTTP Request Processing */

static void ESP_HandleHttpRequest(int link_id,
                                  const char *request)
{
    char json[200];

    int status_code = 200;
    const char *status_text = "OK";

    if (strncmp(request, "GET ", 4) != 0)
    {
        snprintf(json, sizeof(json),
                 "{\"status\":\"error\",\"message\":\"Method not supported\"}");

        status_code = 405;
        status_text = "Method Not Allowed";
    }
    else if (!sensor_data_valid)
    {
        snprintf(json, sizeof(json),
                 "{\"status\":\"error\",\"message\":\"Sensor data unavailable\"}");

        status_code = 503;
        status_text = "Service Unavailable";
    }
    else
    {
        snprintf(json, sizeof(json),
                 "{\"status\":\"success\","
                 "\"temperature\":%.2f,"
                 "\"humidity\":%.2f,"
                 "\"pressure\":%.2f}",

                 temperature,
                 humidity,
                 pressure);
    }

    ESP_SendHTTPResponse(link_id,
                         json,
                         status_code,
                         status_text);
}


/* ESP8266 Incoming HTTP Request Processing */

static void ESP_ProcessIncoming(void)
{
    uint8_t ch;

    if (HAL_UART_Receive(&huart2, &ch, 1, 2) == HAL_OK)
    {
        if (esp_rx_length < ESP_RX_BUFFER_SIZE - 1)
        {
            esp_rx_buffer[esp_rx_length++] = ch;

            esp_rx_buffer[esp_rx_length] = '\0';
        }
        else
        {
            esp_rx_length = 0;

            memset(esp_rx_buffer, 0,
                   sizeof(esp_rx_buffer));
        }
    }

    char *ipd = strstr(esp_rx_buffer, "+IPD,");

    if (ipd == NULL)
    {
        if (esp_rx_length > 5)
        {
            memmove(esp_rx_buffer,
                    esp_rx_buffer + esp_rx_length - 5,
                    5);

            esp_rx_length = 5;

            esp_rx_buffer[5] = '\0';
        }

        return;
    }

    char *comma = strchr(ipd + 5, ',');

    if (comma == NULL)
    {
        return;
    }

    char *colon = strchr(comma + 1, ':');

    if (colon == NULL)
    {
        return;
    }

    int link_id = atoi(ipd + 5);

    int payload_length = atoi(comma + 1);

    if (payload_length <= 0 ||
        payload_length >= ESP_REQUEST_BUFFER_SIZE)
    {
        esp_rx_length = 0;

        memset(esp_rx_buffer, 0,
               sizeof(esp_rx_buffer));

        return;
    }

    uint16_t header_length =
        (uint16_t)(colon - esp_rx_buffer + 1);

    uint16_t total_length =
        header_length + payload_length;

    if (esp_rx_length < total_length)
    {
        return;
    }

    char request[ESP_REQUEST_BUFFER_SIZE];

    memcpy(request,
           colon + 1,
           payload_length);

    request[payload_length] = '\0';

    uint16_t remaining =
        esp_rx_length - total_length;

    memmove(esp_rx_buffer,
            esp_rx_buffer + total_length,
            remaining);

    esp_rx_length = remaining;

    esp_rx_buffer[esp_rx_length] = '\0';

    ESP_HandleHttpRequest(link_id, request);
}

/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  static uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

  /* Set up the API configuration function pointers */

  dev.intf_ptr = &dev_addr;

  dev.intf = BME280_I2C_INTF;

  dev.read = user_i2c_read;

  dev.write = user_i2c_write;

  dev.delay_us = user_delay_us;

  /* Initialize the BME280 sensor */

  rslt = bme280_init(&dev);

  /* Configure sensor settings */

  settings.osr_h = BME280_OVERSAMPLING_1X;

  settings.osr_p = BME280_OVERSAMPLING_1X;

  settings.osr_t = BME280_OVERSAMPLING_1X;

  settings.filter = BME280_FILTER_COEFF_OFF;

  rslt = bme280_set_sensor_settings(
      BME280_SEL_OSR_PRESS |
      BME280_SEL_OSR_TEMP |
      BME280_SEL_OSR_HUM |
      BME280_SEL_FILTER,
      &settings,
      &dev
  );

  /* Set power mode to normal */

  rslt = bme280_set_sensor_mode(
      BME280_POWERMODE_NORMAL,
      &dev
  );

  /* Take the first sensor reading */

  BME280_ReadSensor();

  /* Initialize ESP8266 Wi-Fi and HTTP server */

  esp_ready = ESP_Init();

  uint32_t last_sensor_read = HAL_GetTick();

  uint32_t last_esp_retry = HAL_GetTick();

  /* USER CODE END 2 */


  /* Infinite loop */

  /* USER CODE BEGIN WHILE */

  while (1)
  {

    /* Read BME280 every one second */

    if (HAL_GetTick() - last_sensor_read >= 1000)
    {
        last_sensor_read = HAL_GetTick();

        BME280_ReadSensor();
    }

    /* Retry ESP8266 initialization if connection fails */

    if (!esp_ready &&
        HAL_GetTick() - last_esp_retry >= 10000)
    {
        last_esp_retry = HAL_GetTick();

        esp_ready = ESP_Init();
    }

    /* Process incoming HTTP requests */

    if (esp_ready)
    {
        ESP_ProcessIncoming();
    }

    /* USER CODE BEGIN 3 */

  }

  /* USER CODE END 3 */

}


/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitTypeDef RCC_ClkInitStruct2 = {0};

  RCC_ClkInitStruct2.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct2.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct2.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct2.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct2.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct2,
                          FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;

  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */

static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;

  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


/**
  * @brief This function is executed in case of error occurrence.
  * @retval None
  */

void Error_Handler(void)
{

  __disable_irq();

  while (1)
  {
  }

}


#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}

#endif