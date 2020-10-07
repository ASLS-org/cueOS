/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "DMX512_chaser_pool.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Finds the array index of a chaser
 *
 * @param id the chaser's identifier
 * @return int16_t the array index of the chaser.
 * -1 is returned if the chaser couldn't be found
 */
static int16_t _DMX512_chaser_pool_search(DMX512_chaser_pool_s *chaser_pool, uint16_t id){
	for(uint16_t i=0; i<chaser_pool->chaser_count; i++){
		if(id == chaser_pool->chasers[i].id){
			return i;
		}
	}
	return -1;
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 *=============================================================================================================================*/

/**
 * @brief Creates a new chaser pool instance
 *
 * @return DMX512_chaser_pool_s the created pool instance
 */
DMX512_chaser_pool_s *DMX512_chaser_pool_new(void){
	DMX512_chaser_pool_s *chaser_pool = pvPortMalloc(sizeof(DMX512_chaser_pool_s));
	chaser_pool->chasers 		= pvPortMalloc(sizeof(DMX512_chaser_s));
	chaser_pool->chaser_count 	= 0;
	return chaser_pool;
}

/**
 * @brief Adds a chaser instance into the pool
 *
 * @param *chaser_pool pointer to the chaser pool instance
 * @param chaser chaser instance to be added to the pool
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_pool_add(DMX512_chaser_pool_s *chaser_pool, DMX512_chaser_s chaser){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(_DMX512_chaser_pool_search(chaser_pool, chaser.id) >=0){
		err = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else{
		chaser_pool->chasers = (DMX512_chaser_s*) pvPortRealloc(chaser_pool->chasers, sizeof(DMX512_chaser_s) * (chaser_pool->chaser_count + 1));
		chaser_pool->chasers[chaser_pool->chaser_count] = chaser;
		chaser_pool->chaser_count++;
	}

	return err;

}

/**
 * @brief Deletes a chaser instance from the pool
 * @param *chaser_pool pointer to the chaser pool instance
 * @param id the chaser idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_pool_del(DMX512_chaser_pool_s *chaser_pool, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_chaser_pool_search(chaser_pool, id);

	if(index >= 0){
		for(uint16_t i=index+1; i<chaser_pool->chaser_count; i++){
				chaser_pool->chasers[i-1] = chaser_pool->chasers[i];
		}
		chaser_pool->chaser_count--;
		chaser_pool->chasers = pvPortRealloc(chaser_pool->chasers, sizeof(DMX512_chaser_s) * (chaser_pool->chaser_count));
	}else{
		err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	}

	return err;

}

/**
 * @brief Gets a chaser instance from the pool
 * @param *chaser_pool pointer to the chaser pool instance
 * @param id the chaser identifier
 * @return *DMX512_chaser_s pointer to the chaser instance
 */
DMX512_engine_err_e DMX512_chaser_pool_get(DMX512_chaser_pool_s *chaser_pool, uint16_t id, DMX512_chaser_s **chaser){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;

	int16_t index = _DMX512_chaser_pool_search(chaser_pool, id);

	if(index >= 0){
		*chaser = &chaser_pool->chasers[index];
		err = DMX512_ENGINE_OK;
	}

	//TODO: return error here

}

/**
 * @brief Manages execution of chaser instances for a whole pool
 *
 * @param chaser_pool pointer to a chaser pool instance
 */
void DMX512_chaser_pool_manage(DMX512_chaser_pool_s *chaser_pool){
	for(uint16_t i=0; i<chaser_pool->chaser_count; i++){
		DMX512_chaser_manage(&chaser_pool->chasers[i]);
	}
}

/**
 * @brief Frees instance pool
 *
 * @param chaser_pool pointer to a chaser pool instance
 */
void DMX512_chaser_pool_free(DMX512_chaser_pool_s *chaser_pool){
	if(chaser_pool != NULL){
		vPortFree(chaser_pool->chasers);
		vPortFree(chaser_pool);
	}
}

#endif
