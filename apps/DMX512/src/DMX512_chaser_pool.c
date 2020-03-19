/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include"DMX512_chaser_pool.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Finds the array index of a chaser
 *
 * @param id the chaser's identifier
 * @return int16_t the array index of the chaser.
 * -1 is returned if the chaser couldn't be found
 */
static int16_t _DMX512_chaser_pool_search(DMX512_chaser_pool_s *this, uint16_t id){
	for(uint16_t i=0; i<this->chaser_count; i++){
		if(id == this->chasers[i].id){
			return i;
		}
	}
	return -1;
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser_pool.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new chaser pool instance
 * @return DMX512_chaser_pool_s the created pool instance
 */
DMX512_chaser_pool_s *DMX512_chaser_pool_new(void){
	DMX512_chaser_pool_s *this = pvPortMalloc(sizeof(DMX512_chaser_pool_s));
	this->chasers 		= NULL;
	this->chaser_count 	= 0;
	return this;
}

/**
 * Adds a chaser instance into the pool
 *
 * @param id the chaser's idendifier
 * @param addr chaser's first channel address
 * @param ch_stop chaser's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_pool_add(DMX512_chaser_pool_s *this, DMX512_chaser_s chaser){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	//TODO: check for chaser validity
	if(_DMX512_chaser_pool_search(this, chaser.id) >=0){
		err = DMX512_CHASER_DUP;
	}else{
		this->chasers = (DMX512_chaser_s*) pvPortRealloc(this->chasers, sizeof(DMX512_chaser_s) * (this->chaser_count + 1));
		this->chasers[this->chaser_count] = chaser;
		this->chaser_count++;
	}

	return err;

}

/**
 * Deletes a chaser instance from the pool
 *
 * @param id the chaser's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_pool_del(DMX512_chaser_pool_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_chaser_pool_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i<this->chaser_count; i++){
				this->chasers[i-1] = this->chasers[i];
		}
		this->chaser_count--;
		this->chasers = pvPortRealloc(this->chasers, sizeof(DMX512_chaser_s) * (this->chaser_count));
	}else{
		err = DMX512_FIXTURE_UNKNW;
	}

	return err;

}

/**
 * Gets a chaser instance from the pool
 *
 * @param id the chaser's identifier
 * @return *DMX512_chaser_s pointer to the chaser instance
 */
DMX512_chaser_s *DMX512_chaser_pool_get(DMX512_chaser_pool_s *this, uint16_t id){
	int16_t index = _DMX512_chaser_pool_search(this, id);
	if(index >= 0){
		return &this->chasers[index];
	}else{
		return NULL;
	}

}

/**
 * Manages execution of chaser instances for a whole pool
 *
 *@param this pointer to a chaser pool instance
 */
void DMX512_chaser_pool_manage(DMX512_chaser_pool_s *this){
	for(uint16_t i=0; i<this->chaser_count; i++){
		DMX512_chaser_manage(&this->chasers[i]);
	}
}
