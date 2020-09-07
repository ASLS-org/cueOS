
/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <stdlib.h>
#include "math.h"
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_scene.h"


/***===========================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Finds the array index of a fixture
 *
 * @param *scene pointer to the scene instance
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_scene_search(DMX512_scene_s *scene, uint16_t fixture_id){
	for(uint16_t i=0; i<scene->fixture_preset_count; i++){
		if(fixture_id == scene->fixture_presets[i].fixture->id){
			return i;
		}
	}
	return -1;
}

/**
 * @brief Sets a scenes' fixtures channel values to their predefined preset values according to time
 *
 * @param *scene pointer to the scene instance to be faded in
 */
static void _DMX512_scene_fadein(DMX512_scene_s *scene){
	for(uint16_t i=0; i<scene->fixture_preset_count; i++){
		for(uint16_t j=0; j<scene->fixture_presets[i].ch_count; j++){
			uint16_t addr 	= scene->fixture_presets[i].fixture->addr + scene->fixture_presets[i].channels[j];
			uint16_t value 	= scene->fixture_presets[i].values[j];
			uint32_t time 	= ms_chronometer_get_elapsed_ms(&scene->ms_chronometer);
			if(time < scene->fadein_time){
				value = floor(((float)value/(float)scene->fadein_time)*((float)(time)));
			}else{
				scene->state = DMX512_SCENE_IDLE;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}

/**
 * @brief Sets a scenes' fixtures channel values to their "OFF" values according to time
 * @param *scene pointer to the scene instance to be faded out
 */
static void _DMX512_scene_fadeout(DMX512_scene_s *scene){
	for(uint16_t i=0; i<scene->fixture_preset_count; i++){
		for(uint16_t j=0; j<scene->fixture_presets[i].ch_count; j++){
			uint16_t addr 	= scene->fixture_presets[i].fixture->addr + scene->fixture_presets[i].channels[j];
			uint16_t value 	= scene->fixture_presets[i].values[j];
			uint32_t time 	= ms_chronometer_get_elapsed_ms(&scene->ms_chronometer);
			if(time < scene->fadeout_time){
				value = floor(((float)value/(float)scene->fadein_time)*((float)(time + value)));
			}else{
				scene->state = DMX512_SCENE_IDLE;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Creates and initialises a new scene instance
 *
 * @param id the scene identifier
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
 * @brief Adds a fixture preset instance into the scene
 *
 * @param *scene pointer to the scene instance
 * @param fixture_preset fixture_preset instance to be added to the scene
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_add_fixture_preset(DMX512_scene_s *scene, DMX512_fixture_preset_s fixture_preset){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(fixture_preset.status == DMX512_FIXTURE_PRESET_UNINITIALISED){
		err = DMX512_ENGINE_INSTANCE_INVALID;
	}else if(_DMX512_scene_search(scene, fixture_preset.fixture->id) >= 0){
		err = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else{
		scene->fixture_presets = (DMX512_fixture_preset_s*) pvPortRealloc(scene->fixture_presets, sizeof(DMX512_fixture_preset_s) * (scene->fixture_preset_count + 1));
		scene->fixture_presets[scene->fixture_preset_count] = fixture_preset;
		scene->fixture_preset_count++;
	}

	return err;

}

/**
 * @brief Deletes a fixture preset instance from the scene
 *
 * @param *scene pointer to the scene instance
 * @param id the fixture preset's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_del_fixture_preset(DMX512_scene_s *scene, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	int16_t index = _DMX512_scene_search(scene, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< scene->fixture_preset_count; i++){
			scene->fixture_presets[i-1] = scene->fixture_presets[i];
		}
		scene->fixture_preset_count--;
		DMX512_fixture_preset_free(&scene->fixture_presets[index]);
		scene->fixture_presets = pvPortRealloc(scene->fixture_presets, sizeof(DMX512_fixture_s) * (scene->fixture_preset_count));
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * @brief Gets a fixture instance from the pool
 *
 * @param scene pointer to the scene instance
 * @param id the fixture preset identifier
 * @return *DMX512_fixture_preset_s pointer to the fixture preset instance
 */
DMX512_fixture_preset_s *DMX512_scene_get_fixture_preset(DMX512_scene_s *scene, uint16_t id){
	int16_t index = _DMX512_scene_search(scene, id);
	if(index >= 0){
		return &scene->fixture_presets[index];
	}else{
		return NULL;
	}
}

/**
 * @brief Starts a scene fade-in process
 *
 * @param *scene pointer to the scene instance to be started
 */
void DMX512_scene_start(DMX512_scene_s *scene){
	ms_chronometer_reset(&scene->ms_chronometer);
	scene->state = DMX512_SCENE_FADEIN;
}

/**
 * @brief Starts a scene fade-out process
 *
 * @param *scene pointer to the scene instance to be stopped
 */
void DMX512_scene_stop(DMX512_scene_s *scene){
	ms_chronometer_reset(&scene->ms_chronometer);
	scene->state = DMX512_SCENE_FADEOUT;
}

/**
 * @brief State machine to manage scene at a given state
 *
 * @param *scene pointer to the scene instance to be managed
 * @see DMX512_scene.h DMX512_scene_state_e state enumeration
 */
void DMX512_scene_manage(DMX512_scene_s *scene){
	switch(scene->state){
		case DMX512_SCENE_FADEIN: 	_DMX512_scene_fadein(scene); 	break;
		case DMX512_SCENE_FADEOUT: 	_DMX512_scene_fadeout(scene); 	break;
		case DMX512_SCENE_IDLE:		/*TODO: handle IDLE state ?*/	break;
	}
}

/**
 * @brief Resets a scene to its uninitialised state.
 * 		  frees dynamically allocated scene Data.
 *
 * @param *scene pointer to the scene instance
 */
void DMX512_scene_free(DMX512_scene_s *scene){
	if(scene != NULL){
		for(uint16_t i=0; i<scene->fixture_preset_count; i++){
			DMX512_fixture_preset_free(&scene->fixture_presets[i]);
		}
		scene->fadein_time 	= 0;
		scene->fadeout_time = 0;
		scene->state 		= DMX512_SCENE_UNINITIALISED;
		scene->status		= DMX512_SCENE_IDLE;
		vPortFree(scene->fixture_presets);
	}
}

#endif
