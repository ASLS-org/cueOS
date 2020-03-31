#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <stdint.h>
#include <stdlib.h>
#include "http_response.h"


typedef struct http_request http_request_s;
typedef void(*router_fn)(http_request_s *req);

typedef struct http_param{
	char *arg;
	char *val;
}http_param_s;

typedef struct http_request{
	http_method_e method;
	char *uri;
	char *content;
	http_param_s *params;
	uint8_t param_count;
	uint8_t retry_count;
	uint16_t content_length;
	router_fn router;
	http_response_s *res;
}http_request_s;


http_request_s *http_request_new(router_fn router);
void http_request_free(http_request_s *req);
void http_request_parse(http_request_s *req, struct pbuf *p);

#endif
