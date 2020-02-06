#include "DMX512_fixture.h"
#include "DMX512_channel.h"

static DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value);
static void DMX512_fixture_free(DMX512_fixture_s *this);

DMX512_engine_err_e DMX512_fixture_init(DMX512_fixture_s *fixture, uint16_t id, uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	fixture = malloc(sizeof(DMX512_fixture_s));

	fixture->id			= id;
	fixture->chStart 	= chStart;
	fixture->chStop 	= chStop;
	fixture->chCount 	= chStart - chStop;
	fixture->channels 	= malloc(sizeof(DMX512_channel_s) * fixture->chCount);

	fixture->set	 	= DMX512_fixture_setValue;
	fixture->free		= DMX512_fixture_free;

	for(int i=0; i< fixture->chCount; i++){
		err = DMX512_channel_init(fixture->channels[i], chStart + i);
		if(err != DMX512_ENGINE_OK){
			break;
		}
	}

	return err;

}

static DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value){
	return this->channels[channel]->set(this->channels[channel], value);
}

static void DMX512_fixture_free(DMX512_fixture_s *this){
	free(this->channels);
	free(this);
}
