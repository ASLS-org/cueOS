#ifndef Q_WEBAPP_API_H__
#define Q_WEBAPP_API_H__

#define Q_WEBAPP_API_PORT 8000

#define Q_WEBAPP_API_ENDPOINT_AUTH 			"/auth"

#define Q_WEBAPP_API_ENDPOINT_DMX 			"/DMX"
#define Q_WEBAPP_API_ENDPOINT_DMX_PATCH 	"/patch"
#define Q_WEBAPP_API_ENDPOINT_DMX_GROUPS 	"/groups"
#define Q_WEBAPP_API_ENDPOINT_DMX_SCENES 	"/scenes"
#define Q_WEBAPP_API_ENDPOINT_DMX_CHASERS 	"/chasers"
#define Q_WEBAPP_API_ENDPOINT_DMX_EFFECTS 	"/effects"
#define Q_WEBAPP_API_ENDPOINT_DMX_CUES 		"/cues"

void webapp_api_start(void);

#endif
