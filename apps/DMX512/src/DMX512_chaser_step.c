/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "DMX512_chaser_step.h"
#include "cmsis_os.h"


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new chaser step instance
 *
 * @param *scene pointer to the scene instance to be referenced
 * @param fadein chaser step fade-in time in miliseconds
 * @param fadeout chaser step fade-out time in miliseconds
 * @param hold chaser step hold time in miliseconds
 * @return DMX512_chaser_step_s the chaser step instance
 */
DMX512_chaser_step_s DMX512_chaser_step_init(DMX512_scene_s *scene, uint16_t fadein, uint16_t fadeout, uint16_t hold){

	DMX512_chaser_step_s this = DMX512_CHASER_STEP_DEFAULT;

	if(scene != NULL){
		this.scene   = scene;
		this.fadein  = fadein;
		this.fadeout = fadeout;
		this.hold 	 = hold;
		this.status  = DMX512_CHASER_STEP_INITIALISED;
	}

	return this;

}

void DMX512_chaser_step_manage(DMX512_chaser_step_s *this){
	switch(this->state){
		case DMX512_CHASER_STEP_FADE_IN:
			if(this->scene->state == DMX512_SCENE_IDLE){
				DMX512_utils_mschronometer_reset(&this->mschronometer);
				this->state = DMX512_CHASER_STEP_HOLD;
			}
			break;
		case DMX512_CHASER_STEP_HOLD:
			if(DMX512_utils_mschronometer_get_elapsed_ms(&this->mschronometer) > this->hold){
				this->state = DMX512_CHASER_STEP_FADE_OUT;
				DMX512_scene_stop(this->scene);
			}
			break;
		case DMX512_CHASER_STEP_FADE_OUT:
			if(this->scene->state == DMX512_SCENE_IDLE){
				this->state = DMX512_CHASER_STEP_IDLE;
			}
			break;
		case DMX512_CHASER_STEP_IDLE:
			break;
	}
}

void DMX512_chaser_step_start(DMX512_chaser_step_s *this){
	this->state = DMX512_CHASER_STEP_FADE_IN;
	DMX512_scene_start(this->scene);
}
