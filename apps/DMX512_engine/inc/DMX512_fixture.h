/**
 * @ingroup DMX512_fixture_pool
 * @defgroup DMX512_fixture
 * Fixtures are machines daisy chained together to form a DMX512 universe.
 * Each fixture is addressed over a range of up to 512 addresses. A fixture's
 * channel count defines its address footprint in a DMX512 universe and fixture addresses
 * and hannels may not overlap one into another.
 * @{
 **/

#ifndef DMX512_fixture_H
#define DMX512_fixture_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_defs.h"

/**
 * @brief DMX512 fixture structure object
 */
typedef struct DMX512_fixture{
	uint16_t id;				/**< the fixture's unique identifier */
	uint16_t addr;				/**< the fixture's univers address (should be described in the 0-512 interval)  */
	uint16_t ch_count;			/**< the fixture's channel count (should be described in the 0-512 interval) */
	uint16_t ch_stop;			/**< the fixture's last channel address vlue (should be described in the 0-512 interval) */
}DMX512_fixture_s;

DMX512_fixture_s DMX512_fixture_new(uint16_t id, uint16_t addr, uint16_t ch_count);

#endif

/**
 * @} Grouping in DMX512_fixture submodule ends
 * @} Grouping in DMX512_engine module ends
 */
