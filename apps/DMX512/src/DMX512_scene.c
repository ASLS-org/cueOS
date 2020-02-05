#include "DMX512_scene.h"
#include "DMX512_fixture.h"
#include "DMX512_patcher.h"
#include <string.h>


static DMX512_engine_err_e DMX512_scene_addPreset(DMX512_scene_s *this, uint16_t fixtureId, uint8_t *values);
static DMX512_engine_err_e DMX512_scene_updPreset(DMX512_scene_s *this, uint16_t id, uint8_t *values);
static DMX512_engine_err_e DMX512_scene_clrPreset(DMX512_scene_s *scene, uint16_t fixtureId);
static void DMX512_scene_trigger(DMX512_scene_s *this);
static int16_t DMX512_scene_getPresetIndex(DMX512_scene_s *this, uint16_t presetId);


DMX512_scene_s *DMX512_scene_init(uint16_t id){

	DMX512_scene_s *scene = malloc(sizeof(DMX512_scene_s));

	scene->id = id;
	scene->presetCount = 0;
	scene->presets = NULL;

	scene->addPreset = DMX512_scene_addPreset;
	scene->updPreset = DMX512_scene_updPreset;
	scene->clrPreset = DMX512_scene_clrPreset;
	scene->trigger	 = DMX512_scene_trigger;

	return scene;

}

static DMX512_engine_err_e DMX512_scene_addPreset(DMX512_scene_s *this, uint16_t id, uint8_t *values){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(DMX512_scene_getPresetIndex(this, id) == DMX512_PRESET_NOT_FOUND){
		this->presetCount ++;
		this->presets = realloc(this->presets, this->presetCount * sizeof(DMX512_preset_s));
		err = DMX512_preset_init(this->presets[this->presetCount], id, values);
		if(err != DMX512_ENGINE_OK){
			DMX512_scene_clrPreset(this, id);
		}
	}else{
		err = DMX512_SCENE_PRESET_DUP;
	}

	return err;

}

static DMX512_engine_err_e DMX512_scene_updPreset(DMX512_scene_s *this, uint16_t id, uint8_t *values){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	uint16_t index = DMX512_scene_getPresetIndex(this, id);

	if(id != DMX512_PRESET_NOT_FOUND){
		err = this->presets[index]->update(this->presets[index], values);
	}else{
		err = DMX512_SCENE_PRESET_UNKNW;
	}

	return err;

}

static DMX512_engine_err_e DMX512_scene_clrPreset(DMX512_scene_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = DMX512_scene_getPresetIndex(this,id);

	if(id != DMX512_PRESET_NOT_FOUND){

		DMX512_preset_s **tmpPresets = malloc((this->presetCount - 1) * sizeof(DMX512_preset_s));

		for(uint16_t i=0; i<this->presetCount; i++){
			if(i < index){
				tmpPresets[i] = this->presets[i];
			}else{
				tmpPresets[i] = this->presets[i+1];
			}
		}

		this->presetCount--;
		memcpy(this->presets, tmpPresets, this->presetCount * sizeof(DMX512_preset_s));
		free(tmpPresets);

	}else{
		err = DMX512_SCENE_PRESET_UNKNW;
	}


	return err;

}

static void DMX512_scene_trigger(DMX512_scene_s *this){
	for(uint16_t i = 0; i< this->presetCount; i++){
		for(uint16_t j = 0; j< this->presets[i]->fixture->chCount; j++){
			this->presets[i]->fixture->channels[i]->setValue(this->presets[i]->values[j]);
		}
	}
}


static int16_t DMX512_scene_getPresetIndex(DMX512_scene_s *this, uint16_t presetId){

	for(uint16_t i=0; i< (sizeof(this->presets)/sizeof(DMX512_preset_s)); i++){
		if(presetId == this->presets[i]->id){
			return i;
		}
	}

	return DMX512_PRESET_NOT_FOUND;

}
