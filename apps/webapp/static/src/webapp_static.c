#include <string.h>
#include "htmlfs.h"
#include "http.h"

struct fs_file file_handle;

static uint32_t Q_webapp_router(char *uri, char *header_field_values[HTTP_REQ_HEADER_CONTENT_LOCATION],
		 	 	 	 	 	 	char *data, http_param_s *params,const char **res_buf) {

	struct fs_file *file = NULL;
	uint16_t uri_len 	 = strlen(uri);
	uint32_t res_buf_len = 0;
	err_t err;

	if(uri_len > 0 &&  uri[uri_len - 1] == '/'){
		char dafault_uri[uri_len + strlen("index.html")];
		memmove(dafault_uri, uri, uri_len);
		memmove(dafault_uri + uri_len, "index.html", strlen("index.html"));
		dafault_uri[uri_len + strlen("index.html")] = 0;
		err = fs_open(&file_handle, dafault_uri);
	}else{
		err =fs_open(&file_handle, uri);
	}

	if(err == ERR_OK){
		file = &file_handle;
		*res_buf = file->data;
		res_buf_len = (uint32_t)file->len;
	}

	return res_buf_len;

}

void webapp_static_start(void){
	http_init(80, Q_webapp_router);
}
