/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependencies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <math.h>
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_chaser_step.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @ingroup DMX512_chaser_step
 * @fn _DMX512_step_fadein
 * @brief Sets a chaser step's fixtures channel values to their predefined preset values according to time
 *
 * @param this the chaser step instance to be faded in
 */
static void _DMX512_step_fadein(DMX512_chaser_step_s *this){
	for(uint16_t i=0; i<this->scene->preset_count; i++){
		for(uint16_t j=0; j<this->scene->presets[i].ch_count; j++){
			uint16_t addr 	= this->scene->presets[i].fixture->addr + this->scene->presets[i].channels[j];
			uint16_t value 	= this->scene->presets[i].values[j];
			uint32_t time 	= DMX512_utils_mschronometer_get_elapsed_ms(&this->mschronometer);
			if(time < this->fadein_time){
				value = floor(((float)value/(float)this->fadein_time)*((float)(time)));
			}else{
				this->state = DMX512_CHASER_STEP_HOLD;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}

/**
 * @ingroup DMX512_chaser_step
 * @fn _DMX512_step_hold
 * @brief Holds a chaser step's faded in values during a certain amount of time
 *
 * @param this the chaser step instance to be held
 */
static void _DMX512_step_hold(DMX512_chaser_step_s *this){
	if(DMX512_utils_mschronometer_get_elapsed_ms(&this->mschronometer) > this->hold_time){
		this->state = DMX512_CHASER_STEP_FADE_OUT;
	}
}

/**
 * @ingroup DMX512_chaser_step
 * @fn _DMX512_step_fadeout
 * @brief Sets a chaser step's fixtures channel values to their "OFF" values according to time
 *
 * @param this the chaser step instance to be faded out
 */
static void _DMX512_step_fadeout(DMX512_chaser_step_s *this){
	for(uint16_t i=0; i<this->scene->preset_count; i++){
		for(uint16_t j=0; j<this->scene->presets[i].ch_count; j++){
			uint16_t addr 	= this->scene->presets[i].fixture->addr + this->scene->presets[i].channels[j];
			uint16_t value 	= this->scene->presets[i].values[j];
			uint32_t time 	= DMX512_utils_mschronometer_get_elapsed_ms(&this->mschronometer);
			if(time < this->fadeout_time){
				value = floor(((float)value/(float)this->fadeout_time)*((float)(time + value)));
			}else{
				this->state = DMX512_CHASER_STEP_IDLE;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

/**
 * @ingroup DMX512_chaser_step
 * @fn DMX512_chaser_step_init
 * @brief Creates a new chaser step instance
 *
 * @param *scene pointer to the scene instance to be referenced
 * @param fadein chaser step fade-in time in miliseconds
 * @param fadeout chaser step fade-out time in miliseconds
 * @param hold chaser step hold time in miliseconds
 * @return DMX512_chaser_step_s the chaser step instance
 */
DMX512_chaser_step_s DMX512_chaser_step_init(DMX512_scene_s *scene, uint16_t fadein_time, uint16_t fadeout_time, uint16_t hold_time){

	DMX512_chaser_step_s this = DMX512_CHASER_STEP_DEFAULT;

	if(scene != NULL){
		this.scene   		= scene;
		this.fadein_time  	= fadein_time;
		this.fadeout_time 	= fadeout_time;
		this.hold_time 	 	= hold_time;
		this.status  		= DMX512_CHASER_STEP_INITIALISED;
	}

	return this;

}

/**
 * @ingroup DMX512_chaser_step
 * @fn DMX512_chaser_step_manage
 * @brief Chaser step state machine. Manage a chaser step's state over time.
 *
 * @param this the chaser step instance to be held
 */
void DMX512_chaser_step_manage(DMX512_chaser_step_s *this){
	switch(this->state){
		case DMX512_CHASER_STEP_FADE_IN:  _DMX512_step_fadein(this); 	break;
		case DMX512_CHASER_STEP_HOLD:	  _DMX512_step_hold(this); 		break;
		case DMX512_CHASER_STEP_FADE_OUT: _DMX512_step_fadeout(this); 	break;
		case DMX512_CHASER_STEP_IDLE:	/*TODO: handle IDLE case ?*/	break;
	}
}

/**
 * @ingroup DMX512_chaser_step
 * @fn DMX512_chaser_step_start
 * @brief Starts a chaser step by assigning it to its fade_in state.
 *
 * @param this the chaser step instance to be started
 */
void DMX512_chaser_step_start(DMX512_chaser_step_s *this){
	DMX512_utils_mschronometer_reset(&this->mschronometer);
	this->state = DMX512_CHASER_STEP_FADE_IN;
}

/**
 * @ingroup DMX512_chaser_step
 * @fn DMX512_chaser_step_stop
 * @brief Stops a chaser step by assigning it to its idle state.
 *
 * @param this the chaser step instance to be stopped
 */
void DMX512_chaser_step_stop(DMX512_chaser_step_s *this){
	DMX512_utils_mschronometer_reset(&this->mschronometer);
	this->state = DMX512_CHASER_STEP_IDLE;
}

#endif
