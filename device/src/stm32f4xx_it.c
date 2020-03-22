/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "main.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"


extern ETH_HandleTypeDef heth;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern SD_HandleTypeDef hsd;
extern TIM_HandleTypeDef htim2;


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see stm32f4xx_it.h for declarations
 *=============================================================================================================================*/

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void){
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void){
	for(;;);
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void){
	for(;;);
}


/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void){
	for(;;);
}


/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void){
	for(;;);
}


/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void){
	for(;;);
}


/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void){
  HAL_TIM_IRQHandler(&htim2);
}

/**
  * @brief This function handles SDIO global interrupt.
  */
void SDIO_IRQHandler(void){
  HAL_SD_IRQHandler(&hsd);
}

/**
  * @brief This function handles DMA2 stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void){
  HAL_DMA_IRQHandler(&hdma_sdio_rx);
}

/**
  * @brief This function handles Ethernet global interrupt.
  */
void ETH_IRQHandler(void){
  HAL_ETH_IRQHandler(&heth);
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void){
  HAL_DMA_IRQHandler(&hdma_sdio_tx);
}
