/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include "cmsis_os.h"
#include "DMX512_scene_pool.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Finds the array index of a scene
 *
 * @param *scene_pool pointer to the scene pool instance to be referenced
 * @param id the scene's identifier
 * @return int16_t the array index of the scene.
 * -1 is returned if the scene couldn't be found
 */
static int16_t _DMX512_scene_pool_search(DMX512_scene_pool_s *scene_pool, uint16_t id){
	for(uint16_t i=0; i<scene_pool->scene_count; i++){
		if(id == scene_pool->scenes[i].id){
			return i;
		}
	}
	return -1;
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_fixture_pool.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Creates a new scene pool instance
 *
 * @return DMX512_scene_pool_s* pointer to the created pool instance
 */
DMX512_scene_pool_s *DMX512_scene_pool_new(void){
	DMX512_scene_pool_s *scene_pool = pvPortMalloc(sizeof(DMX512_scene_pool_s));
	scene_pool->scenes 	  = NULL;
	scene_pool->scene_count = 0;
	return scene_pool;
}

/**
 * @brief Adds a scene instance into the pool
 *
 * @param *scene_pool pointer to the scene pool instance to be referenced
 * @param scene scene instance to be added into the pool
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_pool_s *scene_pool, DMX512_scene_s scene){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(scene.status != DMX512_SCENE_INITIALISED){
		err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	}else if(_DMX512_scene_pool_search(scene_pool, scene.id) >= 0){
		err  = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else if(err == DMX512_ENGINE_OK){
		scene_pool->scenes = (DMX512_scene_s*) pvPortRealloc(scene_pool->scenes, sizeof(DMX512_scene_s) * (scene_pool->scene_count + 1));
		scene_pool->scenes[scene_pool->scene_count] = scene;
		scene_pool->scene_count++;
	}

	return err;

}

/**
 * @brief Deletes a scene instance from the pool
 *
 * @param *scene_pool pointer to the scene pool instance to be referenced
 * @param id the scene's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_pool_del(DMX512_scene_pool_s *scene_pool, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_scene_pool_search(scene_pool, id);

	if(index >= 0){
		for(uint16_t i=index+1; i<scene_pool->scene_count; i++){
				scene_pool->scenes[i-1] = scene_pool->scenes[i];
		}
		scene_pool->scene_count--;
		scene_pool->scenes = pvPortRealloc(scene_pool->scenes, sizeof(DMX512_scene_s) * (scene_pool->scene_count));
	}else{
		err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	}

	return err;

}

/**
 * @brief Gets a scene instance from the pool
 *
 * @param *scene_pool pointer to the scene pool instance to be referenced
 * @param id the scene's identifier
 * @return DMX512_scene_s* pointer to the scene instance
 */
DMX512_scene_s *DMX512_scene_pool_get(DMX512_scene_pool_s *scene_pool, uint16_t id){
	int16_t index = _DMX512_scene_pool_search(scene_pool, id);
	if(index >= 0){
		return &scene_pool->scenes[index];
	}else{
		return NULL;
	}
}

/**
 * @brief Manages all scenes contained within the pool
 *
 * @param *scene_pool pointer to the scene pool instance to be referenced
 */
void DMX512_scene_pool_manage(DMX512_scene_pool_s *scene_pool){
	for(uint16_t i = 0; i<scene_pool->scene_count;i++){
		DMX512_scene_manage(&scene_pool->scenes[i]);
	}
}

#endif
