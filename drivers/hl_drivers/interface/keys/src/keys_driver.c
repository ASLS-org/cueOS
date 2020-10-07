#include "stm32f4xx_hal.h"

I2C_HandleTypeDef hi2c3;

/**
 * TODO: Device pins too busy for I2C...
 */
static void _keys_config_i2c(void){
	hi2c3.Instance = I2C3;
	hi2c3.Init.ClockSpeed = 100000;
	hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	HAL_I2C_Init(&hi2c3);
}

void keys_init(void){

	_keys_config_i2c();
	char *test = "test";
	int i;

	if(HAL_I2C_Master_Transmit(&hi2c3, 0x70, test, 4, 10000) == HAL_OK){
		uint8_t hh = 0;
	}

//	for(i=0x70; i<0x80; i++){
//		if(HAL_I2C_Master_Transmit(&hi2c3, i, &test, 4, 1000) == HAL_OK){
//			uint8_t hh = 0;
//			break;
//		}
//	}

}
