#ifndef DMX512_FIXTURE_PRESET_H_
#define DMX512_FIXTURE_PRESET_H_

#include <stdint.h>
#include "DMX512_defs.h"
#include "DMX512_fixture.h"

/**
 * @ingroup DMX512_fixture_preset
 * @def DMX512_FIXTURE_PRESET_DEFAULT
 * @brief Fixture preset's default values
 *
 * Used during initialisation of a fixture preset instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define DMX512_FIXTURE_PRESET_DEFAULT {NULL, 0, NULL, NULL, DMX512_FIXTURE_PRESET_UNINITIALISED}


/**
 * @ingroup DMX512_fixture_preset
 * @enum DMX512_fixture_preset_status
 * @brief Status of a fixture preset
 *
 * Defines whether or not a fixture preset has been correctly initialised.
 */
typedef enum{
	DMX512_FIXTURE_PRESET_UNINITIALISED,		/**< Fixture preset is initialised */
	DMX512_FIXTURE_PRESET_INITIALISED			/**< Fixture preset is unititialised */
}DMX512_fixture_preset_status;


/**
 * @ingroup DMX512_fixture_preset
 * @struct DMX512_fixture_preset_s
 * @brief Defines a DMX512 fixture preset object
 *
 * Fixture presets are used to define a scene's fixture channel values
 * to be set into the DMX512 driver send buffer once the scene is triggered.
 */
typedef struct DMX512_fixture_preset{
	DMX512_fixture_s *fixture;					/**< Pointer to the scene fixture instance */
	uint16_t ch_count;							/**< Count of channels which are will be set by the preset */
	uint16_t *channels;							/**< List of channels affected by the preset */
	uint8_t *values;							/**< List of values directly tied to the preset's channel list*/
	DMX512_fixture_preset_status status;		/**< Current status of the fixture preset **/
}DMX512_fixture_preset_s;


DMX512_fixture_preset_s DMX512_fixture_preset_new(DMX512_fixture_s *fixture, uint16_t channel_count, uint16_t *channels, uint8_t *values);

#endif
