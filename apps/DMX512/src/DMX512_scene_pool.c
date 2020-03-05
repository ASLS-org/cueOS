#include"DMX512_scene_pool.h"

/* Private variable declarations */
static DMX512_scene_s *_instances;	//Dynamic array containing scene instances
static size_t _count;					//Current size of the dynamic array

/* Private functions declarations */
static DMX512_engine_err_e _DMX512_scene_pool_check(uint16_t id, uint16_t addr, uint16_t ch_stop);
static int16_t _DMX512_scene_pool_search(uint16_t id);


/**
 * TODO: to improve search time greatly, it may be useful to implement
 * a linked hashtable/linkedlist combo. but since every call to malloc
 * requires 10 more bytes, for ARM bit alignment, this will be tried
 * later, when external RAM will be available
 */

/**
 * Adds a scene instance into the pool
 *
 * @param id the scene's idendifier
 * @param addr scene's first channel address
 * @param ch_stop scene's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_s scene){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(_DMX512_scene_pool_search(scene.id) >= 0){
		err  = DMX512_SCENE_DUP;
	}else if(err == DMX512_ENGINE_OK){
		_instances = (DMX512_scene_s*) pvPortRealloc(_instances, sizeof(DMX512_scene_s) * (_count + 1));
		_instances[_count] = scene;
		_count++;
	}

	return err;

}

/**
 * Deletes a scene instance from the pool
 *
 * @param id the scene's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_pool_del(uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_scene_pool_search(id);

	if(index >= 0){
		for(uint16_t i=index+1; i<_count; i++){
				_instances[i-1] = _instances[i];
		}
		_count--;
		_instances = pvPortRealloc(_instances, sizeof(DMX512_scene_s) * (_count));
	}else{
		err = DMX512_SCENE_UNKNW;
	}

	return err;

}

/**
 * Gets a scene instance from the pool
 *
 * @param id the scene's identifier
 * @return *DMX512_scene_s pointer to the scene instance
 */
DMX512_scene_s *DMX512_scene_pool_get(uint16_t id){
	int16_t index = _DMX512_scene_pool_search(id);
	if(index >= 0){
		return &_instances[index];
	}else{
		return NULL;
	}

}

/**
 * Returns the whole instance array
 *
 */
DMX512_scene_s *DMX512_scene_pool_get_all(void){
	return _instances;
}

/**
 * Returns the whole instance array
 *
 */
uint16_t DMX512_scene_pool_get_size(void){
	return _count;
}


/**
 * Finds the array index of a scene
 *
 * @param id the scene's identifier
 * @return int16_t the array index of the scene.
 * -1 is returned if the scene couldn't be found
 */
static int16_t _DMX512_scene_pool_search(uint16_t id){
	for(uint16_t i=0; i<_count; i++){
		if(id == _instances[i].id){
			return i;
		}
	}
	return -1;
}
