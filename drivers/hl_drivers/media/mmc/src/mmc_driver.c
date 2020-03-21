/*
 * mmc_driver.c
 *
 *  Created on: 31 Jan 2020
 *      Author: root
 */

#include "mmc_driver.h"

DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;
SD_HandleTypeDef hsd;

void mmc_init(void){
	  hsd.Instance = SDIO;
	  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	  hsd.Init.ClockDiv = 0;
}
