#ifndef DMX512_fixture_pool_H
#define DMX512_fixture_pool_H

#include <stdint.h>

#include "DMX512_defs.h"
#include "DMX512_fixture.h"

typedef struct{
	DMX512_fixture_s *fixtures;
	uint16_t fixture_count;
}DMX512_fixture_pool_s;

DMX512_fixture_pool_s *DMX512_fixture_pool_new(void);
DMX512_engine_err_e DMX512_fixture_pool_add(DMX512_fixture_pool_s *this, DMX512_fixture_s fixture);
DMX512_engine_err_e DMX512_fixture_pool_del(DMX512_fixture_pool_s *this, uint16_t id);
DMX512_engine_err_e DMX512_fixture_pool_get(DMX512_fixture_pool_s *this, uint16_t id, DMX512_fixture_s **fixture);

#endif
