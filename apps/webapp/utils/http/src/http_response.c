/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "pbuf.h"
#include "http_response.h"
#include "cmsis_os.h"


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see http_response.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new HTTP response instance
 *
 * @return http_response_s the create HTTP response instance
 */
http_response_s *http_response_new(void){

	http_response_s *res = pvPortMalloc(sizeof(http_response_s));

	res->is_static      = 1;
	res->data_ptr 		= NULL;
	res->buf_index 		= 0;
	res->data_ptr_index = 0;
	res->data_len		= 0;

	return res;

}

/**
 * Safely frees an HTTP response instance
 *
 * @param res HTTP response instance to be freed
 */
void http_response_free(http_response_s *res){

	if(!res->is_static){
		vPortFree(res->data_ptr);
	}

	res->data_ptr 		= NULL;
	res->buf_index 		= 0;
	res->data_ptr_index = 0;
	res->data_len		= 0;

	vPortFree(res);

}

/**
 * Returns the amount of bytes left to be processed
 * @warning this function does not affect the current
 * 			instance data pointer index. this parameter
 *  	    is incremented during the server send process.
 *
 * @param res HTTP response instance to be freed
 * @return uint32_t bytes ledt count
 */
uint32_t http_response_get_bytes_left(http_response_s *res){
	return res->data_len - res->data_ptr_index;
}
