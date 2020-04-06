/**
 * @ingroup DMX512_scene_pool
 * @defgroup DMX512_scene
 * Scenes fundamentally define a set of fixtures pre-defined values to be transferred over
 * to the DMX512 driver buffer at given any time. A scene's fixture preset values may be
 * faded in and out over time.
 * @{
 */

#ifndef DMX512_SCENE_H
#define DMX512_SCENE_H

#include <stdint.h>

#include "DMX512_defs.h"
#include "DMX512_fixture_preset.h"
#include "DMX512_utils.h"

/**
 * @brief scene instance default values
 *
 * Used during initialisation of a scene instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define DMX512_SCENE_DEFAULT {0, 0, 0, 0, NULL, {}, DMX512_SCENE_IDLE, DMX512_SCENE_UNINITIALISED}


/**
 * @brief Status of a scene instance
 *
 * Defines whether or not a scene instance has been correctly initialised.
 */
typedef enum{
	DMX512_SCENE_UNINITIALISED,						/**< The scene instance is initialised*/
	DMX512_SCENE_INITIALISED						/**< The scene instance is uninitialised*/
}DMX512_scene_status_e;

/**
 * @brief Current playing state of a scene
 *
 * Used by the scene state machine to determine the current operation to
 * be done on the scene based on its curent state value.
 */
typedef enum{
	DMX512_SCENE_IDLE,								/**< The scene is currently not playing*/
	DMX512_SCENE_FADEIN,							/**< Fading in fiture values*/
	DMX512_SCENE_FADEOUT,							/**< Fading out fiture values*/
}DMX512_scene_state_e;


/**
 * @brief DMX512 scene structure object
 */
typedef struct{
	uint16_t id;									/**< The scene's unique identifier*/
	uint16_t fadein_time;							/**< Fadein time in ms*/
	uint16_t fadeout_time;							/**< Fadeout time in ms*/
	uint16_t preset_count;							/**< Count of current fixture presets associated to the scene*/
	DMX512_fixture_preset_s *presets;				/**< List of current fixture presets associated to the scene*/
	DMX512_utils_mschronometer_s mschronometer;		/**< Scene chronometer used to keep track of time during play-time*/
	DMX512_scene_state_e state;						/**< Current play-state of the scene @see DMX512_scene_state_e*/
	DMX512_scene_status_e status;					/**< Scene initialisation status @see DMX512_scene_state_e*/
}DMX512_scene_s;


DMX512_scene_s DMX512_scene_new(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time);
DMX512_engine_err_e DMX512_scene_add_preset(DMX512_scene_s *scene, DMX512_fixture_preset_s fp);
DMX512_engine_err_e DMX512_scene_del_preset(DMX512_scene_s *scene, uint16_t id);
DMX512_fixture_preset_s *DMX512_scene_get_preset(DMX512_scene_s *scene, uint16_t id);
void DMX512_scene_start(DMX512_scene_s *scene);
void DMX512_scene_stop(DMX512_scene_s *scene);
void DMX512_scene_manage(DMX512_scene_s *scene);

#endif

/**
 * @} Grouping in DMX512_scene submodule ends
 * @} Grouping in DMX512_engine module ends
 */
