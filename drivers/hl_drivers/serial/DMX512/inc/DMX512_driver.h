#ifndef _DMX512_DRIVER_H_
#define _DMX512_DRIVER_H_

#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "../../../../../apps/DMX512_engine/inc/DMX512_defs.h"

#define DMX512_BREAKMAB_BAUDRATE 0x186A0U
#define DMX512_FRAME_BAUDRATE 	 0x3D090U
#define DMX512_BREAKMAB_BYTESIZE 0x00001U
#define DMX512_FRAME_BYTESIZE 	 0x00200U
#define DMX512_BREAKMAB_BYTEDATA 0x00000U

#define DEFAULT_DMX512_DRIVER {{}, {0}, DMX512_DRIVER_UNINITIALISED}

typedef enum{
	DMX512_DRIVER_OK,
	DMX512_DRIVER_THREAD_ERR,
	DMX512_DRIVER_ADDRESS_OUT_OF_BOUNDS
}DMX512_driver_err_e;

typedef enum{
	DMX512_DRIVER_INITIALISED,
	DMX512_DRIVER_UNINITIALISED
}DMX512_driver_status_e;

typedef struct{
	UART_HandleTypeDef uart;
	uint8_t value_buffer[DMX512_FRAME_BYTESIZE - 1];
	DMX512_driver_status_e status;
}DMX512_driver_s;

void DMX512_driver_init(void);
DMX512_driver_status_e DMX512_driver_get_status(void);
DMX512_driver_err_e DMX512_driver_start(void);
DMX512_driver_err_e DMX512_driver_stop(void);
DMX512_driver_err_e DMX512_driver_set_single(uint16_t address, uint8_t value);

#endif
