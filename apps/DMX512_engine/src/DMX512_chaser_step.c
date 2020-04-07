/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependencies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <math.h>
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_chaser_step.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Sets a chaser step's fixtures channel values to their predefined preset values according to time
 *
 * @param chaser_step the chaser step instance to be faded in
 */
static void _DMX512_step_fadein(DMX512_chaser_step_s *chaser_step){
	for(uint16_t i=0; i<chaser_step->scene->fixture_preset_count; i++){
		for(uint16_t j=0; j<chaser_step->scene->fixture_presets[i].ch_count; j++){
			uint16_t addr 	= chaser_step->scene->fixture_presets[i].fixture->addr + chaser_step->scene->fixture_presets[i].channels[j];
			uint16_t value 	= chaser_step->scene->fixture_presets[i].values[j];
			uint32_t time 	= ms_chronometer_get_elapsed_ms(&chaser_step->ms_chronometer);
			if(time < chaser_step->fadein_time){
				value = floor(((float)value/(float)chaser_step->fadein_time)*((float)(time)));
			}else{
				chaser_step->state = DMX512_CHASER_STEP_HOLD;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}

/**
 * @brief Holds a chaser step's faded in values during a certain amount of time
 *
 * @param chaser_step the chaser step instance to be held
 */
static void _DMX512_step_hold(DMX512_chaser_step_s *chaser_step){
	if(ms_chronometer_get_elapsed_ms(&chaser_step->ms_chronometer) > chaser_step->hold_time){
		chaser_step->state = DMX512_CHASER_STEP_FADE_OUT;
	}
}

/**
 * @brief Sets a chaser step's fixtures channel values to their "OFF" values according to time
 *
 * @param chaser_step the chaser step instance to be faded out
 */
static void _DMX512_step_fadeout(DMX512_chaser_step_s *chaser_step){
	for(uint16_t i=0; i<chaser_step->scene->fixture_preset_count; i++){
		for(uint16_t j=0; j<chaser_step->scene->fixture_presets[i].ch_count; j++){
			uint16_t addr 	= chaser_step->scene->fixture_presets[i].fixture->addr + chaser_step->scene->fixture_presets[i].channels[j];
			uint16_t value 	= chaser_step->scene->fixture_presets[i].values[j];
			uint32_t time 	= ms_chronometer_get_elapsed_ms(&chaser_step->ms_chronometer);
			if(time < chaser_step->fadeout_time){
				value = floor(((float)value/(float)chaser_step->fadeout_time)*((float)(time + value)));
			}else{
				chaser_step->state = DMX512_CHASER_STEP_IDLE;
			}

			DMX512_driver_set_single(addr, value);
		}
	}
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 *=============================================================================================================================*/

/**
 * @brief Creates a new chaser step instance
 *
 * @param *scene pointer to the scene instance to be referenced
 * @param fadein_time chaser step fade-in time in miliseconds
 * @param fadeout_time chaser step fade-out time in miliseconds
 * @param hold_time chaser step hold time in miliseconds
 * @return DMX512_chaser_step_s the chaser step instance
 */
DMX512_chaser_step_s DMX512_chaser_step_init(DMX512_scene_s *scene, uint16_t fadein_time, uint16_t fadeout_time, uint16_t hold_time){

	DMX512_chaser_step_s chaser_step = DMX512_CHASER_STEP_DEFAULT;

	if(scene != NULL){
		chaser_step.scene   		= scene;
		chaser_step.fadein_time  	= fadein_time;
		chaser_step.fadeout_time 	= fadeout_time;
		chaser_step.hold_time 	 	= hold_time;
		chaser_step.status  		= DMX512_CHASER_STEP_INITIALISED;
	}

	return chaser_step;

}

/**
 * @brief Chaser step state machine. Manage a chaser step's state over time.
 *
 * @param *chaser_step pointer to the chaser step instance to be managed
 */
void DMX512_chaser_step_manage(DMX512_chaser_step_s *chaser_step){
	switch(chaser_step->state){
		case DMX512_CHASER_STEP_FADE_IN:  _DMX512_step_fadein(chaser_step); 	break;
		case DMX512_CHASER_STEP_HOLD:	  _DMX512_step_hold(chaser_step); 		break;
		case DMX512_CHASER_STEP_FADE_OUT: _DMX512_step_fadeout(chaser_step); 	break;
		case DMX512_CHASER_STEP_IDLE:	/*TODO: handle IDLE case ?*/	break;
	}
}

/**
 * @brief Starts a chaser step by assigning it to its fade_in state.
 *
 * @param *chaser_step pointer to the chaser step instance to be started
 */
void DMX512_chaser_step_start(DMX512_chaser_step_s *chaser_step){
	ms_chronometer_reset(&chaser_step->ms_chronometer);
	chaser_step->state = DMX512_CHASER_STEP_FADE_IN;
}

/**
 * @brief Stops a chaser step by assigning it to its idle state.
 *
 * @param *chaser_step pointer to the chaser step instance to be stopped
 */
void DMX512_chaser_step_stop(DMX512_chaser_step_s *chaser_step){
	ms_chronometer_reset(&chaser_step->ms_chronometer);
	chaser_step->state = DMX512_CHASER_STEP_IDLE;
}

#endif
