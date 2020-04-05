#ifndef WEBAPP_API_H__
#define WEBAPP_API_H__

#define WEBAPP_API_PORT 8000
#define WEBAPP_API_ENDPOINT_SEPARATOR 	"/"

typedef enum{
	WEBAPP_API_ENDPOINT_PATCH,
	WEBAPP_API_ENDPOINT_GROUPS,
	WEBAPP_API_ENDPOINT_SCENES,
	WEBAPP_API_ENDPOINT_CHASERS,
	WEBAPP_API_ENDPOINT_EFFECTS,
	WEBAPP_API_ENDPOINT_CUES,
	WEBAPP_API_ENDPOINT_DEFAULT
}webapp_api_endpoint_e;


static char *const webapp_api_endpoints[] = {
	"/patch",
	"/groups",
	"/scenes",
	"/chasers",
	"/effects",
	"/cues"
};

static const char *const webapp_api_responses[] = {
	"{err: 404, msg: Invalid api path}",
};

static char *const endpoint_err_content = "{\"err\": \"Invalid API endpoint\"}";
static char *const auth_err_content	   = "{\"err\": \"Invalid creddentials\"}";
static char *const req_ok_content	   = "{\"msg\": \"success\"}";
static char *const method_err_content  = "{\"err\": HTTP Method not supported\"\"}";
static char *const not_found_err_content  = "{\"err\": Instance not found\"\"}";


void webapp_api_start(void);

#endif
