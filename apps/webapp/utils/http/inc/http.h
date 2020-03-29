#ifndef HTTP_H_
#define HTTP_H_

#include "htmlfs.h"
#include "altcp.h"

#define HTTP_REQUEST_HEADER_SEPARATOR "\r\n"
#define HTTP_REQUEST_HEADER_DELIMITOR "\r\n\r\n"
#define HTTP_REQUEST_PARAMS_SEPARATOR "&"
#define HTTP_REQUEST_PARAMS_DELIMITOR "?"

static const char *const http_req_header_field_str[] = {
  "Content-Length: ",
  "Content-Type: ",
  "Content-Encoding: ",
  "Content-Language: ",
  "Content-Location: ",
};

typedef enum{
	HTTP_REQ_HEADER_CONTENT_LENGTH,
	HTTP_REQ_HEADER_CONTENT_TYPE,
	HTTP_REQ_HEADER_CONTENT_ENCODING,
	HTTP_REQ_HEADER_CONTENT_LANGUAGE,
	HTTP_REQ_HEADER_CONTENT_LOCATION
}http_req_header_field_e;

typedef struct{
	char *str;
	uint8_t len;
}http_method_s;

typedef struct{
	char *arg;
	char *val;
}http_param_s;

static const http_method_s http_methods[] = {
	{"GET ",    3},
	{"POST ",   4},
	{"PUT ",    3},
	{"PATCH ",  5},
	{"DELETE ", 6}
};

typedef uint32_t(*router_fn)(char *uri, char *header_field_values[HTTP_REQ_HEADER_CONTENT_LOCATION],
						 char *data, http_param_s *params,const char **res_buf);

typedef enum{
	HTTP_GET,
	HTTP_POST,
	HTTP_PUT,
	HTTP_PATCH,
	HTTP_DELETE
}http_method_e;

typedef struct{
	struct altcp_pcb *pcb;
	uint16_t port;
	router_fn router;
	struct fs_file *file;
}http_s;

typedef struct{
	const char *res_buf;
	uint32_t res_buf_len;
	http_s *server;
	uint8_t retries;
}http_request_s;

http_s *http_init(uint16_t port, router_fn router);

#endif
