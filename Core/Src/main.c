/* USER CODE BEGIN Header */
/**
 * HENRIQUE RAUPP & PEDRO TRENTO
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "ssd1306.h"
#include "fonts.h"

#include <stdlib.h>
#include <time.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

////////////////////MAQUINA DE ESTADOS///////////////////////
typedef enum
{
	CONFIGURANDO = 0,
	SORTEANDO,
	EXIBINDO
} Estados_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define TOUT 100
#define TAM_MSG 200

#define TEMPO_BOT 2
#define MAX_AZ 1.99

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

///////////ACEl////////
uint8_t Rec_Data[6];
uint8_t check;
uint8_t Data;

int16_t Acell_X = 0;
int16_t Acell_Y = 0;
int16_t Acell_Z = 0;

float Ax, Ay, Az, Gx, Gy, Gz;

////////////DEB./////////
enum
{
	AGORA = 0,
	ANTES
};

uint8_t comando = 0, comando_hold = 0;
uint8_t bt1[2] = {0, 0};

uint8_t bsubida;

////////////////////MAQUINA DE ESTADOS///////////////////////

Estados_t modo = CONFIGURANDO;

/////////////////SORTEIO////////////////////
uint32_t numerogrande;
uint16_t conta;
char exibe_numero[2];

uint8_t min;
uint8_t max;
uint8_t numero_aleatorio;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//============================= Funcoes acelerometro =======================//

void MPU6050_Init(void)
{

	// Enviando o WHO AM I
	HAL_I2C_Mem_Read(&hi2c1, 0xD0, 0X75, 1, &check, 1, 1000);

	if (check == 104) // Se retornar do who am i
	{
		// WAKE UP
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, 0xD0, 0x6B, 1, &Data, 1, 1000);

		// Setando DATA RATE 1KHz
		Data = 0x07;
		HAL_I2C_Mem_Write(&hi2c1, 0xD0, 0x19, 1, &Data, 1, 1000);

		// Setando configs do acelerometro
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, 0xD0, 0X1C, 1, &Data, 1, 1000);
	}
	else // se nao retornar do who am i
		sprintf(msg, "\n\rNao deu pra ler");
	HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);
}

void MPU6050_Read_Accel(void)
{

	HAL_I2C_Mem_Read(&hi2c1, 0xD0, 0x3B, 1, Rec_Data, 6, 1000);

	Acell_X = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
	Acell_Y = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
	Acell_Z = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

	// Convertendo para unidade de medida -> Datasheet
	Ax = Acell_X / 16384.0;
	Ay = Acell_Y / 16384.0;
	Az = Acell_Z / 16384.0;
}

//============================= Funcoes exibicao =======================//

void escolhe(void)
{

	switch (comando)
	{

	case 0:
		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts(" D4 ", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;

	case 1:

		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts(" D6 ", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;

	case 2:

		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts(" D8 ", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;

	case 3:

		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts(" D10 ", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;

	case 4:

		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts(" D12 ", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;

	case 5:

		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts(" D20 ", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;

	case 6:

		header();

		SSD1306_GotoXY(35, 30);
		SSD1306_Puts("FIM!", &Font_11x18, 1);
		SSD1306_UpdateScreen(); // display

		break;
	}
}

void sorteia(void)
{

	switch (comando)
	{
	case 0:
		min = 1;
		max = 4;
		break;
	case 1:
		min = 1;
		max = 6;
		break;
	case 2:
		min = 1;
		max = 8;
		break;
	case 3:
		min = 1;
		max = 10;
		break;
	case 4:
		min = 1;
		max = 12;
		break;
	case 5:
		min = 1;
		max = 20;
		break;
	}
}

void header(void)
{

	//////HEADER//////////
	SSD1306_GotoXY(15, 0);
	SSD1306_Puts("Raupp & Trento", &Font_7x10, 1);
	SSD1306_UpdateScreen(); // display
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
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_TIM1_Init();
	MX_TIM11_Init();
	/* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim11);

	srand(time(NULL));

	MPU6050_Init();

	SSD1306_Init(); // inicia o display

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		switch (modo)
		{

		case CONFIGURANDO:

			escolhe();

			if (comando_hold >= TEMPO_BOT)
			{ // Mantem pressionado por 3 segundos ou mais
				comando = comando - 1;
				bsubida = 0;
				comando_hold = 0;
				modo = SORTEANDO;
			}

			break;

		case SORTEANDO:

			sorteia();

			SSD1306_GotoXY(28, 30);
			SSD1306_Puts("SACUDA!", &Font_11x18, 1);
			SSD1306_UpdateScreen(); // display

			srand(conta);
			numerogrande = rand();
			numero_aleatorio = numerogrande % max;

			while (numero_aleatorio == 0)
			{

				srand(conta);
				numerogrande = rand();
				numero_aleatorio = numerogrande % max;
			}

			sprintf(exibe_numero, "%d", numero_aleatorio);

			if (Az > MAX_AZ)
			{

				SSD1306_Clear();
				modo = EXIBINDO;
			}

			break;

		case EXIBINDO:

			header();

			SSD1306_GotoXY(55, 30);
			SSD1306_Puts(exibe_numero, &Font_11x18, 1);
			SSD1306_UpdateScreen(); // display

			sprintf(msg, "\r Numero sorteado = %i\n\r", numero_aleatorio);
			HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);

			if (bsubida)
			{

				comando = 0;
				comando_hold = 0;
				SSD1306_Clear();
				modo = CONFIGURANDO;
			}

			break;
		}

		/* USER CODE END WHILE */

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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Instance == TIM1)
	{

		conta++;

		if (conta > 4092)
			conta = 0;

		// Lendo o sensor

		MPU6050_Read_Accel();

		//=======================DEBOUNCE E DETECCAO DE BORDA =======================//

		bt1[AGORA] = HAL_GPIO_ReadPin(BOT1_GPIO_Port, BOT1_Pin);

		if (bt1[AGORA] == 1 && bt1[ANTES] == 0)
		{

			bsubida = 1;
			comando++;
		}

		if (bt1[AGORA] == 0 && bt1[ANTES] == 1)
		{
			// bdescida = 1;
			bsubida = 0;
		}

		bt1[ANTES] = bt1[AGORA]; /////// DETC BOT 1

		if (comando > 6)
			comando = 0;

	} //////////////FIM DEBOUNCE E DETEC.BORDA

	if (htim->Instance == TIM11)
	{

		if (bsubida)
		{
			comando_hold++;

			if (comando_hold > 3) // 3 segundos
				comando_hold = 0;
		}

		else
			comando_hold = 0;
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
