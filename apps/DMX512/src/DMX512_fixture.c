#include "cmsis_os.h"
#include "DMX512_fixture.h"
#include "DMX512_channel.h"

static DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value);
static void DMX512_fixture_free(DMX512_fixture_s *this);

DMX512_fixture_s *DMX512_fixture_init(uint16_t id, uint16_t chStart, uint16_t chStop){

	DMX512_fixture_s *fixture = pvPortMalloc(sizeof(DMX512_fixture_s));

	fixture->id			= id;
	fixture->chStart 	= chStart;
	fixture->chStop 	= chStop;
	fixture->chCount 	= chStop - chStart;
	fixture->channels 	= pvPortMalloc(sizeof(DMX512_channel_s) * (fixture->chCount + 1));

	fixture->set	 	= DMX512_fixture_setValue;
	fixture->free		= DMX512_fixture_free;

	for(int i=0; i< fixture->chCount; i++){
		fixture->channels[i] = DMX512_channel_init(chStart + i);
	}

	return fixture;

}

static DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value){
	return this->channels[channel]->set(this->channels[channel], value);
}

static void DMX512_fixture_free(DMX512_fixture_s *this){
	for(int i=0; i< this->chCount; i++){ vPortFree(this->channels[i]); }
	vPortFree(this->channels);
	vPortFree(this);
}
