//#include "DMX512_preset.h"
//#include <string.h>
//
//static DMX512_engine_err_e DMX512_preset_update(DMX512_preset_s *this, uint16_t *channels, uint8_t *values);
//static DMX512_engine_err_e DMX512_preset_validate(DMX512_preset_s *this, uint16_t *channels, uint8_t *values);
//static DMX512_engine_err_e DMX512_preset_apply(DMX512_preset_s *this);
//
//
//DMX512_engine_err_e DMX512_preset_init(DMX512_preset_s *preset, uint16_t fixtureId, uint16_t *channels, uint8_t *values){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//	preset = pvPortMalloc(sizeof(DMX512_preset_s));
//
//	preset->id 		= preset->fixture->id;
//	preset->chCount = sizeof(preset->channels)/sizeof(uint16_t);
//	preset->upd 	= DMX512_preset_update;
//	preset->set 	= DMX512_preset_apply;
//	err = preset->upd(preset, channels, values);
//
//	if(err != DMX512_ENGINE_OK){ free(preset); }
//
//	return err;
//
//}
//
//static DMX512_engine_err_e DMX512_preset_update(DMX512_preset_s *this, uint16_t *channels, uint8_t *values){
//
//	DMX512_engine_err_e err = DMX512_preset_validate(this, channels, values);
//
//	if(err == DMX512_ENGINE_OK){
//		this->channels 	= pvPortMalloc(this->chCount * sizeof(uint16_t));
//		this->values 	= pvPortMalloc(this->chCount * sizeof(uint8_t));
//		memcpy(this->channels, channels, this->chCount);
//		memcpy(this->values, values, this->chCount);
//	}
//
//	return err;
//
//}
//
//static DMX512_engine_err_e DMX512_preset_apply(DMX512_preset_s *this){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//
//	for(uint16_t i=0; i< sizeof(this->channels)/sizeof(uint16_t); i++){
//		//this->fixture->set(this->fixture, this->channels[i], this->values[i]);
//	}
//
//	return err;
//
//}
//
//static DMX512_engine_err_e DMX512_preset_validate(DMX512_preset_s *this, uint16_t *channels, uint8_t *values){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//
//	for(uint16_t i=0; i< this->chCount; i++){
//		if(sizeof(values)/sizeof(uint16_t) != this->chCount || sizeof(channels)/sizeof(uint16_t) != this->chCount){
//			err = DMX512_PRESET_CHANNEL_COUNT_MISMATCH;
//		}else if(channels[i] > this->fixture->chCount){
//			err = DMX512_PRESET_CHANNEL_OUT_OF_BOUNDS;
//		}else if(values[i] < DMX512_CHANNEL_VALUE_MIN || values[i] > DMX512_CHANNEL_VALUE_MAX){
//			err = DMX512_PRESET_VALUE_OUT_OF_BOUNDS;
//		}
//	}
//
//	return err;
//
//}
