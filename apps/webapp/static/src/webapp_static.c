/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "htmlfs.h"
#include "http_server.h"

struct fs_file file_handle;


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Fetches file data using provided URI. Fetches a default
 * 404 html error page in case the requested file could not
 * be found within htmlgen.c file.
 *
 * @param req the request to be routed
 * @see htmlgen folder for further information regarding
 * 		html file generation and html file parsing
 */
static void _webapp_static_router(http_request_s *req) {

	struct fs_file *file = NULL;
	uint16_t uri_len 	 = strlen(req->uri);

	err_t err;

	req->res->is_static =  HTTP_RESPONSE_IS_STATIC;

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
		req->res->data_ptr = (char*)file->data;
		req->res->data_len = (uint32_t)file->len;
	}

}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser_pool.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Starts static HTTP server on port 80. This server is ONLY dedicated to serve
 * compiled HTML static files contained within htmlgen.c. Run "htmlgen.sh" script
 * to compile the files put into the "htmlgen" subfolder "dist"  into a single c file.
 * Output of the script is copied into the "htmlgen" subfolder "release" as "htmlgen.c"
 */
void webapp_static_start(void){
	http_server_init(80, _webapp_static_router);
}
