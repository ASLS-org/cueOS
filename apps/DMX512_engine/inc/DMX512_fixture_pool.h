/**
 * @ingroup DMX512_engine
 * @defgroup DMX512_fixture_pool Fixture Pool
 * Fixture pools are used to store a universe's patch information. A patch is a pool of addressed fixtures
 * which channel channel values must me comprised in within a 0-512 address value interval. A patched fixture's
 * channels may not overlap over the channel addresses of another fixture member of the patch.
 * @{
 */

#ifndef DMX512_fixture_pool_H
#define DMX512_fixture_pool_H

#include <stdint.h>
#include "DMX512_defs.h"
#include "DMX512_fixture.h"


/**
 * @brief DMX512 fixture pool structure object
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

/**
 * @} Grouping in DMX512_fixture_pool submodule ends
 */
