/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <stdlib.h>
#include "math.h"
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_scene.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Finds the array index of a fixture
 *
 * @param this pointer to the scene instance
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_scene_search(DMX512_scene_s *this, uint16_t fixture_id){
	for(uint16_t i=0; i<this->preset_count; i++){
		if(fixture_id == this->presets[i].fixture->id){
			return i;
		}
	}
	return -1;
}

/**
 * Sets a scenes' fixtures channel values to their predefined preset values according to time
 * @param this the scene instance to be faded in
 */
static void _DMX512_scene_fadein(DMX512_scene_s *this){
	for(uint16_t i=0; i<this->preset_count; i++){
		for(uint16_t j=0; j<this->presets[i].ch_count; j++){
			uint16_t addr 	= this->presets[i].fixture->addr + this->presets[i].channels[j];
			uint16_t value 	= this->presets[i].values[j];
			uint32_t time 	= DMX512_utils_mschronometer_get_elapsed_ms(&this->mschronometer);
			if(time < this->fadein_time){
				value = floor(((float)value/(float)this->fadein_time)*((float)(time)));
			}else{
				this->state = DMX512_SCENE_IDLE;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}

/**
 * Sets a scenes' fixtures channel values to their "OFF" values according to time
 * @param this the scene instance to be faded out
 */
static void _DMX512_scene_fadeout(DMX512_scene_s *this){
	for(uint16_t i=0; i<this->preset_count; i++){
		for(uint16_t j=0; j<this->presets[i].ch_count; j++){
			uint16_t addr 	= this->presets[i].fixture->addr + this->presets[i].channels[j];
			uint16_t value 	= this->presets[i].values[j];
			uint32_t time 	= DMX512_utils_mschronometer_get_elapsed_ms(&this->mschronometer);
			if(time < this->fadeout_time){
				value = floor(((float)value/(float)this->fadein_time)*((float)(time + value)));
			}else{
				this->state = DMX512_SCENE_IDLE;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * Creates and initialises a new scene instance
 *
 * @param id the scene identifier (@see DMX512_scene_pool.h)
 * @param fadein_time the scene fade-in time in milliseconds
 * @param fadeout_time the scene fade-out time in milliseconds
 * @return DMX512_scene_s the created scene
 */
DMX512_scene_s DMX512_scene_new(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time){
	DMX512_scene_s scene = DMX512_SCENE_DEFAULT;
	scene.id 			 = id;
	scene.fadein_time 	 = fadein_time;
	scene.fadeout_time 	 = fadeout_time;
	scene.status 		 = DMX512_SCENE_INITIALISED;
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
DMX512_engine_err_e DMX512_scene_add_preset(DMX512_scene_s *this, DMX512_fixture_preset_s preset){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(preset.status == DMX512_FIXTURE_PRESET_UNINITIALISED){
		err = DMX512_ENGINE_INSTANCE_INVALID;
	}else if(_DMX512_scene_search(this, preset.fixture->id) >= 0){
		err = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else{
		this->presets = (DMX512_fixture_preset_s*) pvPortRealloc(this->presets, sizeof(DMX512_fixture_preset_s) * (this->preset_count + 1));
		this->presets[this->preset_count] = preset;
		this->preset_count++;
	}

	return err;

}

/**
 * Deletes a fixture preset instance from the scene
 *
 * @param this pointer to the scene instance
 * @param id the fixture preset's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_del_preset(DMX512_scene_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	int16_t index = _DMX512_scene_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< this->preset_count; i++){
			this->presets[i-1] = this->presets[i];
		}
		this->preset_count--;
		vPortFree(this->presets[index].values);
		vPortFree(this->presets[index].channels);
		//TODO: create free template in DMX512_fixture_preset.c
		this->presets = pvPortRealloc(this->presets, sizeof(DMX512_fixture_s) * (this->preset_count));
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * Gets a fixture instance from the pool
 *
 * @param this pointer to the scene instance
 * @param id the fixture preset identifier
 * @return *DMX512_fixture_preset_s pointer to the fixture preset instance
 */
DMX512_fixture_preset_s *DMX512_scene_get_preset(DMX512_scene_s *this, uint16_t id){
	int16_t index = _DMX512_scene_search(this, id);
	if(index >= 0){
		return &this->presets[index];
	}else{
		return NULL;
	}
}

/**
 * Starts a scene fade-in process
 *
 * @param this pointer to the scene instance
 */
void DMX512_scene_start(DMX512_scene_s *this){
	DMX512_utils_mschronometer_reset(&this->mschronometer);
	this->state = DMX512_SCENE_FADEIN;
}

/**
 * Starts a scene fade-out process
 *
 * @param this pointer to the scene instance
 */
void DMX512_scene_stop(DMX512_scene_s *this){
	DMX512_utils_mschronometer_reset(&this->mschronometer);
	this->state = DMX512_SCENE_FADEOUT;
}

/**
 * State machine to manage scene at a given state
 *
 * @param this pointer to the scene instance
 * @see DMX512_scene.h DMX512_scene_state_e state enumeration
 */
void DMX512_scene_manage(DMX512_scene_s *this){
	switch(this->state){
		case DMX512_SCENE_FADEIN: 	_DMX512_scene_fadein(this); 	break;
		case DMX512_SCENE_FADEOUT: 	_DMX512_scene_fadeout(this); 	break;
		case DMX512_SCENE_IDLE:		/*TODO: handle IDLE state ?*/	break;
	}
}
