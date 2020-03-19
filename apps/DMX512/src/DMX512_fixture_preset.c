/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "DMX512_fixture_preset.h"


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_fixture.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new scene fixture preset instance
 *
 * @param *fixture pointer to the fixture instance to be referenced
 * @param channel_count the fixture preset channel count
 * @param *channels pointer to an array of channels
 * @param *values pointer to an array of values ordered as defined in channels array
 * @return DMX512_fixture_preset_s the created fixture preset instance
 */
DMX512_fixture_preset_s DMX512_fixture_preset_new(DMX512_fixture_s *fixture, uint16_t channel_count, uint16_t *channels, uint8_t *values){

	DMX512_fixture_preset_s this = DMX512_FIXTURE_PRESET_DEFAULT;

	if(fixture != NULL){

		this.fixture  = fixture;
		this.ch_count = channel_count;
		this.channels = pvPortMalloc(this.ch_count * sizeof(uint16_t));
		this.values   = pvPortMalloc(this.ch_count * sizeof(uint8_t));

		memcpy(this.channels, channels, this.ch_count * sizeof(uint16_t));
		memcpy(this.values, values, this.ch_count * sizeof(uint8_t));

		this.status = DMX512_FIXTURE_PRESET_INITIALISED;

	}

	return this;

}
