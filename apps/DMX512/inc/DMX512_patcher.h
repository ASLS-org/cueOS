#ifndef __DMX512_patcher_H
#define __DMX512_patcher_H

#include <stdint.h>
#include <stdlib.h>
#include "DMX512_fixture.h"

typedef struct{
	DMX512_fixture_s **fixtures;
	uint16_t fixtureCount;
	void(*patch)();
	void(*unpatch)();
	void(*getFixtureInstance)();
}DMX512_patcher_s;

DMX512_patcher_s *DMX512_patcher_init(void);

#endif
