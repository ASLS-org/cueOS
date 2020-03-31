#include "http_server.h"
#include "webapp_api.h"
#include <string.h>
#include "cmsis_os.h"
#include "tcpip.h"
#include "DMX512_engine.h"

static void webapp_api_dynamic_router(http_request_s *req){


//TEST ONLY
	char *test = "test";
	char *ok   = "ok";
	req->res->is_static = 0;
	req->res->data_ptr = pvPortMalloc(20*sizeof(char));
	sprintf(req->res->data_ptr, "%s %s %d", test, ok, 200);
	req->res->data_len = strlen(req->res->data_ptr);

}

void webapp_api_start(void){
	http_server_init(WEBAPP_API_PORT, webapp_api_dynamic_router);
}
