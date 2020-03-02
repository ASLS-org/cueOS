#include "cmsis_os.h"
#include "DMX512_fixture.h"
#include "DMX512_channel_pool.h"

DMX512_fixture_s DMX512_fixture_init(uint16_t id, uint16_t addr, uint16_t ch_count){

	DMX512_fixture_s fixture;

	fixture.id		 = id;
	fixture.addr 	 = addr;
	fixture.ch_count = ch_count;
	fixture.ch_stop  = addr + ch_count - 1;

	return fixture;

}
