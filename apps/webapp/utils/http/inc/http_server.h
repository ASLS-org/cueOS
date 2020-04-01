#ifndef HTTP_H_
#define HTTP_H_

#include <stdint.h>
#include <stdlib.h>
#include "altcp.h"
#include "http_defs.h"
#include "http_request.h"

typedef struct http_server{
	struct altcp_pcb *pcb;
	uint16_t port;
	router_fn router;
}http_server_s;

http_server_s *http_server_init(uint16_t port, router_fn router);

#endif
