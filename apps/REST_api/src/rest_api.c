/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "http_server.h"
#include "DMX512_rest_api.h"
#include "rest_api.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief routes HTTP requests received on API port
 *
 * @param req the request to be routed
 */
static void _rest_api_router(http_request_s *req){
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	DMX512_rest_api_router(req);
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_MASTER

#endif

}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see http_request.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Starts API HTTP server on port 8000.
 */
void rest_api_start(void){
	http_server_init(REST_API_PORT, _rest_api_router);
}
