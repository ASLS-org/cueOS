/*
 * DMX512.h
 *
 *  Created on: 24 Jan 2020
 *      Author: root
 */

#ifndef ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_DRIVER_H_
#define ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_DRIVER_H_

#include "stm32f4xx_hal.h"

#define DMX_BREAK_BAUDRATE	80000
#define DMX_DATA_BAUDRATE	250000
#define DMX_BREAK_SIZE		1
#define DMX_DATA_SIZE		512
#define DMX_BREAK_VALUE		0x00

typedef enum{
	DMX_IDLE,
	DMX_BUSY,
}ASLS_DMX512_State;

typedef enum{
	DMX_BLACKOUT_ON,
	DMX_BLACKOUT_OFF
}ASLS_DMX512_Blackout;

typedef struct{
	UART_HandleTypeDef huart;
	uint8_t buffer[DMX_DATA_SIZE];
	ASLS_DMX512_State state;
	ASLS_DMX512_Blackout blackout;
}ASLS_DMX512_Driver;

#endif /* ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_DRIVER_H_ */
