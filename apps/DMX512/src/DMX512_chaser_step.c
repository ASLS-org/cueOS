#include "DMX512_chaser_step.h"
#include "cmsis_os.h"

//TODO: create linked list of steps for => easier time management
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


