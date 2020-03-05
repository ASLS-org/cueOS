#include "DMX512_chaser.h"
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
static int16_t _DMX512_chaser_search(DMX512_chaser_s *this, uint16_t fixture_id){
	for(uint16_t i=0; i<this->step_count; i++){
		if(fixture_id == this->steps[i].scene->id){
			return i;
		}
	}
	return -1;
}

//TODO: Implement chaser fade-in fade-out

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

/**
 * Creates and initialises a new chaser instance
 *
 * @param id the chaser identifier (@see DMX512_chaser_pool.h)
 * @param mode the chaser trigger mode
 * @param dir the chaser step play direction
 * @return DMX512_chaser_s the created chaser
 */
DMX512_chaser_s DMX512_chaser_init(uint16_t id, DMX512_chaser_step_mode mode, DMX512_chaser_step_direction dir){

	DMX512_chaser_s this = DMX512_CHASER_DEFAULT;

	this.id   = id;
	this.mode = mode;
	this.dir  = dir;

	return this;

}

/**
 * Adds a scene preset instance into the chaser
 *
 * @param this pointer to the chaser instance
 * @param id the scene's idendifier
 * @param values the list of values to be stored within the chaser
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_add(DMX512_chaser_s *this, DMX512_chaser_step_s step){

	DMX512_engine_err_e err   = DMX512_ENGINE_OK;

	if(step.status == DMX512_CHASER_STEP_UNINITIALISED){
		err = DMX512_INVALID_CHASER_STEP;
	}else if(_DMX512_chaser_search(this, step.scene->id) >= 0){
		err = DMX512_DUPLICATE_CHASER_STEP;
	}else{
		this->steps = (DMX512_chaser_step_s*) pvPortRealloc(this->steps, sizeof(DMX512_chaser_step_s) * (this->step_count + 1));
		this->steps[this->step_count] = step;
		this->step_count++;
	}

	return err;

}

/**
 * Deletes a fixture preset instance from the chaser
 *
 * @param id the fixture preset's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_del(DMX512_chaser_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_INVALID_CHASER_STEP;
	int16_t index = _DMX512_chaser_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< this->step_count; i++){
			this->steps[i-1] = this->steps[i];
		}
		this->step_count--;
		//TODO: Maybe stop the step and/or create free function to clear instance safely
		this->steps = pvPortRealloc(this->steps, sizeof(DMX512_fixture_s) * (this->step_count));
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
DMX512_fixture_preset_s *DMX512_chaser_get(DMX512_chaser_s *this, uint16_t id){
	int16_t index = _DMX512_chaser_search(this, id);
	if(index >= 0){
		return &this->steps[index];
	}else{
		return NULL;
	}
}

/**
 * Triggers a DMX chaser by assinging fixture preset values to DMX512 buffer
 *
 * @param this chaser instance
 */
void DMX512_chaser_trigger(DMX512_chaser_s *this){
	//TODO: see timr implementation for fadein/out/hold
}


