/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "stm32f4xx_hal.h"
#include "device.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

I2C_HandleTypeDef hi2c3;
SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

/**
 * Configure system clocks
 */
void _device_config_clock(void){

	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 72;


	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 3;


	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;


	/** ====================
	 *
	 */
//	__HAL_RCC_SYSCFG_CLK_ENABLE();
//	__HAL_RCC_PWR_CLK_ENABLE();
	/**
	 * ====================
	 */

}

/**
 * TODO: mauybe put ll_drivers into device folder ? Seems to make more sense ?
 * TODO: init DMX512 serial peripheral here$
 * TODO: implement error handling
 */

/**
 * @brief initialises SDIO to be used for communication
 * 		  with SD an SD card
 */
static void _mmc_init(void){
	  hsd.Instance = SDIO;
	  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	  hsd.Init.ClockDiv = 0;
}


/**
 * Configure system GPIO clocks
 * TODO: would it be better to put these locally where GPIO are initialised ?
 */
void _device_config_gpio(void){
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef test = {0};

	test.Pin = GPIO_PIN_15;
	test.Mode = GPIO_MODE_INPUT;
	test.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &test);

}

/**
 * Configure system DMA clocks and interruptions
 * TODO: would it be better to put these locally where DMA is initialised/required ?
 */
void _device_config_dma(void){

	__HAL_RCC_DMA2_CLK_ENABLE();

	//TODO: move these declaration to related chunks of code
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

	HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see cueos.h for declarations
 *=============================================================================================================================*/

/**
 * InitialisesHardware Abstraction Layer drivers defined within driver/ll_drivers folder
 * and configures system clocks and ll peripherals
 * TODO: peripheral configuration should be handled at HL driver initialisation. Not within
 * Device configuration file.
 */
void device_init(void){

	HAL_Init();

	_device_config_clock();
	_device_config_gpio();
	_mmc_init();
	_device_config_dma();

}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
}


