/**
 * @ingroup libs
 * @defgroup HTTP_server
 * Handles the creation of an HTTP server instance. This http server
 * module is able to serve both static (eg. static files) and dynamic
 * (eg. API responses) response data.
 * @{
 */

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
 * @brief HTTP server structure object
 */
typedef struct{
	struct altcp_pcb *pcb;					/*< The server's application layered TCP control block*/
	uint16_t port;							/*< The server's running port*/
	router_fn router;						/*< The server routing function through which HTTP request should be processed*/
}http_server_s;

http_server_s *http_server_init(uint16_t port, router_fn router);


/**
 * @} Grouping in http_server submodule ends
 * @} Grouping in http submodule ends
 * @} Grouping in webapp_utils module ends
 **/

#endif
