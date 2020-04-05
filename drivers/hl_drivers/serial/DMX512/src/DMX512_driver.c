/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "DMX512_driver.h"
#include "cmsis_os.h"


/**============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static DMX512_driver_s this = DEFAULT_DMX512_DRIVER;
static osThreadId_t DMX512_driver_ThreadId = NULL;


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @ingroup DMX512_driver
 * @fn _DMX512_driver_send_frame
 * @brief Sends DMX frames using values contained within the driver's value buffer over RS485
 * Blocking UART API is used to ensure sanity of the serial output (Avoid packet collision)
 *
 * @param this DMX512 driver instance
 */
static void _DMX512_driver_send_frame(void){
	this.uart.Init.BaudRate  = DMX512_FRAME_BAUDRATE;
	this.uart.Init.StopBits  = UART_STOPBITS_2;
	HAL_UART_Init(&this.uart);
	HAL_UART_Transmit(&this.uart, this.value_buffer, DMX512_FRAME_BYTESIZE, HAL_MAX_DELAY);
}

/**
 * @ingroup DMX512_driver
 * @fn _DMX512_driver_send_breakmab
 * @brief Sends DMX BREAK-MAB sequence by slowing down UART baudrate from 115200b/s to 100000b/s:
 * BREAK timing typ = 88us | MAB timing	typ = 8us
 * BREAK = startbit + BREAK_DATA = 9 * 10 = 90us
 * MAB 	 = stopbit 				 = 1 * 10 = 10us
 * Blocking UART API is used to ensure sanity of the serial output (Avoid packet collision)
 *
 * @param this DMX512 driver instance
 */
static void _DMX512_driver_send_breakmab(void){
	this.uart.Init.BaudRate  = DMX512_BREAKMAB_BAUDRATE;
	this.uart.Init.StopBits  = UART_STOPBITS_1;
	HAL_UART_Init(&this.uart);
	HAL_UART_Transmit(&this.uart, DMX512_BREAKMAB_BYTEDATA, DMX512_BREAKMAB_BYTESIZE, 0x0001);
}

/**
 * @ingroup DMX512_driver
 * @fn _DMX512_driver_thread
 * @brief DMX512 driver thread
 *
 * Sequentially sends a DMX break sequence and DMX Data
 * IMPORTANT: for this to work, successive calls need to be blocking to prevent packet collisions.
 */
static void _DMX512_driver_thread(void *arg){
	for(;;){
		_DMX512_driver_send_breakmab();
		_DMX512_driver_send_frame();
		osDelay(23);
	}
}

/**
 * @ingroup DMX512_driver
 * @fn _DMX512_driver_address_check
 * @brief Checks if the provided address is contained within permited DMX address bounds
 *
 * @param address the address to be be checked
 * @return uint8_t returns 1 if provided address is correct 0 otherwise.
 */
uint8_t _DMX512_driver_address_check(uint16_t address){
	return (address < DMX512_CHANNEL_ADDRESS_MIN || address > DMX512_CHANNEL_ADDRESS_MAX) ? 0 : 1;
}

/**
 * @ingroup DMX512_driver
 * @fn _DMX512_driver_GPIO_init
 * @brief Initialises GPIOB pins 6 and 7 to work in UART Mode
 *	 	  GPIO settings are generated to enable USART1
 */
void _DMX512_driver_GPIO_init(void){

	__USART1_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin 		 = GPIO_PIN_6;
    GPIO_InitStructure.Mode 	 = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
    GPIO_InitStructure.Speed 	 = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull 	 = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
 * @ingroup DMX512_driver
 * @fn _DMX512_driver_UART_init
 * @brief Initialises uart peripheral
 *
 * @param this handle to the DMX512 driver
 */
void _DMX512_driver_UART_init(void){

	this.uart.Instance 		 	= USART1;
	this.uart.Init.WordLength 	= UART_WORDLENGTH_8B;
	this.uart.Init.StopBits 	= UART_STOPBITS_2;
	this.uart.Init.Parity 		= UART_PARITY_NONE;
	this.uart.Init.Mode		 	= UART_MODE_TX_RX;
	this.uart.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
	this.uart.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&this.uart);

}

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_driver.h for declarations
 *=============================================================================================================================*/

/**
 * @ingroup DMX512_driver
 * @fn DMX512_driver_init
 * @brief Initialises DMX512 driver periphals
 *
 * Once initialised the driver may be controlled started or stopped
 * using DMX512_driver_start and DMX512_driver_stop functions
 */
void DMX512_driver_init(void){
	if(this.status == DMX512_DRIVER_UNINITIALISED){
		_DMX512_driver_GPIO_init();
		_DMX512_driver_UART_init();
		this.status = DMX512_DRIVER_INITIALISED;
	}
}

/**
 * @ingroup DMX512_driver
 * @fn DMX512_driver_get_status
 * @brief Returns the status of the DMX512 driver
 * @return DMX512_driver_status_e status of the driver
 * @see DMX512_driver.h for further information regarding
 * 		the driver's status enumeration
 */
DMX512_driver_status_e DMX512_driver_get_status(void){
	return this.status;
}

/**
 * @ingroup DMX512_driver
 * @fn DMX512_driver_start
 * @brief Starts the DMX512 driver thread
 *
 * @return DMX512_driver_err_e returns DMX512_DRIVER_OK if the thread could be started.
 * @see DMX512_driver.h for complementary informations regarding the driver's error codes
 */
DMX512_driver_err_e DMX512_driver_start(void){

	const osThreadAttr_t DMX512_driver_thread_attr = {
		.priority = osPriorityNormal,
		.stack_size = configMINIMAL_STACK_SIZE*4
	};

	DMX512_driver_ThreadId = osThreadNew(_DMX512_driver_thread, NULL, &DMX512_driver_thread_attr);
	return DMX512_driver_ThreadId != NULL ? DMX512_DRIVER_OK : DMX512_DRIVER_THREAD_ERR;

}

/**
 * @ingroup DMX512_driver
 * @fn DMX512_driver_stop
 * @brief Terminates the DMX512 driver thread
 *
 * @return DMX512_driver_err_e returns DMX512_DRIVER_OK if the thread could be terminated.
 * @see DMX512_defs.h for complementary informations regarding error codes
 */
DMX512_driver_err_e DMX512_driver_stop(void){
	return osThreadTerminate(DMX512_driver_ThreadId) == osOK ? DMX512_DRIVER_OK : DMX512_DRIVER_THREAD_ERR;
}

/**
 * @ingroup DMX512_driver
 * @fn DMX512_driver_set_single
 * @brief Sets a buffer's address to a given value
 *
 * @param address the address concerned.
 * @param value the value of the address to be set.
 * @return DMX512_driver_err_e returns DMX512_driver_OK if the value could be set.
 * @see DMX512_defs.h for complementary informations regarding error codes
 */
DMX512_driver_err_e DMX512_driver_set_single(uint16_t address, uint8_t value){

	DMX512_driver_err_e err = DMX512_DRIVER_OK;

	if(!_DMX512_driver_address_check(address)){
		err = DMX512_DRIVER_ADDRESS_OUT_OF_BOUNDS;
	}else{
		this.value_buffer[address] = value;
	}

	return err;
}
