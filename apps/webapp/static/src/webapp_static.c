#include <string.h>
#include "htmlfs.h"
#include "http_server.h"

struct fs_file file_handle;

static void Q_webapp_router(http_request_s *req) {

	struct fs_file *file = NULL;
	uint16_t uri_len 	 = strlen(req->uri);
	err_t err;

	req->res->is_static = 1;

	if(uri_len > 0 &&  req->uri[uri_len - 1] == '/'){
		char dafault_uri[uri_len + strlen("index.html")];
		memmove(dafault_uri, req->uri, uri_len);
		memmove(dafault_uri + uri_len, "index.html", strlen("index.html"));
		dafault_uri[uri_len + strlen("index.html")] = 0;
		err = fs_open(&file_handle, dafault_uri);
	}else{
		err =fs_open(&file_handle, req->uri);
	}

	if(err == ERR_OK){
		file = &file_handle;
		req->res->data_ptr = file->data;
		req->res->data_len = (uint32_t)file->len;
	}

}

void webapp_static_start(void){
	http_server_init(80, Q_webapp_router);
}
