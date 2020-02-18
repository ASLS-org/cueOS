#include "cmsis_os.h"
#include "DMX512_fixture.h"
#include "DMX512_channel_pool.h"

DMX512_fixture_s DMX512_fixture_init(uint16_t id, uint16_t ch_start, uint16_t ch_stop){

	DMX512_fixture_s fixture;

	fixture.id			= id;
	fixture.ch_start 	= ch_start;
	fixture.ch_stop 	= ch_stop;

	return fixture;

}
//
//DMX512_engine_err_e DMX512_fixture_setValue(DMX512_fixture_s *this, uint16_t channel, uint8_t value){
//	return DMX512_channel_pool_set(channel, value);
//}
