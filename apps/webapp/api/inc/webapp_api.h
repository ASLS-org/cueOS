#ifndef WEBAPP_API_H__
#define WEBAPP_API_H__

#define WEBAPP_API_PORT 8000
#define WEBAPP_API_ENDPOINT_SEPARATOR 	"/"
#define WEBAPP_API_

static char *const wabapp_api_endpoints[] = {
	"/auth",
	"/patch",
	"/groups",
	"/scenes",
	"/chasers",
	"/effects",
	"/cues"
};

static const char *const wabapp_api_responses[] = {
	"{err: 404, msg: Invalid api path}",
};

void webapp_api_start(void);

#endif
