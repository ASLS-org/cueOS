#include "http.h"
#include "webapp_api.h"
#include <string.h>

static uint32_t webapp_api_router(char *uri, char *header_field_values[HTTP_REQ_HEADER_CONTENT_LOCATION],
	 	 				 char *data, http_param_s *params,const char **res_buf){

	*res_buf = "caca";
	return strlen(*res_buf);

}

void webapp_api_start(void){
	http_init(Q_WEBAPP_API_PORT, webapp_api_router);
}
