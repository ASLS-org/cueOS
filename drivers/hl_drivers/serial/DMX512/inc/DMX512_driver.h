#ifndef _DMX512_DRIVER_H_
#define _DMX512_DRIVER_H_

#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "DMX512_defs.h"

#define DMX512_BREAKMAB_BAUDRATE 0x186A0U		/** @ingroup DMX512_driver @def DMX512_BREAKMAB_BAUDRATE @brief USART Baudrate for break-mab transmission */
#define DMX512_FRAME_BAUDRATE 	 0x3D090U		/** @ingroup DMX512_driver @def DMX512_FRAME_BAUDRATE @brief USART Baudrate for DMX512 frame transmission */
#define DMX512_BREAKMAB_BYTESIZE 0x00001U		/** @ingroup DMX512_driver @def DMX512_BREAKMAB_BYTESIZE @brief size of a break-mab sequence in bytes */
#define DMX512_FRAME_BYTESIZE 	 0x00200U		/** @ingroup DMX512_driver @def DMX512_FRAME_BYTESIZE @brief size of DMX512 fram in bytes*/
#define DMX512_BREAKMAB_BYTEDATA 0x00000U		/** @ingroup DMX512_driver @def DMX512_BREAKMAB_BYTEDATA @brief break-mab sequence data*/


/**
 * @ingroup DMX512_driver
 * @def DEFAULT_DMX512_DRIVER
 * @brief DMX512 driver object default values
 *
 * Used during initialisation of a DMX512 driver instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define DEFAULT_DMX512_DRIVER {{}, {0}, DMX512_DRIVER_UNINITIALISED}

/**
 * @ingroup DMX512_driver
 * @enum DMX512_driver_err_e
 * @brief QLSF  DMX512 driver error constants
 *
 * Series of pre-defined constant error values to be used
 * throughout the whole DMX512 driver life-cycle.
 */
typedef enum{
	DMX512_DRIVER_OK,											/*< No error*/
	DMX512_DRIVER_THREAD_ERR,									/*< The driver thread crashed*/
	DMX512_DRIVER_ADDRESS_OUT_OF_BOUNDS							/*< A provided address was outside of the unverse's 0-512 addresses interval*/
}DMX512_driver_err_e;

/**
 * @ingroup DMX512_driver
 * @enum DMX512_driver_status_e
 * @brief Status of the DMX512 driver instance
 *
 * Defines whether or not a DMX512 driver instance has been correctly initialised.
 */
typedef enum{
	DMX512_DRIVER_INITIALISED,									/*< Driver is initialised*/
	DMX512_DRIVER_UNINITIALISED									/*< Driver is uninitialised*/
}DMX512_driver_status_e;

/**
 * @ingroup DMX512_driver
 * @struct DMX512_driver_s
 * @brief Defines a DMX512 driver instance
 *
 * DMX512 is a standard for digital communication widely used to control
 * stage equipment. data is transmitted over differential pair using RS-485 standard.
 * each DMX512 frame consists of a set of 512 ordered channel 8bit values.
 */
typedef struct{
	UART_HandleTypeDef uart;									/*< UART handle*/
	uint8_t value_buffer[DMX512_FRAME_BYTESIZE - 1];			/*< DMX512 frame channel values buffer*/
	DMX512_driver_status_e status;								/*< Current status of the driver @see DMX512_driver_status_e*/
}DMX512_driver_s;

void DMX512_driver_init(void);
DMX512_driver_status_e DMX512_driver_get_status(void);
DMX512_driver_err_e DMX512_driver_start(void);
DMX512_driver_err_e DMX512_driver_stop(void);
DMX512_driver_err_e DMX512_driver_set_single(uint16_t address, uint8_t value);

#endif
