/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include"DMX512_fixture_pool.h"


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
static int16_t _DMX512_fixture_pool_search(DMX512_fixture_pool_s *this, uint16_t id){
	for(uint16_t i=0; i<this->fixture_count; i++){
		if(id == this->fixtures[i].id){
			return i;
		}
	}
	return -1;
}

/**
 * Checks if a fixture can be added into the pool
 *
 * @param id the fixture's idendifier
 * @param addr fixture's first channel address
 * @param ch_stop fixture's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
static DMX512_engine_err_e _DMX512_fixture_pool_check(DMX512_fixture_pool_s *this, DMX512_fixture_s fixture){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(_DMX512_fixture_pool_search(this, fixture.id) >= 0){
		err  = DMX512_FIXTURE_DUP;
	}else if(fixture.addr < DMX512_CHANNEL_ADDRESS_MIN || fixture.ch_count + fixture.addr - 1 > DMX512_CHANNEL_ADDRESS_MAX){
		err  = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
	}else{
		for(uint16_t i=0; i<this->fixture_count; i++){
			if(fixture.addr <= this->fixtures[i].ch_stop && this->fixtures[i].addr <=  fixture.ch_count + fixture.addr - 1){
				err = DMX512_CHANNEL_ADDRESS_DUP;
				break;
			}
		}
	}

	return err;

}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_fixture_pool.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new fixture pool instance
 * @return DMX512_fixture_pool_s the created pool instance
 */
DMX512_fixture_pool_s *DMX512_fixture_pool_new(void){
	DMX512_fixture_pool_s *this = pvPortMalloc(sizeof(DMX512_fixture_pool_s));
	this->fixtures 		= NULL;
	this->fixture_count 	= 0;
	return this;
}

/**
 * Adds a fixture instance into the pool
 *
 * @param id the fixture's identifier
 * @param addr fixture's first channel address
 * @param ch_stop fixture's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_fixture_pool_add(DMX512_fixture_pool_s *this, DMX512_fixture_s fixture){

	//TODO: check if fixture instance is valid first
	DMX512_engine_err_e err = _DMX512_fixture_pool_check(this, fixture);;

	if(err == DMX512_ENGINE_OK){
		this->fixtures = (DMX512_fixture_s*) pvPortRealloc(this->fixtures, sizeof(DMX512_fixture_s) * (this->fixture_count + 1));
		this->fixtures[this->fixture_count] = fixture;
		this->fixture_count++;
	}

	return err;

}

/**
 * Deletes a fixture instance from the pool
 *
 * @param id the fixture's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_fixture_pool_del(DMX512_fixture_pool_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_fixture_pool_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i<this->fixture_count; i++){
				this->fixtures[i-1] = this->fixtures[i];
		}
		this->fixture_count--;
		this->fixtures = pvPortRealloc(this->fixtures, sizeof(DMX512_fixture_s) * (this->fixture_count));
	}else{
		err = DMX512_FIXTURE_UNKNW;
	}

	return err;

}

/**
 * Gets a fixture instance from the pool
 *
 * @param id the fixture's identifier
 * @return *DMX512_fixture_s pointer to the fixture instance
 */
DMX512_fixture_s *DMX512_fixture_pool_get(DMX512_fixture_pool_s *this, uint16_t id){
	int16_t index = _DMX512_fixture_pool_search(this, id);
	if(index >= 0){
		return &this->fixtures[index];
	}else{
		return NULL;
	}

}
