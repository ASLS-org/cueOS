
#ifndef DMX512_CHANNEL_POOL_H_
#define MX512_CHANNEL_POOL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DMX512_defs.h"


typedef struct DMX512_channel_pool{
	uint8_t _value_buffer[DMX512_CHANNEL_ADDRESS_MAX];
	DMX512_engine_err_e (*set)(uint16_t address, uint8_t value);
}DMX512_channel_pool_s;


DMX512_channel_pool_s *DMX512_channel_pool_init(void);
DMX512_engine_err_e DMX512_channel_pool_set(uint16_t address, uint8_t value);

#endif
