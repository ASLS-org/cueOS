#ifndef DMX512_SCENE_H
#define DMX512_SCENE_H

#include <stdint.h>

#include "DMX512_defs.h"
#include "DMX512_fixture_preset.h"
#include "DMX512_utils.h"

/**
 * @ingroup DMX512_scene
 * @def DMX512_SCENE_DEFAULT
 * @brief scene instance default values
 *
 * Used during initialisation of a scene instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define DMX512_SCENE_DEFAULT {0, 0, 0, 0, NULL, {}, DMX512_SCENE_IDLE, DMX512_SCENE_UNINITIALISED}


/**
 * @ingroup DMX512_scene
 * @enum DMX512_scene_status_e
 * @brief Status of a scene instance
 *
 * Defines whether or not a scene instance has been correctly initialised.
 */
typedef enum{
	DMX512_SCENE_UNINITIALISED,
	DMX512_SCENE_INITIALISED
}DMX512_scene_status_e;

/**
 * @ingroup DMX512_scene
 * @enum DMX512_scene_state_e
 * @brief Current playing state of a scene
 *
 * Used by the scene state machine to determine the current operation to
 * be done on the scene based on its curent state value.
 */
typedef enum{
	DMX512_SCENE_IDLE,
	DMX512_SCENE_FADEIN,
	DMX512_SCENE_FADEOUT,
}DMX512_scene_state_e;


/**
 * @ingroup DMX512_scene
 * @struct DMX512_scene_s
 * @brief Defines a DMX512 scene object
 *
 * Scenes fundamentally define a set of fixtures pre-defined values to be transferred over
 * to the DMX512 driver buffer at given any time. A scene's fixture preset values may be
 * faded in and out over time.
 */
typedef struct DMX512_scene{
	uint16_t id;									/*< The scene's unique identifier*/
	uint16_t fadein_time;							/*< Fadein time in ms*/
	uint16_t fadeout_time;							/*< Fadeout time in ms*/
	uint16_t preset_count;							/*< Count of current fixture presets associated to the scene*/
	DMX512_fixture_preset_s *presets;				/*< List of current fixture presets associated to the scene*/
	DMX512_utils_mschronometer_s mschronometer;		/*< Scene chronometer used to keep track of time during play-time*/
	DMX512_scene_state_e state;						/*< Current play-state of the scene @see DMX512_scene_state_e*/
	DMX512_scene_status_e status;					/*< Scene initialisation status @see DMX512_scene_state_e*/
}DMX512_scene_s;


DMX512_scene_s DMX512_scene_new(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time);
DMX512_engine_err_e DMX512_scene_add_preset(DMX512_scene_s *this, DMX512_fixture_preset_s fp);
DMX512_engine_err_e DMX512_scene_del_preset(DMX512_scene_s *this, uint16_t id);
DMX512_fixture_preset_s *DMX512_scene_get_preset(DMX512_scene_s *this, uint16_t id);
void DMX512_scene_start(DMX512_scene_s *this);
void DMX512_scene_stop(DMX512_scene_s *this);
void DMX512_scene_manage(DMX512_scene_s *this);

#endif
