/*
 * funcoes.c
 *
 *  Created on: Sep 9, 2023
 *      Author: hraup
 */

#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "stdio.h"
#include "ssd1306.h"
#include "fonts.h"

#include <stdio.h>
#include <stdlib.h>

#include "ssd1306.h"

extern I2C_HandleTypeDef hi2c2;

/*
//============================= Funcoes acelerometro =======================//

void MPU6050_Init (void)
{

	// Enviando o WHO AM I
	HAL_I2C_Mem_Read (&hi2c1, 0xD0, 0X75,1, &check, 1, 1000);

	if (check == 104)  // Se retornar do who am i
	{
		// WAKE UP
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, 0xD0, 0x6B, 1,&Data, 1, 1000);

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


void MPU6050_Read_Accel (void)
{

	HAL_I2C_Mem_Read (&hi2c1, 0xD0, 0x3B, 1, Rec_Data, 6, 1000);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

//Convertendo para unidade de medida -> Datasheet
	Ax = Accel_X_RAW/16384.0;
	Ay = Accel_Y_RAW/16384.0;
	Az = Accel_Z_RAW/16384.0;
}


void escolhe (void){


	  switch(comando){

	  case 0:
		  header();

		  SSD1306_GotoXY (35, 30);
		  SSD1306_Puts (" D4 ", &Font_11x18, 1);
		  SSD1306_UpdateScreen(); //display


		  sprintf(msg, "0. 1-4 | D4\r");
		  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);

		  break;

	  case 1:

		  header();

		  SSD1306_GotoXY (35, 30);
		  SSD1306_Puts (" D6 ", &Font_11x18, 1);
		  SSD1306_UpdateScreen(); //display

		  sprintf(msg, "1. 1-6 | D6\r");
		  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);



		  break;

	  case 2:

		    header();

		    SSD1306_GotoXY (35, 30);
		    SSD1306_Puts (" D8 ", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display


		    sprintf(msg, "2. 1-8 | D8\r");
			HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);

		  break;

	  case 3:

		  	header();

		    SSD1306_GotoXY (35, 30);
		    SSD1306_Puts (" D10 ", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    sprintf(msg, "3. 1-10 | D10\r");
			HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);


		  break;

	  case 4:

		  	header();

		    SSD1306_GotoXY (35, 30);
		    SSD1306_Puts (" D12 ", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    sprintf(msg, "4. 1-12 | D12\r");
			HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);


		  break;

	  case 5:

		    header();

		    SSD1306_GotoXY (35, 30);
		    SSD1306_Puts (" D20 ", &Font_11x18, 1);
		    SSD1306_UpdateScreen(); //display

		    sprintf(msg, "5. 1-20 | D20\r");
			HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);


		  break;

	  case 6:

		  header();

		  SSD1306_GotoXY (35, 30);
		  SSD1306_Puts ("FIM!", &Font_11x18, 1);
		  SSD1306_UpdateScreen(); //display

		  sprintf(msg, "FIM!\r");
		  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);


		  break;
	  }


}

void sorteia(void){

    switch (comando) {
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

void header(void){

	  //////HEADER//////////
	  SSD1306_GotoXY (15,0);
	  SSD1306_Puts ("Raupp & Trento", &Font_7x10, 1);
	  SSD1306_UpdateScreen(); //display

}


void exibe_sorteio(void){
	switch(numero_aleatorio) {

	    case 0:
	    case 1:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("1", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 2:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("2", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 3:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("3", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 4:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("4", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 5:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("5", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 6:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("6", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 7:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("7", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 8:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("8", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 9:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("9", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 10:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("10", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 11:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("11", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 12:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("12", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 13:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("13", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 14:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("14", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 15:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("15", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 16:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("16", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 17:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("17", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 18:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("18", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 19:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("19", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    case 20:
	        header();
	        SSD1306_GotoXY(45, 30);
	        SSD1306_Puts("20", &Font_16x26, 1);
	        SSD1306_UpdateScreen();
	        break;
	    default:
	        break;
	}



}
*/
