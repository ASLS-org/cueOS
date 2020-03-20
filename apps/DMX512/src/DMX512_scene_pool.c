/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include"DMX512_scene_pool.h"
#include "cmsis_os.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Finds the array index of a scene
 *
 * @param id the scene's identifier
 * @return int16_t the array index of the scene.
 * -1 is returned if the scene couldn't be found
 */
static int16_t _DMX512_scene_pool_search(DMX512_scene_pool_s *this, uint16_t id){
	for(uint16_t i=0; i<this->scene_count; i++){
		if(id == this->scenes[i].id){
			return i;
		}
	}
	return -1;
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_fixture_pool.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new scene pool instance
 * @return DMX512_scene_pool_s the created pool instance
 */
DMX512_scene_pool_s *DMX512_scene_pool_new(void){
	DMX512_scene_pool_s *this = pvPortMalloc(sizeof(DMX512_scene_pool_s));
	this->scenes 	  = NULL;
	this->scene_count = 0;
	return this;
}

/**
 * Adds a scene instance into the pool
 *
 * @param id the scene's idendifier
 * @param addr scene's first channel address
 * @param ch_stop scene's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_pool_s *this, DMX512_scene_s scene){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(scene.status != DMX512_SCENE_INITIALISED){
		err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	}else if(_DMX512_scene_pool_search(this, scene.id) >= 0){
		err  = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else if(err == DMX512_ENGINE_OK){
		this->scenes = (DMX512_scene_s*) pvPortRealloc(this->scenes, sizeof(DMX512_scene_s) * (this->scene_count + 1));
		this->scenes[this->scene_count] = scene;
		this->scene_count++;
	}

	return err;

}

/**
 * Deletes a scene instance from the pool
 *
 * @param id the scene's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_pool_del(DMX512_scene_pool_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_scene_pool_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i<this->scene_count; i++){
				this->scenes[i-1] = this->scenes[i];
		}
		this->scene_count--;
		this->scenes = pvPortRealloc(this->scenes, sizeof(DMX512_scene_s) * (this->scene_count));
	}else{
		err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	}

	return err;

}

/**
 * Gets a scene instance from the pool
 *
 * @param id the scene's identifier
 * @return *DMX512_scene_s pointer to the scene instance
 */
DMX512_scene_s *DMX512_scene_pool_get(DMX512_scene_pool_s *this, uint16_t id){
	int16_t index = _DMX512_scene_pool_search(this, id);
	if(index >= 0){
		return &this->scenes[index];
	}else{
		return NULL;
	}
}

/**
 * Manages all scenes contained within the pool
 *
 * @param this pointer to the instance pool
 */
void DMX512_scene_pool_manage(DMX512_scene_pool_s *this){
	for(uint16_t i = 0; i<this->scene_count;i++){
		DMX512_scene_manage(&this->scenes[i]);
	}
}
