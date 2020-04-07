/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <math.h>
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_chaser.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Finds the array index of a fixture preset
 *
 * @param id the fixture preset's identifier
 * @return int16_t the array index of the fixture preset.
 * -1 is returned if the fixture preset couldn't be found
 */
static int16_t _DMX512_chaser_search(DMX512_chaser_s *chaser, uint16_t fixture_id){
	for(uint16_t i=0; i<chaser->step_count; i++){
		if(fixture_id == chaser->steps[i].scene->id){
			return i;
		}
	}
	return -1;
}

/**
 * @brief Iterates throug steps in ascending order
 *
 * @param chaser pointer to the chaser instance
 */
static void _DMX512_chaser_iterate_steps_forward(DMX512_chaser_s *chaser){
	chaser->current_step = (chaser->current_step + 1) % chaser->step_count;
	DMX512_chaser_step_start(&chaser->steps[chaser->current_step]);
	chaser->state = ( chaser->mode == DMX512_CHASER_MODE_SINGLE_SHOT && chaser->current_step == chaser->step_count - 1) ? DMX512_CHASER_IDLE : DMX512_CHASER_PLAYING;
}

/**
 * @brief Iterates through steps in descending order
 *
 * @param chaser pointer to the chaser instance
 */
static void _DMX512_chaser_iterate_steps_backward(DMX512_chaser_s *chaser){
	chaser->current_step = chaser->current_step > 0 ? chaser->current_step - 1 : chaser->step_count - 1;
	DMX512_chaser_step_start(&chaser->steps[chaser->current_step]);
	chaser->state = ( chaser->mode == DMX512_CHASER_MODE_SINGLE_SHOT && chaser->current_step == 0 ) ? DMX512_CHASER_IDLE : DMX512_CHASER_PLAYING;
}

/**
 * @brief Iterates through steps back and forth
 *
 * @param chaser pointer to the chaser instance
 */
static void _DMX512_chaser_iterate_steps_pingpong(DMX512_chaser_s *chaser){
	if(chaser->pingpong == DMX512_CHASER_PINGPONG_PING){
		chaser->current_step = (chaser->current_step + 1);
		DMX512_chaser_step_start(&chaser->steps[chaser->current_step]);
		chaser->pingpong = chaser->current_step >= chaser->step_count -1 ? DMX512_CHASER_PINGPONG_PONG : DMX512_CHASER_PINGPONG_PING;
	}else{
		chaser->current_step = chaser->current_step > 0 ? chaser->current_step - 1 : chaser->step_count - 1;
		DMX512_chaser_step_start(&chaser->steps[chaser->current_step]);
		chaser->pingpong = chaser->current_step == 0  ? DMX512_CHASER_PINGPONG_PING : DMX512_CHASER_PINGPONG_PONG;
		chaser->state = ( chaser->mode == DMX512_CHASER_MODE_SINGLE_SHOT && chaser->current_step == 0 ) ? DMX512_CHASER_IDLE : DMX512_CHASER_PLAYING;
	}
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

//TODO: Implement chaser fade-in fade-out

/**
 * @brief Creates and initialises a new chaser instance
 *
 * @param id the chaser identifier (@see DMX512_chaser_pool.h)
 * @param mode the chaser trigger mode
 * @param direction the chaser step play direction
 * @return DMX512_chaser_s the created chaser
 */
DMX512_chaser_s DMX512_chaser_new(uint16_t id, DMX512_chaser_mode_e mode, DMX512_chaser_direction_e direction){
	DMX512_chaser_s chaser = DMX512_CHASER_DEFAULT;
	chaser.id	   		= id;
	chaser.mode  		= mode;
	chaser.direction  = direction;
	chaser.status 	= DMX512_CHASER_INITIALISED;
	return chaser;
}

/**
 * @brief Adds a scene preset instance into the chaser
 *
 * @param chaser pointer to the chaser instance
 * @param step step instance to be added into the chaser @see DMX512_step
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_add_step(DMX512_chaser_s *chaser, DMX512_chaser_step_s step){

	DMX512_engine_err_e err   = DMX512_ENGINE_OK;

	if(step.status != DMX512_CHASER_STEP_INITIALISED){
		err = DMX512_ENGINE_INSTANCE_INVALID;
	}else if(_DMX512_chaser_search(chaser, step.scene->id) >= 0){
		err = DMX512_ENGINE_INSTANCE_DUPLICATE;
	}else{
		chaser->steps = (DMX512_chaser_step_s*) pvPortRealloc(chaser->steps, sizeof(DMX512_chaser_step_s) * (chaser->step_count + 1));
		chaser->steps[chaser->step_count] = step;
		chaser->step_count++;
	}

	return err;

}

/**
 * @brief Deletes a fixture preset instance from the chaser
 *
 * @param chaser pointer to the chaser instance
 * @param id the chaser step idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_chaser_del_step(DMX512_chaser_s *chaser, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_INSTANCE_UNDEFINED;
	int16_t index = _DMX512_chaser_search(chaser, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< chaser->step_count; i++){
			chaser->steps[i-1] = chaser->steps[i];
		}
		chaser->step_count--;
		chaser->steps = pvPortRealloc(chaser->steps, sizeof(DMX512_chaser_step_s) * (chaser->step_count));
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * @brief Gets a fixture instance from the pool
 *
 * @param chaser pointer to the chaser instance
 * @param id the chaser step identifier
 * @return *DMX512_fixture_preset_s pointer to the fixture preset instance
 */
DMX512_chaser_step_s *DMX512_chaser_get_step(DMX512_chaser_s *chaser, uint16_t id){
	int16_t index = _DMX512_chaser_search(chaser, id);
	if(index >= 0){
		return &chaser->steps[index];
	}else{
		return NULL;
	}
}

/**
 * @brief Handles step selection and trigger over time
 *
 * @param chaser pointer to the chaser instance
 */
void DMX512_chaser_manage(DMX512_chaser_s *chaser){
	if(chaser->state == DMX512_CHASER_PLAYING){
		DMX512_chaser_step_manage(&chaser->steps[chaser->current_step]);
		if(chaser->steps[chaser->current_step].state == DMX512_CHASER_STEP_IDLE){
			switch(chaser->direction){
				case DMX512_CHASER_DIRECTION_FORWARD:  _DMX512_chaser_iterate_steps_forward(chaser); 	break;
				case DMX512_CHASER_DIRECTION_BACKWARD: _DMX512_chaser_iterate_steps_backward(chaser); break;
				case DMX512_CHASER_DIRECTION_PINGPONG: _DMX512_chaser_iterate_steps_pingpong(chaser);	break;
				case DMX512_CHASER_DIRECTION_RANDOM: /*TODO: handle RNG for random step iteration*/	break;
			}
		}
	}
}

/**
 * @brief Starts a scene fade-in process
 *
 * @param chaser pointer to the scene instance
 */
void DMX512_chaser_start(DMX512_chaser_s *chaser){
	//TODO: see if current step preselection could be handled better
	chaser->current_step = chaser->direction == DMX512_CHASER_DIRECTION_BACKWARD ? chaser->step_count - 1 : 0;
	DMX512_chaser_step_start(&chaser->steps[chaser->current_step]);
	chaser->state = DMX512_CHASER_PLAYING;
}

#endif
