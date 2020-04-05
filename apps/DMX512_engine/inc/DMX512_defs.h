#ifndef DMX512_defs_H
#define DMX512_defs_H

#define DMX512_CHANNEL_ADDRESS_MIN 0
#define DMX512_CHANNEL_ADDRESS_MAX 512
#define DMX512_CHANNEL_VALUE_MIN   0
#define DMX512_CHANNEL_VALUE_MAX   255

typedef enum{
	DMX512_ENGINE_OK,
	DMX512_ENGINE_INSTANCE_DUPLICATE 	 = -1,
	DMX512_ENGINE_INSTANCE_INVALID	 	 = -2,
	DMX512_ENGINE_INSTANCE_UNDEFINED	 = -3,
	DMX512_ENGINE_CONFIG_DUMP_EXCEPTION	 = -4,
	DMX512_ENGINE_CONFIG_LOAD_EXCEPTION	 = -5,
	DMX512_ENGINE_MEMORY_OVERFLOW		 = -6,
	DMX512_ENGINE_API_ENDPOINT_UNDEF	 = -7,
	DMX512_ENGINE_API_METHOD_UNDEF	 	 = -8,
}DMX512_engine_err_e;

static char *const DMX512_engine_errs_str[] = {
	"Success",
	"Ressource id already exist",
	"Wrong parameter values",
	"Ressource id not found",
	"Load config error",
	"Save config error",
	"API endoint doesn't exist",
	"Unauthorised method",
};

#endif
