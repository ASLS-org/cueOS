#include "DMX512_channel.h"

static DMX512_engine_err_e DMX512_channel_setValue(DMX512_channel_s *channel, uint8_t value);


DMX512_engine_err_e DMX512_channel_init(DMX512_channel_s *channel, uint16_t address){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	channel = malloc(sizeof(DMX512_channel_s));

	if(address >= DMX512_CHANNEL_ADDRESS_MIN && address <= DMX512_CHANNEL_ADDRESS_MAX){
		channel->address 	= address;
		channel->value 		= DMX512_CHANNEL_VALUE_MIN;
		channel->setValue	= DMX512_channel_setValue;
	}else{
		err = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
	}

	return err;

}

static DMX512_engine_err_e DMX512_channel_setValue(DMX512_channel_s *channel, uint8_t value){
	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	if(value >= DMX512_CHANNEL_VALUE_MIN && value <= DMX512_CHANNEL_VALUE_MAX){
		channel->value = value;
	}else{
		err = DMX512_CHANNEL_VALUE_OUT_OF_BOUNDS;
	}
	return err;
}
