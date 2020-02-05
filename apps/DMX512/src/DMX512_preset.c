#include "DMX512_preset.h"
#include "DMX512_patcher.h"
#include <string.h>

static DMX512_engine_err_e DMX512_preset_update(DMX512_preset_s *this, uint16_t chCount, uint16_t *channels, uint8_t *values);
static DMX512_engine_err_e DMX512_preset_validate(DMX512_preset_s *this, uint16_t chCount,  uint16_t *channels, uint8_t *values);


DMX512_engine_err_e DMX512_preset_init(DMX512_preset_s *preset, uint16_t fixtureId, uint16_t chCount, uint8_t *values, uint16_t *channels){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	preset = malloc(sizeof(DMX512_preset_s));

	if((err = DMX512_patcher_getFixtureInstance(preset->fixture, fixtureId)) == DMX512_ENGINE_OK){
		preset->id = preset->fixture->id;
		preset->update = DMX512_preset_update;
		err = preset->update(preset, chCount, channels, values);
	}else{
		free(preset);
	}

	return err;

}

static DMX512_engine_err_e DMX512_preset_update(DMX512_preset_s *this, uint16_t chCount, uint16_t *channels, uint8_t *values){

	DMX512_engine_err_e err = DMX512_preset_validate(this, chCount, values, channels);

	if(err == DMX512_ENGINE_OK){
		this->channels 	= malloc(chCount * sizeof(uint16_t));
		this->values 	= malloc(chCount * sizeof(uint8_t));
		memcpy(this->channels, channels, chCount);
		memcpy(this->values, values, chCount);
	}

	return err;

}

static DMX512_engine_err_e DMX512_preset_validate(DMX512_preset_s *this, uint16_t chCount, uint16_t *channels, uint8_t *values){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(uint16_t i=0; i< chCount; i++){
		if(sizeof(values)/sizeof(uint16_t) != chCount || sizeof(channels)/sizeof(uint16_t) != chCount){
			err = DMX512_PRESET_CHANNEL_COUNT_MISMATCH;
		}else if(channels[i] > this->fixture->chCount){
			err = DMX512_PRESET_CHANNEL_OUT_OF_BOUNDS;
		}else if(values[i] < DMX512_CHANNEL_VALUE_MIN || values[i] > DMX512_CHANNEL_VALUE_MAX){
			err = DMX512_PRESET_VALUE_OUT_OF_BOUNDS;
		}
	}

	err = DMX512_patcher_getFixtureInstance(this->fixture, this->fixture->id);

	return err;

}
