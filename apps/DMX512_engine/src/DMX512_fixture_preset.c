/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include <string.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "DMX512_fixture_preset.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Checks whether or not provided fixture preset arguments are correct
 *
 * @param *fixture pointer to the fixture instance to be referenced
 * @param channel_count the fixture preset channel count
 * @param *channels pointer to an array of channels
 * @param *values pointer to an array of values ordered as defined in channels array
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, negative error code otherwise
 * @see DMX512_defs.h for further information relative to error codes
 */
static uint8_t _DMX512_fixture_preset_check(DMX512_fixture_s *fixture, uint16_t channel_count, uint16_t *channels, uint8_t *values){
	if(fixture == NULL || channel_count > fixture->ch_count){
		return 0;
	}else{
		for(uint16_t i = 0; i< channel_count; i++){
			if(channels[i] > fixture->ch_count){ return 0; }
		}
	}
	return 1;
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_fixture.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Creates a new scene fixture preset instance
 *
 * @param *fixture pointer to the fixture instance to be referenced
 * @param channel_count the fixture preset channel count
 * @param *channels pointer to an array of channels
 * @param *values pointer to an array of values ordered as defined in channels array
 * @return DMX512_fixture_preset_s the created fixture preset instance
 */
DMX512_fixture_preset_s DMX512_fixture_preset_new(DMX512_fixture_s *fixture, uint16_t channel_count, uint16_t *channels, uint8_t *values){

	DMX512_fixture_preset_s this = DMX512_FIXTURE_PRESET_DEFAULT;

	if(_DMX512_fixture_preset_check(fixture, channel_count, channels, values)){

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

/**
 * @brief Frees a fixture preset's dynamically allocated ressources
 * 		  and resets its values parameters to default
 *
 * @param *DMX512_fixture_preset_s pointer to the fixture preset instance to be reseted
 */
void DMX512_fixture_preset_free(DMX512_fixture_preset_s *fixture_preset){
	if(fixture_preset != NULL){
		vPortFree(fixture_preset->channels);
		vPortFree(fixture_preset->values);
		fixture_preset->ch_count = 0;
		fixture_preset->fixture	 = NULL;
	}
}

#endif
