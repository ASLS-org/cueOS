#include "cmsis_os.h"
#include "DMX512_fixture.h"
#include "DMX512_channel_pool.h"

DMX512_fixture_s DMX512_fixture_init(uint16_t id, uint16_t chStart, uint16_t chStop){

	DMX512_fixture_s fixture;// = pvPortMalloc(sizeof(DMX512_fixture_s));

	fixture.id		= id;
	fixture.chStart = chStart;
	fixture.chStop 	= chStop;
	//fixture->chCount 	= chStop - chStart + 1;
	//fixture->_next 		= NULL;

	return fixture;

}

DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value){
	return DMX512_channel_pool_set(channel, value);
}
