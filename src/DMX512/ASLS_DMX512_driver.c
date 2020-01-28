/*
 * ASLS_DMX512_driver.c
 *
 *  Created on: 9 Jan 2020
 *      Author: root
 */

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "ASLS_DMX512_driver.h"

static uint8_t DMX_BLACKOUT_BUFFER[512] = {0};
uint16_t global_bpm_value;

static void ASLS_DMX512_init(void);
static void ASLS_DMX512_sendBreak(void);
static void ASLS_DMX512_sendData(void);
static void ASLS_DMX512_flush(void);

static ASLS_DMX512_Driver *this;

void ASLS_DMX512_init(void){

	ASLS_DMX512_flush();

	this->state = DMX_IDLE;

	this->huart.Instance 			= USART1;
	this->huart.Init.BaudRate 		= DMX_BREAK_BAUDRATE;
	this->huart.Init.WordLength 	= UART_WORDLENGTH_8B;
	this->huart.Init.StopBits 		= UART_STOPBITS_2;
	this->huart.Init.Parity 		= UART_PARITY_NONE;
	this->huart.Init.Mode 			= UART_MODE_TX_RX;
	this->huart.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
	this->huart.Init.OverSampling 	= UART_OVERSAMPLING_16;

	HAL_UART_Init(&this->huart);

}

void ASLS_DMX512_start(void){
	if(this->state == DMX_IDLE){
		this->state = DMX_BUSY;
		ASLS_DMX512_sendBreak();
	}
}

void ASLS_DMX512_stop(void){
	this->state = DMX_IDLE;
}

void ASLS_DMX512_sendBreak(void){
	switch(this->state){
		case DMX_IDLE:
			this->huart.TxCpltCallback = NULL;
			break;
		case DMX_BUSY:
			this->huart.Init.BaudRate 	= DMX_BREAK_BAUDRATE;
			this->huart.TxCpltCallback = ASLS_DMX512_sendData;
			HAL_UART_Init(&this->huart);
			HAL_UART_Transmit_IT(&this->huart, DMX_BREAK_VALUE, DMX_BREAK_SIZE);
	}
}

void ASLS_DMX512_sendData(void){
	switch(this->state){
		case DMX_IDLE:
			this->huart.TxCpltCallback = NULL;
			break;
		case DMX_BUSY:
			this->huart.Init.BaudRate = DMX_DATA_BAUDRATE;
			this->huart.TxCpltCallback = ASLS_DMX512_sendBreak;
			HAL_UART_Init(&this->huart);
			if(this->blackout == DMX_BLACKOUT_ON){
				HAL_UART_Transmit_IT(&this->huart, DMX_BLACKOUT_BUFFER, DMX_DATA_SIZE);
			}else{
				HAL_UART_Transmit_IT(&this->huart, this->buffer , DMX_DATA_SIZE);
			}
			break;
	}
}

void ASLS_DMX512_flush(void){
	for (int i=0; i<512; i++){
		this->buffer[i] = 0;
	}
}
