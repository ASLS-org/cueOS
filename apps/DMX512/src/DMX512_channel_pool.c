#include "DMX512_channel_pool.h"

static DMX512_channel_pool_s *this;

DMX512_channel_pool_s *DMX512_channel_pool_init(void){

	this = malloc(sizeof(DMX512_channel_pool_s));

	this->set = DMX512_channel_pool_set;

	for(uint16_t i=0; i< DMX512_CHANNEL_ADDRESS_MAX; i++){
		this->_value_buffer[i] = DMX512_CHANNEL_VALUE_MIN;
	}

	return this;

}

DMX512_engine_err_e DMX512_channel_pool_set(uint16_t address, uint8_t value){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(address >= DMX512_CHANNEL_ADDRESS_MIN && address <= DMX512_CHANNEL_ADDRESS_MAX){
		if(value >= DMX512_CHANNEL_VALUE_MIN && value <= DMX512_CHANNEL_VALUE_MAX){
			this->_value_buffer[address] = value;
		}else{
			err = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
		}
	}else{
		err = DMX512_CHANNEL_VALUE_OUT_OF_BOUNDS;
	}

	return err;

}
