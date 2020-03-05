#include <string.h>
#include "DMX512_fixture_preset.h"


DMX512_fixture_preset_s DMX512_fixture_preset_init(DMX512_fixture_s *fixture, uint16_t channel_count, uint16_t *channels, uint8_t *values){

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
