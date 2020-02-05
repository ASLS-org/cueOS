#include "DMX512_channel.h"

static DMX512_engine_err_e DMX512_channel_setValue(DMX512_channel_s *channel, uint8_t value);


DMX512_engine_err_e DMX512_channel_init(DMX512_channel_s *channel, uint16_t address){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	channel = malloc(sizeof(DMX512_channel_s));

	if(address >= DMX512_MIN_CHANNEL && address <= DMX512_MIN_CHANNEL){
		channel->address 	= address;
		channel->value 		= DMX512_MIN_VALUE;
		channel->setValue	= DMX512_channel_setValue;
	}else{
		err = DMX512_CHN_OUT_OF_BOUNDS;
	}

	return err;

}

static DMX512_engine_err_e DMX512_channel_setValue(DMX512_channel_s *channel, uint8_t value){
	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	if(value >= DMX512_MIN_VALUE && value <= DMX512_MAX_VALUE){
		channel->value = value;
	}else{
		err = DMX512_VAL_OUT_OF_BOUNDS;
	}
	return err;
}
