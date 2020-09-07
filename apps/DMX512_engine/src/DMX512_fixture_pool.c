/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "DMX512_fixture_pool.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Finds the array index of a fixture
 *
 * @param *fixture_pool pointer to the fixture pool
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_fixture_pool_search(DMX512_fixture_pool_s *fixture_pool, uint16_t id){
	for(uint16_t i=0; i<fixture_pool->fixture_count; i++){
		if(id == fixture_pool->fixtures[i].id){
			return i;
		}
	}
	return -1;
}

/**
 * @brief Checks if a fixture can be added into the pool
 * @param *fixture_pool pointer to the fixture pool
 * @param fixture fixture instance to be checked
 * @return DMX512_engine_err_e error code following the function call
 */
static uint8_t _DMX512_fixture_pool_check(DMX512_fixture_pool_s *fixture_pool, DMX512_fixture_s fixture){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(_DMX512_fixture_pool_search(fixture_pool, fixture.id) >= 0 ){
		err = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else if(fixture.ch_stop > DMX512_CHANNEL_ADDRESS_MAX){
		err = DMX512_ENGINE_INSTANCE_INVALID;
	}else{
		for(uint16_t i=0; i<fixture_pool->fixture_count; i++){
			if(fixture.addr <= fixture_pool->fixtures[i].ch_stop &&
			   fixture_pool->fixtures[i].addr <= fixture.ch_count + fixture.addr - 1){
				err = DMX512_ENGINE_INSTANCE_INVALID;
			}
		}
	}

	return err;

}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_fixture_pool.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Creates a new fixture pool instance
 *
 * @return DMX512_fixture_pool_s the created pool instance
 */
DMX512_fixture_pool_s *DMX512_fixture_pool_new(void){
	DMX512_fixture_pool_s *fixture_pool = pvPortMalloc(sizeof(DMX512_fixture_pool_s));
	fixture_pool->fixtures 		= NULL;
	fixture_pool->fixture_count = 0;
	return fixture_pool;
}

/**
 * @brief Adds a fixture instance into the pool
 *
 * @param *fixture_pool pointer to the fixture pool
 * @param fixture fixture instance to be added into the pool
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_fixture_pool_add(DMX512_fixture_pool_s *fixture_pool, DMX512_fixture_s fixture){

	//TODO: check if fixture instance is valid first
	DMX512_engine_err_e err = _DMX512_fixture_pool_check(fixture_pool, fixture);

	if(err == DMX512_ENGINE_OK){
		fixture_pool->fixtures = (DMX512_fixture_s*) pvPortRealloc(fixture_pool->fixtures, sizeof(DMX512_fixture_s) * (fixture_pool->fixture_count + 1));
		fixture_pool->fixtures[fixture_pool->fixture_count] = fixture;
		fixture_pool->fixture_count++;
	}

	return err;

}

/**
 * @brief Deletes a fixture instance from the pool
 * @param *fixture_pool pointer to the fixture pool
 * @param id fixture idendifier of the fixture to be removed from the pool
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_fixture_pool_del(DMX512_fixture_pool_s *fixture_pool, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	int16_t index = _DMX512_fixture_pool_search(fixture_pool, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< fixture_pool->fixture_count; i++){
			fixture_pool->fixtures[i-1] = fixture_pool->fixtures[i];
		}
		fixture_pool->fixture_count--;
		fixture_pool->fixtures = pvPortRealloc(fixture_pool->fixtures, sizeof(DMX512_fixture_s) * (fixture_pool->fixture_count));
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * @brief Gets a fixture instance from the pool
 *
 * @param *fixture_pool pointer to the fixture pool
 * @param id id of the fixture to be searched
 * @param **fixture pointer to the fixture instance to be returned
 * @return DMX512_engine_err_e ERR_OK if fixture was found, DMX512_ENGINE_INSTANCE_UNDEFINED otherwise
 */
DMX512_engine_err_e DMX512_fixture_pool_get(DMX512_fixture_pool_s *fixture_pool, uint16_t id, DMX512_fixture_s **fixture){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;

	int16_t index = _DMX512_fixture_pool_search(fixture_pool, id);

	if(index >= 0){
		*fixture = &fixture_pool->fixtures[index];
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * @brief Frees instance pool
 *
 * @return DMX512_fixture_pool_s the created pool instance
 */
void DMX512_fixture_pool_free(DMX512_fixture_pool_s *fixture_pool){
	vPortFree(fixture_pool->fixtures);
	vPortFree(fixture_pool);
}

#endif
