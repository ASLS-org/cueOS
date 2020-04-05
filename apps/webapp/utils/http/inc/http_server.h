#ifndef HTTP_H_
#define HTTP_H_

#include <stdint.h>
#include <stdlib.h>
#include "altcp.h"
#include "http_defs.h"
#include "http_request.h"


/**
 * @ingroup http_server
 * @struct http_server_s
 * @brief HTTP server object
 *
 * Multiple instances of HTTP server may created. however they should specify
 * a different running port number.
 */
typedef struct http_server{
	struct altcp_pcb *pcb;					/*< The server's application layered TCP control block*/
	uint16_t port;							/*< The server's running port */
	router_fn router;						/*< The server routing function through which HTTP request should be processed*/
}http_server_s;

http_server_s *http_server_init(uint16_t port, router_fn router);

#endif
