/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "http_server.h"
#include "web_application_content.c"

struct fs_file file_handle;


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief opens a pre-compiled file located in htmlgen.c
 *
 * @param *file pointer to an fs_file object
 * @param *name pointer to a string containing the path/name
 * 		  of the file to be opened
 */
static err_t _web_application_open(struct fs_file *file, const char *name) {

  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
    return ERR_ARG;
  }

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (const char *)f->name)) {
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->flags = f->flags;
      return ERR_OK;
    }
  }

  /* file not found */
  return ERR_VAL;
}


/**
 * @brief Fetches file data using provided URI. Fetches a default
 * 404 html error page in case the requested file could not
 * be found within htmlgen.c file.
 *
 * @param req the request to be routed
 * @see htmlgen folder for further information regarding
 * 		html file generation and html file parsing
 */
static void _web_application_router(http_request_s *req) {

	struct fs_file *file = NULL;
	uint16_t uri_len 	 = strlen(req->uri);

	err_t err;

	req->res->is_static =  HTTP_RESPONSE_IS_STATIC;

	if(uri_len > 0 &&  req->uri[uri_len - 1] == '/'){
		char dafault_uri[uri_len + strlen("index.html")];
		memmove(dafault_uri, req->uri, uri_len);
		memmove(dafault_uri + uri_len, "index.html", strlen("index.html"));
		dafault_uri[uri_len + strlen("index.html")] = 0;
		err = _web_application_open(&file_handle, dafault_uri);
	}else{
		err = _web_application_open(&file_handle, req->uri);
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
void web_application_start(void){
	http_server_init(80, _web_application_router);
}
