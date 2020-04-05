
#ifndef DMX512_API_H_
#define DMX512_API_H_

#define DMX512_API_DEFAULT_RES_SIZE 100
#define DMX512_ARG_UNDEF			513

/**
 * @ingroup DMX512_api
 * @enum DMX512_api_endpoint_e
 * @brief DMX512 API endpoint string indexes
 */
typedef enum{
	DMX512_API_ENDPOINT_PATCH,
	DMX512_API_ENDPOINT_GROUPS,
	DMX512_API_ENDPOINT_SCENES,
	DMX512_API_ENDPOINT_CHASERS,
	DMX512_API_ENDPOINT_EFFECTS,
	DMX512_API_ENDPOINT_CUES,
	DMX512_API_ENDPOINT_DEFAULT
}DMX512_api_endpoint_e;

/**
 * @ingroup DMX512_api
 * @enum DMX512_api_args_e
 * @brief DMX512 API parameter strings indexes
 */
typedef enum{
	DMX512_API_PARAM_ID,
	DMX512_API_PARAM_ADDRESS,
	DMX512_API_PARAM_CHANNEL_COUNT,
	DMX512_API_PARAM_FADEIN_TIME,
	DMX512_API_PARAM_FADEOUT_TIME,
	DMX512_API_PARAM_CHANNELS,
	DMX512_API_PARAM_VALUES,
	DMX512_API_PARAM_START,
	DMX512_API_PARAM_STOP
}DMX512_api_args_e;


static const char *const DMX512_api_endpoints_str[] = {
	"/patch",
	"/groups",
	"/scenes",
	"/chasers",
	"/effects",
	"/cues"
};

static char *const DMX512_api_args_str[] = {
	"id",
	"addr",
	"ch_count",
	"fadein",
	"fadeout",
	"channels",
	"values",
	"start",
	"stop"
};


void DMX512_api_router(http_request_s *req);


#endif
