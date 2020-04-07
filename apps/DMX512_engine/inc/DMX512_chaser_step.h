/**
 * @ingroup DMX512_chaser
 * @defgroup DMX512_chaser_step Chaser Step
 * A chaser step represents a scene with additional information regarding
 * its timing. Information such as fade in/out and hold time are used
 * by a chaser to affect timings of scene's playing sequence.
 * @{
 */

#ifndef DMX512_CHASER_STEP_H_
#define DMX512_CHASER_STEP_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_defs.h"
#include "DMX512_scene.h"
#include "ms_chronometer.h"


/**
 * @brief chaser step instance default values
 *
 * Used during initialisation of a chaser step instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define DMX512_CHASER_STEP_DEFAULT {NULL, 0, 0, 0, {}, DMX512_CHASER_STEP_UNINITIALISED, DMX512_CHASER_STEP_IDLE}


/**
 * @enum DMX512_fixture_chaser_step_status_e
 * @brief Status of a chaser step instance
 *
 * Defines whether or not a chaser step instance has been correctly initialised.
 */
typedef enum{
	DMX512_CHASER_STEP_UNINITIALISED,						/**< The chaser step is initialised*/
	DMX512_CHASER_STEP_INITIALISED							/**< The chaser step is uninitialised*/
}DMX512_fixture_chaser_step_status_e;

/**
 * @enum DMX512_fixture_chaser_step_state_e
 * @brief Current playing state of a chaser step
 *
 * Used by the chaser step state machine to determine the current operation to
 * be done on the chaser  step based on its current state value.
 */
typedef enum{
	DMX512_CHASER_STEP_FADE_IN,								/**< The chaser step is fading in*/
	DMX512_CHASER_STEP_HOLD,								/**< The chaser step is holding its values*/
	DMX512_CHASER_STEP_FADE_OUT,							/**< The chaser step is fadeing out*/
	DMX512_CHASER_STEP_IDLE,								/**< The chaser step finished playing*/
}DMX512_fixture_chaser_step_state_e;


/**
 * @struct DMX512_chaser_step_s
 * @brief DMX512 chaser step structure object
 */
typedef struct{
	DMX512_scene_s *scene;									/**< Pointer to a scene instance*/
	uint16_t fadein_time;									/**< Step fade in time in ms*/
	uint16_t fadeout_time;									/**< Step fade out time in ms*/
	uint16_t hold_time;										/**< Step hold time in ms*/
	ms_chronometer_s ms_chronometer;						/**< Step chronometer used to keep track of time during play-time*/
	DMX512_fixture_chaser_step_status_e status;				/**< Step initilisation status @see DMX512_fixture_chaser_step_status_e*/
	DMX512_fixture_chaser_step_state_e state;				/**< Current step playing state @see DMX512_fixture_chaser_step_state_e*/
}DMX512_chaser_step_s;


DMX512_chaser_step_s DMX512_chaser_step_init(DMX512_scene_s *scene, uint16_t fadein_time, uint16_t fadeout_time, uint16_t hold_time);
void DMX512_chaser_step_manage(DMX512_chaser_step_s *this);
void DMX512_chaser_step_start(DMX512_chaser_step_s *this);
void DMX512_chaser_step_stop(DMX512_chaser_step_s *this);

#endif

/**
 * @} Grouping in DMX512_chaser_step submodule ends
 */
