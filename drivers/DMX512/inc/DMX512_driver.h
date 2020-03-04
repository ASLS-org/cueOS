#ifndef _DMX512_DRIVER_H_
#define _DMX512_DRIVER_H_

#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "DMX512_defs.h"

#define DMX512_BREAKMAB_BAUDRATE 0x186A0U
#define DMX512_FRAME_BAUDRATE 	 0x3D090U
#define DMX512_BREAKMAB_BYTESIZE 0x00001U
#define DMX512_FRAME_BYTESIZE 	 0x00200U
#define DMX512_BREAKMAB_BYTEDATA 0x00000U

#define DEFAULT_DMX512_DRIVER {{}, DMX512_DRIVER_IDLE, {0}}

typedef enum{
	DMX512_DRIVER_OK,
	DMX512_DRIVER_THREAD_ERR
}DMX512_driver_err_e;

typedef enum{
	DMX512_DRIVER_IDLE,
	DMX512_DRIVER_BUSY
}DMX512_driver_status_e;

typedef struct{
	UART_HandleTypeDef uart;
	DMX512_driver_status_e status;
	uint8_t value_buffer[DMX512_FRAME_BYTESIZE - 1];
	osThreadId threadId;
}DMX512_driver_s;

DMX512_driver_s DMX512_driver_init(void);
DMX512_driver_err_e DMX512_driver_start(void);
DMX512_driver_err_e DMX512_driver_stop(void);
DMX512_driver_err_e DMX512_driver_set_single(uint16_t address, uint8_t value);
void DMX512_driver_set_multiple(uint8_t *preset_values); //TODO: change type so fixtures with 255+ channels won't overflow
DMX512_driver_err_e DMX512_driver_set_continuous(uint16_t address_start, uint16_t address_stop, uint8_t *values);

#endif
