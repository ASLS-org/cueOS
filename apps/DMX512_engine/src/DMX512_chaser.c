/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <math.h>
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_chaser.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @ingroup DMX512_chaser
 * @fn _DMX512_chaser_search
 * @briefFinds the array index of a fixture preset
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

/**
 * @ingroup DMX512_chaser
 * @fn _DMX512_chaser_iterate_steps_forward
 * @brief Iterates throug steps in ascending order
 *
 * @param this pointer to the chaser instance
 */
static void _DMX512_chaser_iterate_steps_forward(DMX512_chaser_s *this){
	this->current_step = (this->current_step + 1) % this->step_count;
	DMX512_chaser_step_start(&this->steps[this->current_step]);
	this->state = ( this->mode == DMX512_CHASER_MODE_SINGLE_SHOT && this->current_step == this->step_count - 1) ? DMX512_CHASER_IDLE : DMX512_CHASER_PLAYING;
}

/**
 * @ingroup DMX512_chaser
 * @fn _DMX512_chaser_iterate_steps_backward
 * @brief Iterates through steps in descending order
 *
 * @param this pointer to the chaser instance
 */
static void _DMX512_chaser_iterate_steps_backward(DMX512_chaser_s *this){
	this->current_step = this->current_step > 0 ? this->current_step - 1 : this->step_count - 1;
	DMX512_chaser_step_start(&this->steps[this->current_step]);
	this->state = ( this->mode == DMX512_CHASER_MODE_SINGLE_SHOT && this->current_step == 0 ) ? DMX512_CHASER_IDLE : DMX512_CHASER_PLAYING;
}

/**
 * @ingroup DMX512_chaser
 * @fn _DMX512_chaser_iterate_steps_pingpong
 * @brief Iterates through steps back and forth
 *
 * @param this pointer to the chaser instance
 */
static void _DMX512_chaser_iterate_steps_pingpong(DMX512_chaser_s *this){
	if(this->pingpong == DMX512_CHASER_PINGPONG_PING){
		this->current_step = (this->current_step + 1);
		DMX512_chaser_step_start(&this->steps[this->current_step]);
		this->pingpong = this->current_step >= this->step_count -1 ? DMX512_CHASER_PINGPONG_PONG : DMX512_CHASER_PINGPONG_PING;
	}else{
		this->current_step = this->current_step > 0 ? this->current_step - 1 : this->step_count - 1;
		DMX512_chaser_step_start(&this->steps[this->current_step]);
		this->pingpong = this->current_step == 0  ? DMX512_CHASER_PINGPONG_PING : DMX512_CHASER_PINGPONG_PONG;
		this->state = ( this->mode == DMX512_CHASER_MODE_SINGLE_SHOT && this->current_step == 0 ) ? DMX512_CHASER_IDLE : DMX512_CHASER_PLAYING;
	}
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

//TODO: Implement chaser fade-in fade-out

/**
 * @ingroup DMX512_chaser
 * @fn DMX512_chaser_new
 * @brief Creates and initialises a new chaser instance
 *
 * @param id the chaser identifier (@see DMX512_chaser_pool.h)
 * @param mode the chaser trigger mode
 * @param dir the chaser step play direction
 * @return DMX512_chaser_s the created chaser
 */
DMX512_chaser_s DMX512_chaser_new(uint16_t id, DMX512_chaser_mode_e mode, DMX512_chaser_direction_e direction){
	DMX512_chaser_s this = DMX512_CHASER_DEFAULT;
	this.id	   		= id;
	this.mode  		= mode;
	this.direction  = direction;
	this.status 	= DMX512_CHASER_INITIALISED;
	return this;
}

/**
 * @ingroup DMX512_chaser
 * @fn DMX512_chaser_add_step
 * @brief Adds a scene preset instance into the chaser
 *
 * @param this pointer to the chaser instance
 * @param id the scene's idendifier
 * @param values the list of values to be stored within the chaser
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_add_step(DMX512_chaser_s *this, DMX512_chaser_step_s step){

	DMX512_engine_err_e err   = DMX512_ENGINE_OK;

	if(step.status != DMX512_CHASER_STEP_INITIALISED){
		err = DMX512_ENGINE_INSTANCE_INVALID;
	}else if(_DMX512_chaser_search(this, step.scene->id) >= 0){
		err = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else{
		this->steps = (DMX512_chaser_step_s*) pvPortRealloc(this->steps, sizeof(DMX512_chaser_step_s) * (this->step_count + 1));
		this->steps[this->step_count] = step;
		this->step_count++;
	}

	return err;

}

/**
 * @ingroup DMX512_chaser
 * @fn DMX512_chaser_del_step
 * @brief Deletes a fixture preset instance from the chaser
 *
 * @param id the fixture preset's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_del_step(DMX512_chaser_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	int16_t index = _DMX512_chaser_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< this->step_count; i++){
			this->steps[i-1] = this->steps[i];
		}
		this->step_count--;
		this->steps = pvPortRealloc(this->steps, sizeof(DMX512_chaser_step_s) * (this->step_count));
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * @ingroup DMX512_chaser
 * @fn DMX512_chaser_get_step
 * @brief Gets a fixture instance from the pool
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
 * @ingroup DMX512_chaser
 * @fn DMX512_chaser_manage
 * @brief Handles step selection and trigger over time
 *
 * @param this pointer to the chaser instance
 */
void DMX512_chaser_manage(DMX512_chaser_s *this){
	if(this->state == DMX512_CHASER_PLAYING){
		DMX512_chaser_step_manage(&this->steps[this->current_step]);
		if(this->steps[this->current_step].state == DMX512_CHASER_STEP_IDLE){
			switch(this->direction){
				case DMX512_CHASER_DIRECTION_FORWARD:  _DMX512_chaser_iterate_steps_forward(this); 	break;
				case DMX512_CHASER_DIRECTION_BACKWARD: _DMX512_chaser_iterate_steps_backward(this); break;
				case DMX512_CHASER_DIRECTION_PINGPONG: _DMX512_chaser_iterate_steps_pingpong(this);	break;
				case DMX512_CHASER_DIRECTION_RANDOM: /*TODO: handle RNG for random step iteration*/	break;
			}
		}
	}
}

/**
 * @ingroup DMX512_chaser
 * @fn DMX512_chaser_start
 * @brief Starts a scene fade-in process
 *
 * @param this pointer to the scene instance
 */
void DMX512_chaser_start(DMX512_chaser_s *this){
	//TODO: see if current step preselection could be handled better
	this->current_step = this->direction == DMX512_CHASER_DIRECTION_BACKWARD ? this->step_count - 1 : 0;
	DMX512_chaser_step_start(&this->steps[this->current_step]);
	this->state = DMX512_CHASER_PLAYING;
}

#endif
