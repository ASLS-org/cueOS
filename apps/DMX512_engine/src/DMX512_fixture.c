/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include "cmsis_os.h"
#include "DMX512_fixture.h"


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 *=============================================================================================================================*/

/**
 * @brief Creates a new scene fixture instance
 *
 * @param id the fixture's unique identifier
 * @param addr the fixture's address
 * @param ch_count the fixture's channel count
 * @return DMX512_fixture_s the created fixtur instance
 */
DMX512_fixture_s DMX512_fixture_new(uint16_t id, uint16_t addr, uint16_t ch_count){

	DMX512_fixture_s fixture;

	fixture.id		 = id;
	fixture.addr 	 = addr;
	fixture.ch_count = ch_count;
	fixture.ch_stop  = addr + ch_count - 1;

	return fixture;

}

#endif
