#ifndef _FIXTURE_POOL_H_
#define _FIXTURE_POOL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_fixture.h"

#define DMX512_FIXTURE_POOL_HASHTABLE_SIZE 			64

DMX512_fixture_s *DMX512_fixture_pool_get(uint16_t index);
DMX512_engine_err_e DMX512_fixture_pool_add(uint16_t id, uint16_t chStart, uint16_t chStop);
DMX512_engine_err_e DMX512_fixture_pool_clr(uint16_t index);

#endif
