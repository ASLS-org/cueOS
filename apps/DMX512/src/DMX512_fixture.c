#include "DMX512_fixture.h"
#include "DMX512_channel.h"

DMX512_engine_err_e DMX512_fixture_init(DMX512_fixture_s *fixture, uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	fixture = malloc(sizeof(DMX512_fixture_s));

	fixture->chStart 	= chStart;
	fixture->chStop 	= chStop;
	fixture->chCount 	= chStart - chStop;
	fixture->channels = malloc(sizeof(DMX512_channel_s) * fixture->chCount);

	for(int i=0; i< fixture->chCount; i++){
		err = DMX512_channel_init(fixture->channels[i], chStart + i);
		if(err != DMX512_ENGINE_OK){
			break;
		}
	}

	return err;

}
