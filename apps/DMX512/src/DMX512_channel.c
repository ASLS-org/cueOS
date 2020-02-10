#include "cmsis_os.h"
#include "DMX512_channel.h"

static DMX512_engine_err_e DMX512_channel_setValue(DMX512_channel_s *channel, uint8_t value);


DMX512_channel_s *DMX512_channel_init(uint16_t address){

	DMX512_channel_s *channel = (DMX512_channel_s*)pvPortMalloc(sizeof(DMX512_channel_s));

	channel->address 	= address;
	channel->value 		= DMX512_CHANNEL_VALUE_MIN;
	channel->set		= DMX512_channel_setValue;

	return channel;

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
