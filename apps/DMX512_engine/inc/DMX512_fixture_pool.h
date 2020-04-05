#ifndef DMX512_fixture_pool_H
#define DMX512_fixture_pool_H

#include <stdint.h>
#include "DMX512_defs.h"
#include "DMX512_fixture.h"

/**
 * @ingroup DMX512_fixture_pool
 * @struct DMX512_fixture_pool_s
 * @brief Defines a DMX512 fixture pool object
 *
 * DMX512 fixtures may be patched over 512 channels universes. Fixture pools are used
 * to store a universe's patch information. What is referred as a patch here is
 * essentially of fixture objects.
 */
typedef struct{
	DMX512_fixture_s *fixtures;		/**< Pointer to a list of fixtures */
	uint16_t fixture_count;			/**< The amount of fixtures within the fixture list */
}DMX512_fixture_pool_s;

DMX512_fixture_pool_s *DMX512_fixture_pool_new(void);
DMX512_engine_err_e DMX512_fixture_pool_add(DMX512_fixture_pool_s *this, DMX512_fixture_s fixture);
DMX512_engine_err_e DMX512_fixture_pool_del(DMX512_fixture_pool_s *this, uint16_t id);
DMX512_engine_err_e DMX512_fixture_pool_get(DMX512_fixture_pool_s *this, uint16_t id, DMX512_fixture_s **fixture);

#endif
