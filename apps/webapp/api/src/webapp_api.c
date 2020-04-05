/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "http_server.h"
#include "http_defs.h"
#include "http_response.h"
#include "http_request.h"
#include "DMX512_api.h"
#include "webapp_api.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

static void webapp_api_router(http_request_s *req){
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	DMX512_api_router(req);
#endif
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see http_request.h for declarations
 *=============================================================================================================================*/

void webapp_api_start(void){
	http_server_init(WEBAPP_API_PORT, webapp_api_router);
}
