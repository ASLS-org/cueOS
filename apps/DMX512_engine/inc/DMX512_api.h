
#ifndef DMX512_API_H_
#define DMX512_API_H_

#define DMX512_API_DEFAULT_RES_SIZE 100
#define DMX512_ARG_UNDEF			513


typedef enum{
	DMX512_API_ENDPOINT_PATCH,
	DMX512_API_ENDPOINT_GROUPS,
	DMX512_API_ENDPOINT_SCENES,
	DMX512_API_ENDPOINT_CHASERS,
	DMX512_API_ENDPOINT_EFFECTS,
	DMX512_API_ENDPOINT_CUES,
	DMX512_API_ENDPOINT_DEFAULT
}DMX512_api_endpoint_e;

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

typedef enum{
	DMX512_API_DATA_FIXTURE,
	DMX512_API_DATA_SCENE,
	DMX512_API_DATA_PRESET,
	DMX512_API_DATA_CHASER,
	DMX512_API_DATA_STEP,
	DMX512_API_DATA_ERR,
}DMX512_api_data_e;


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

static const char *const DMX512_api_data_str[] = {
	"{\"fixture\":{\"id\":%d,\"addr\":%d,\"ch_count\":%d}, \"errcode\":%d}",
	"{\"scene\":{\"id\":%d,\"fadein\":%d,\"fadeout\":%d, \"presets\":[%s]}, \"errcode\":%d}",
	"{\"preset\":{\"fixture_id\":%d, \"channels\":[%s], \"values\":[%s]}, \"errcode\":%d}",
	"{\"chaser\":{\"id\":%d, \"steps\":[%s]}, \"errcode\":%d}",
	"{\"step\":{\"scene_id\":%d, \"fadein\":%d, \"fadeout\":%d, \"hold\":%d}, \"errcode\":%d}",
	"{\"msg\":\"%s\", \"errcode\":%d}",
};


void DMX512_api_router(http_request_s *req);

#endif
