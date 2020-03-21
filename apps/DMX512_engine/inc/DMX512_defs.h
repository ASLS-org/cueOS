#ifndef DMX512_defs_H
#define DMX512_defs_H

#define DMX512_CHANNEL_ADDRESS_MIN 0
#define DMX512_CHANNEL_ADDRESS_MAX 512
#define DMX512_CHANNEL_VALUE_MIN   0
#define DMX512_CHANNEL_VALUE_MAX   255
#define DMX512_ERROR_MIN -5

typedef enum{
	DMX512_ENGINE_INSTANCE_DUPLICATE = DMX512_ERROR_MIN,
	DMX512_ENGINE_INSTANCE_INVALID,
	DMX512_ENGINE_INSTANCE_UNDEFINED,
	DMX512_ENGINE_CONFIG_DUMP_EXCEPTION,
	DMX512_ENGINE_CONFIG_LOAD_EXCEPTION,
	DMX512_ENGINE_MEMORY_OVERFLOW,
	DMX512_ENGINE_OK,
}DMX512_engine_err_e;

#endif
