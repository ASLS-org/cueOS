/**
 * @ingroup cueos_rest_api
 * @defgroup cueos_DMX512_rest_api DMX512 REST API
 * DMX512 engine HTTP wrapper. Handles manipulation of the DMX512 engine resources
 * over HTTP through a user-friendly REST API.
 * @{
 **/

#ifndef DMX512_API_H_
#define DMX512_API_H_

#define DMX512_ARG_UNDEF			513			/**< Undefined argument default size TODO: remove/change this implementation*/


/**
 * @brief DMX512 API endpoint string indexes
 */
typedef enum{
	DMX512_API_ENDPOINT_PATCH,						/**< "patch" API endpoint string index*/
	DMX512_API_ENDPOINT_GROUPS,						/**< "groups" API endpoint string index*/
	DMX512_API_ENDPOINT_SCENES,						/**< "scenes" API endpoint string index*/
	DMX512_API_ENDPOINT_CHASERS,					/**< "chasers" API endpoint string index*/
	DMX512_API_ENDPOINT_EFFECTS,					/**< "effects" API endpoint string index*/
	DMX512_API_ENDPOINT_CUES,						/**< "cues" API endpoint string index*/
	DMX512_API_ENDPOINT_DEFAULT						/**< defaul API endpoint index*/
}DMX512_rest_api_endpoint_e;

/**
 * @brief DMX512 API parameter strings indexes
 */
typedef enum{
	DMX512_API_PARAM_ID,							/**< "id" API request parameter string index*/
	DMX512_API_PARAM_ADDRESS,						/**< "address" API request parameter string index */
	DMX512_API_PARAM_CHANNEL_COUNT,					/**< "channel count" API request parameter string index */
	DMX512_API_PARAM_FADEIN_TIME,					/**< "fadein time" API request parameter string index */
	DMX512_API_PARAM_FADEOUT_TIME,					/**< "fadeout time" API request parameter string index */
	DMX512_API_PARAM_CHANNELS,						/**< "channels" API request parameter string index */
	DMX512_API_PARAM_VALUES,						/**< "values" API request parameter string index */
	DMX512_API_PARAM_START,							/**< "start" API request parameter string index */
	DMX512_API_PARAM_STOP							/**< "stop" API request parameter string index */
}DMX512_rest_api_args_e;


/**
 * @brief DMX512 API endpoint string
 */
static const char *const DMX512_rest_api_endpoints_str[] = {
	"/patch",										/**< "patch" API endpoint string*/
	"/groups",										/**< "groups" API endpoint string*/
	"/scenes",										/**< "scenes" API endpoint string*/
	"/chasers",										/**< "chasers" API endpoint string*/
	"/effects",										/**< "effects" API endpoint string*/
	"/cues"											/**< "cues" API endpoint string*/
};

/**
 * @brief DMX512 API parameter strings
 */
static char *const DMX512_rest_api_args_str[] = {
	"id",											/**< "id" API request parameter string*/
	"addr",											/**< "address" API request parameter string*/
	"ch_count",										/**< "channel count" API request parameter string*/
	"fadein",										/**< "fadein time" API request parameter string*/
	"fadeout",										/**< "fadeout time" API request parameter string*/
	"channels",										/**< "channels" API request parameter string*/
	"values",										/**< "values" API request parameter string*/
	"start",										/**< "start" API request parameter string*/
	"stop"											/**< "stop" API request parameter string*/
};


void DMX512_rest_api_router(http_request_s *req);

#endif

/**
 * @} Grouping in DMX512_rest_api submodule ends
 */
