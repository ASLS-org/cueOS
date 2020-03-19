/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "DMX512_chaser.h"
#include "DMX512_driver.h"
#include "cmsis_os.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Finds the array index of a fixture preset
 *
 * @param id the fixture preset's identifier
 * @return int16_t the array index of the fixture preset.
 * -1 is returned if the fixture preset couldn't be found
 */
static int16_t _DMX512_chaser_search(DMX512_chaser_s *this, uint16_t fixture_id){
	for(uint16_t i=0; i<this->step_count; i++){
		if(fixture_id == this->steps[i].scene->id){
			return i;
		}
	}
	return -1;
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

//TODO: Implement chaser fade-in fade-out

/**
 * Creates and initialises a new chaser instance
 *
 * @param id the chaser identifier (@see DMX512_chaser_pool.h)
 * @param mode the chaser trigger mode
 * @param dir the chaser step play direction
 * @return DMX512_chaser_s the created chaser
 */
DMX512_chaser_s DMX512_chaser_new(uint16_t id, DMX512_chaser_step_mode_e mode, DMX512_chaser_step_direction_e dir){
	DMX512_chaser_s this = DMX512_CHASER_DEFAULT;
	this.id			  = id;
	this.mode 		  = mode;
	this.dir  		  = dir;
	this.current_step = 0;
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
DMX512_engine_err_e DMX512_chaser_add_step(DMX512_chaser_s *this, DMX512_chaser_step_s step){

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
DMX512_engine_err_e DMX512_chaser_del_step(DMX512_chaser_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_INVALID_CHASER_STEP;
	int16_t index = _DMX512_chaser_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< this->step_count; i++){
			this->steps[i-1] = this->steps[i];
		}
		this->step_count--;
		//TODO: Maybe stop the step and/or create free function to clear instance safely
		this->steps = pvPortRealloc(this->steps, sizeof(DMX512_chaser_step_s) * (this->step_count));
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
DMX512_chaser_step_s *DMX512_chaser_get_step(DMX512_chaser_s *this, uint16_t id){
	int16_t index = _DMX512_chaser_search(this, id);
	if(index >= 0){
		return &this->steps[index];
	}else{
		return NULL;
	}
}

/**
 *
 * Handles step selection and trigger over time
 *
 *@param this pointer to the chaser instance
 */
//TODO: tidy this !
void DMX512_chaser_manage(DMX512_chaser_s *this){
	if(this->state == CHASER_PLAYING){
		DMX512_chaser_step_manage(&this->steps[this->current_step]);
		if(this->steps[this->current_step].state == DMX512_CHASER_STEP_IDLE){
			switch(this->mode){
				case CHASER_MODE_LOOP:
					this->current_step = (this->current_step + 1) % this->step_count;
					DMX512_chaser_step_start(&this->steps[this->current_step]);
					break;
				case CHASER_MODE_SINGLE_SHOT:
					if(this->dir == CHASER_DIRECTION_FORWARD){
						if(this->current_step-- == 0){
							this->state = CHASER_IDLE;
						}
					}else{
						if(this->current_step++ > this->step_count){
							this->state = CHASER_IDLE;
						}
					}
					DMX512_chaser_step_start(&this->steps[this->current_step]);
					break;
				case CHASER_MODE_PING_PONG:
					break;
				case CHASER_MODE_RANDOM:
					break;
			}
		}
	}
}

/**
 * Starts a scene fade-in process
 *
 * @param this pointer to the scene instance
 */
void DMX512_chaser_start(DMX512_chaser_s *this){
	this->current_step = this->dir == CHASER_DIRECTION_FORWARD ? 0 : this->step_count;
	DMX512_chaser_step_start(&this->steps[this->current_step]);
	this->state = CHASER_PLAYING;
}
