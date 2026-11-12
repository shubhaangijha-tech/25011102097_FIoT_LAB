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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
char rx_buf[512];
char html_body[256];
char response[512];
char at_cmd[64];
char ch;
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
		uint8_t rxbuf[128] = {0};
		const char *cmd0 = "AT\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)cmd0, strlen(cmd0), 1000);
		HAL_UART_Receive(&huart2, rxbuf, sizeof(rxbuf) - 1, 2000);
		HAL_UART_Transmit(&huart1, rxbuf, strlen((char *)rxbuf), 1000);
		HAL_Delay(1000);

		memset(rxbuf, 0, sizeof(rxbuf));
		const char *cmd1 = "AT+CWMODE=1\r\n";
		HAL_UART_Transmit(&huart2,(uint8_t*)cmd1,strlen(cmd1),100);
		HAL_UART_Receive (&huart2,rxbuf,sizeof(rxbuf)-1,1000);
		HAL_UART_Transmit(&huart1,rxbuf,strlen((char*)rxbuf),100);
		HAL_Delay(1000);

		memset(rxbuf, 0, sizeof(rxbuf));
		const char *cmd2 = "AT+CWDHCP=1,1\r\n";
		HAL_UART_Transmit(&huart2,(uint8_t*)cmd2,strlen(cmd2),100);
		HAL_UART_Receive (&huart2,rxbuf,sizeof(rxbuf)-1,1000);
		HAL_UART_Transmit(&huart1,rxbuf,strlen((char*)rxbuf),100);
		HAL_Delay(2000);

		memset(rxbuf, 0, sizeof(rxbuf));
		const char *cmd3 = "AT+CWJAP=\"SNUC\",\"snu12345\"\r\n";
		HAL_UART_Transmit(&huart2,(uint8_t*)cmd3,strlen(cmd3),100);
		HAL_UART_Receive (&huart2,rxbuf,sizeof(rxbuf)-1,10000);
		HAL_UART_Transmit(&huart1,rxbuf,strlen((char*)rxbuf),100);
		HAL_Delay(4000);

		memset(rxbuf, 0, sizeof(rxbuf));
		const char *cmd4 = "AT+CIFSR\r\n";
		HAL_UART_Transmit(&huart2,(uint8_t*)cmd4,strlen(cmd4),100);
		HAL_UART_Receive (&huart2,rxbuf,sizeof(rxbuf)-1,3000);
		HAL_UART_Transmit(&huart1,rxbuf,strlen((char*)rxbuf),100);
		HAL_Delay(4000);
		
		memset(rxbuf, 0, sizeof(rxbuf));
		const char *cmd5 = "AT+CIPMUX=1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)cmd5, strlen(cmd5), 100);
		HAL_UART_Receive(&huart2, rxbuf, sizeof(rxbuf) - 1, 1000);
		HAL_UART_Transmit(&huart1, rxbuf, strlen((char*)rxbuf), 100);
		HAL_Delay(1000);

		memset(rxbuf, 0, sizeof(rxbuf));
		const char *cmd6 = "AT+CIPSERVER=1,80\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)cmd6, strlen(cmd6), 100);
		HAL_UART_Receive(&huart2, rxbuf, sizeof(rxbuf) - 1, 1000);
		HAL_UART_Transmit(&huart1, rxbuf, strlen((char*)rxbuf), 100);
		HAL_Delay(1000);
		memset(rxbuf, 0, sizeof(rxbuf));
  while (1)
  {
     memset(rx_buf, 0, sizeof(rx_buf));

    HAL_StatusTypeDef status = HAL_UART_Receive(
        &huart2,
        (uint8_t *)rx_buf,
        sizeof(rx_buf) - 1,
        1000
    );

    if (status == HAL_OK || rx_buf[0] != '\0')
    {
        char *ipd_ptr = strstr(rx_buf, "+IPD,");

        if (ipd_ptr != NULL)
        {
            int link_id;

            if (sscanf(ipd_ptr, "+IPD,%d,", &link_id) == 1)
            {
                int captured_data = 10 + (rand() % 91);

                int body_length = sprintf(
                    html_body,
                    "<html><body>"
                    "<h1>STM32 IoT Node</h1>"
                    "<p>Hi sJHA from STM32</p>"
                    "<p>Captured data = %d</p>"
                    "</body></html>",
                    captured_data
                );

                int total_len = sprintf(
                    response,
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: %d\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "%s",
                    body_length,
                    html_body
                );

                sprintf(
                    at_cmd,
                    "AT+CIPSEND=%d,%d\r\n",
                    link_id,
                    total_len
                );

                HAL_UART_Transmit(
                    &huart2,
                    (uint8_t *)at_cmd,
                    strlen(at_cmd),
                    1000
                );

                /* Wait for the ESP8266 '>' prompt */
                uint32_t start = HAL_GetTick();
                int prompt_received = 0;

                while ((HAL_GetTick() - start) < 5000)
                {
                    if (HAL_UART_Receive(
                            &huart2,
                            (uint8_t *)&ch,
                            1,
                            100) == HAL_OK)
                    {
                        if (ch == '>')
                        {
                            prompt_received = 1;
                            break;
                        }
                    }
                }

                if (prompt_received)
                {
                    HAL_UART_Transmit(
                        &huart2,
                        (uint8_t *)response,
                        total_len,
                        3000
                    );

                    HAL_UART_Transmit(
                        &huart2,
                        (uint8_t *)response,
                        total_len,
                        1000
                    );

                    HAL_Delay(500);

                    sprintf(
                        at_cmd,
                        "AT+CIPCLOSE=%d\r\n",
                        link_id
                    );

                    HAL_UART_Transmit(
                        &huart2,
                        (uint8_t *)at_cmd,
                        strlen(at_cmd),
                        1000
                    );
                }
            }
        }
    }
  /* USER CODE END 3 */
}
	}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDefy structure.
  */
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
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
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
