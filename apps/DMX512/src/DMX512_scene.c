#include "DMX512_scene.h"
#include "DMX512_fixture_pool.h"
#include "DMX512_driver.h"
#include "cmsis_os.h"

/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Finds the array index of a fixture
 *
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_scene_search(DMX512_scene_s *this, uint16_t id){
	for(uint16_t i=0; i<this->fixture_preset_count; i++){
		if(id == this->_fixture_presets[i].id){
			return i;
		}
	}
	return -1;
}

//TODO: Implement scene fade-in fade-out

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * Creates and initialises a new scene instance
 *
 * @param id the scene identifier (@see DMX512_scene_pool.h)
 * @param fadein_time the scene fade-in time in ms
 * @param fadeout_time the scene fade-out time in ms
 * @return DMX512_scene_s the created scene
 */
DMX512_scene_s DMX512_scene_init(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time){
	DMX512_scene_s scene;
	scene.id = id;
	scene.fadein_time = fadein_time;
	scene.fadeout_time = fadeout_time;
	scene._fixture_presets = NULL;
	scene.fixture_preset_count = 0;
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
DMX512_engine_err_e DMX512_scene_add(DMX512_scene_s *this, uint16_t id,  uint16_t *channels, uint8_t *values, uint16_t count){

	DMX512_engine_err_e err   = DMX512_ENGINE_OK;
	DMX512_fixture_s *fixture = DMX512_fixture_pool_get(id);

	if(_DMX512_scene_search(this, id) >= 0){
		err = DMX512_PRESET_DUP;
	}else if(fixture == NULL){
		err = DMX512_FIXTURE_UNKNW;
	}else{

		//TODO make separate file for fxture presets
		DMX512_fixture_preset_s fixture_preset;

		fixture_preset.id 		= fixture->id;
		fixture_preset.values 	= pvPortMalloc(count*sizeof(uint8_t));
		fixture_preset.channels = pvPortMalloc(count*sizeof(uint16_t));
		fixture_preset.count 	= count;

		memcpy(fixture_preset.channels, channels, fixture_preset.count*sizeof(uint16_t));
		memcpy(fixture_preset.values, values, fixture_preset.count*sizeof(uint8_t));

		this->_fixture_presets = (DMX512_fixture_preset_s*) pvPortRealloc(this->_fixture_presets, sizeof(DMX512_fixture_preset_s) * (this->fixture_preset_count + 1));
		this->_fixture_presets[this->fixture_preset_count] = fixture_preset;

		uint16_t chan1 = this->_fixture_presets[this->fixture_preset_count].channels[1];

		this->fixture_preset_count++;

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

	DMX512_engine_err_e err = DMX512_PRESET_UNKNW;
	int16_t index = _DMX512_scene_search(this, id);

	if(index >= 0){
		for(uint16_t i=0; i< this->fixture_preset_count; i++){
			if(i > index){
				this->_fixture_presets[i-1] = this->_fixture_presets[i];
			}
		}
		this->fixture_preset_count--;
		vPortFree(this->_fixture_presets[index].values);
		this->_fixture_presets = pvPortRealloc(this->_fixture_presets, sizeof(DMX512_fixture_s) * (this->fixture_preset_count));
		err = DMX512_ENGINE_OK;
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
 * Triggers a DMX scene by assinging fixture preset values to DMX512 buffer
 *
 * @param this scene instance
 */
void DMX512_scene_trigger(DMX512_scene_s *this){
	for(uint16_t i=0; i<this->fixture_preset_count; i++){
		DMX512_fixture_s *fixture = DMX512_fixture_pool_get(this->_fixture_presets[i].id);
		for(uint16_t j=0; j<this->_fixture_presets[i].count; j++){
			uint16_t addr = fixture->addr + this->_fixture_presets[i].channels[j];
			uint8_t value = this->_fixture_presets[i].values[j];
			DMX512_driver_set_single(addr, value);
		}
	}
}


