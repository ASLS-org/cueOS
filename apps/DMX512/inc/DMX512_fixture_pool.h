#ifndef __DMX512_fixture_pool_H
#define __DMX512_fixture_pool_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "DMX512_fixture.h"
#include "DMX512_defs.h"

DMX512_engine_err_e DMX512_fixture_pool_add(uint16_t id, uint16_t chStart, uint16_t chStop);
DMX512_engine_err_e DMX512_fixture_pool_del(uint16_t id);
DMX512_fixture_s *DMX512_fixture_pool_get(uint16_t id);
DMX512_fixture_s *DMX512_fixture_pool_get_all(void);

#endif
