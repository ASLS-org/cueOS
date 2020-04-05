#ifndef DMX512_CHASER_H
#define DMX512_CHASER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_chaser_step.h"
#include "DMX512_defs.h"


/**
 * @ingroup DMX512_chaser
 * @def DMX512_CHASER_DEFAULT
 * @brief chaser instance default values
 *
 * Used during initialisation of a chaser instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define DMX512_CHASER_DEFAULT {0, NULL, DMX512_CHASER_MODE_LOOP, DMX512_CHASER_DIRECTION_FORWARD, 0, 0, DMX512_CHASER_IDLE, DMX512_CHASER_UNINITIALISED, DMX512_CHASER_PINGPONG_PING}


/**
 * @ingroup DMX512_chaser
 * @enum DMX512_chaser_status_e
 * @brief Status of a chaser instance
 *
 * Defines whether or not a chaser instance has been correctly initialised.
 */
typedef enum{
	DMX512_CHASER_UNINITIALISED,
	DMX512_CHASER_INITIALISED
}DMX512_chaser_status_e;

/**
 * @ingroup DMX512_chaser
 * @enum DMX512_chaser_mode_e
 * @brief Playing mode of a chsaer
 *
 * Defines whether or not a chaser should be played in a loop or not
 */
typedef enum{
	DMX512_CHASER_MODE_LOOP,					/**< */
	DMX512_CHASER_MODE_SINGLE_SHOT,				/**< */
}DMX512_chaser_mode_e;

/**
 * @ingroup DMX512_chaser
 * @enum DMX512_chaser_direction_e
 * @brief Playing direction of a chaser
 *
 * Defines direction of the playing sequence of a chaser.
 */
typedef enum{
	DMX512_CHASER_DIRECTION_FORWARD,			/**< Play consecutive steps in ascending order*/
	DMX512_CHASER_DIRECTION_BACKWARD,			/**< Play consecutive steps in descending order*/
	DMX512_CHASER_DIRECTION_PINGPONG,			/**< Play consecutive steps back and forth*/
	DMX512_CHASER_DIRECTION_RANDOM				/**< Randomly pick steps to be played*/
}DMX512_chaser_direction_e;

/**
 * @ingroup DMX512_chaser
 * @enum DMX512_chaser_pingpong_state_e
 * @brief Current pingpong state
 *
 * Provides information regarding the current pingpong state of
 * a chaser when DMX512_CHASER_DIRECTION_PINGPONG is enables
 */
typedef enum{
	DMX512_CHASER_PINGPONG_PING,				/**< chaser is playing steps in ascending order*/
	DMX512_CHASER_PINGPONG_PONG,				/**< chaser is playing steps in descending order*/
}DMX512_chaser_pingpong_state_e;

/**
 * @ingroup DMX512_chaser
 * @enum DMX512_chaser_step_state_e
 * @brief Current playing state of a chaser
 *
 * Used by the chaser state machine to determine the current operation to
 * be done on the chaser based on its current state value.
 */
typedef enum{
	DMX512_CHASER_PLAYING,						/**< The chaser is playing*/
	DMX512_CHASER_IDLE							/**< The chaser is idle*/
}DMX512_chaser_state_e;


/**
 * @ingroup DMX512_chaser
 * @struct DMX512_chaser_s
 * @brief Defines a DMX512 chaser object
 *
 * In a DMX lightshow, scenes may be chained and played back in a sequence called chaser.
 * Scenes contained within a chaser are called "steps" and each individual steps conveys
 * specific information regarding play-timings of the asociated scene.
 * @see DMX512_chaser_step_s
 */
typedef struct{
	uint16_t id;								/**< Chaser's unique identifier */
	DMX512_chaser_step_s *steps;				/**< List of chaser steps */
	DMX512_chaser_mode_e mode;					/**< Chaser's playing mode @see DMX512_chaser_mode_e */
	DMX512_chaser_direction_e direction;		/**< Chaser's playing direction @see DMX512_chaser_direction_e */
	uint16_t step_count;						/**< Aomunt of steps in the list */
	uint16_t current_step;						/**< Index of the current scene */
	DMX512_chaser_state_e state;				/**< Chaser's playing status @see DMX512_chaser_status_e*/
	DMX512_chaser_status_e status;				/**< Initialisation state of the chaser @see DMX512_chaser_step_state_e */
	DMX512_chaser_pingpong_state_e pingpong;	/**< Current pingpong state (this will be changed) @see DMX512_chaser_pingpong_state_e */
}DMX512_chaser_s;

DMX512_chaser_s DMX512_chaser_new(uint16_t id, DMX512_chaser_mode_e mode, DMX512_chaser_direction_e direction);
DMX512_engine_err_e DMX512_chaser_add_step(DMX512_chaser_s *this, DMX512_chaser_step_s step);
DMX512_engine_err_e DMX512_chaser_del_step(DMX512_chaser_s *this, uint16_t id);
DMX512_chaser_step_s *DMX512_chaser_get_step(DMX512_chaser_s *this, uint16_t id);
void DMX512_chaser_manage(DMX512_chaser_s *this);
void DMX512_chaser_start(DMX512_chaser_s *this);

#endif
