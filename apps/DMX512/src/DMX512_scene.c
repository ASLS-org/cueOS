#include "DMX512_scene.h"
#include "DMX512_fixture_pool.h"
#include "cmsis_os.h"

static int16_t _DMX512_scene_search(DMX512_scene_s *this, uint16_t id);

DMX512_scene_s DMX512_scene_init(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time){
	DMX512_scene_s scene;
	scene.id = id;
	scene.fadein_time = fadein_time;
	scene.fadeout_time = fadeout_time;
	scene._fixture_presets = NULL;
	return scene;
}

/**
 * Adds a fixture preset instance into the scene
 *
 * @param this pointer to the scene instance
 * @param id the fixture's idendifier
 * @param values the list of values to be stored within the scene
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_add(DMX512_scene_s *this, uint16_t id, uint8_t *values){

	DMX512_engine_err_e err   = DMX512_ENGINE_OK;
	DMX512_fixture_s *fixture = DMX512_fixture_pool_get(id);

	if(fixture == NULL){
		DMX512_fixture_preset_s preset;
		preset.id = fixture->id;
		preset._channel_values = pvPortMalloc(sizeof(uint8_t) * (fixture->ch_stop - fixture->ch_start + 1));
		this->_fixture_presets = pvPortRealloc(this->_fixture_presets, sizeof(DMX512_fixture_preset_s) * this->_preset_count + 1);
		this->_fixture_presets[this->_preset_count] = preset;
		this->_preset_count++;
	}else{
		err = DMX512_PRESET_DUP;
	}

	return err;

}


/**
 * Deletes a fixture preset instance from the scene
 *
 * @param id the fixture preset's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_del(DMX512_scene_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_scene_search(this, id);

	if(index >= 0){
		for(uint16_t i=0; i< this->_preset_count; i++){
			if(i > index){
				this->_fixture_presets[i-1] = this->_fixture_presets[i];
			}
		}
		this->_preset_count--;
		vPortFree(this->_fixture_presets[index]._channel_values);
		this->_fixture_presets = pvPortRealloc(this->_fixture_presets, sizeof(DMX512_fixture_s) * (this->_preset_count));
	}else{
		err = DMX512_PRESET_UNKNW;
	}

	return err;

}

/**
 * Gets a fixture instance from the pool
 *
 * @param id the fixture preset identifier
 * @return *DMX512_fixture_preset_s pointer to the fixture preset instance
 */
DMX512_fixture_preset_s *DMX512_scene_get(DMX512_scene_s *this, uint16_t id){
	int16_t index = _DMX512_scene_search(this, id);
	if(index >= 0){
		return &this->_fixture_presets[index];
	}else{
		return NULL;
	}

}


/**
 * Finds the array index of a fixture
 *
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_scene_search(DMX512_scene_s *this, uint16_t id){
	for(uint16_t i=0; i<this->_preset_count; i++){
		if(id == this->_fixture_presets[i].id){
			return i;
		}
	}
	return -1;
}


