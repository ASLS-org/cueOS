/**
 * @ingroup HTTP
 * @defgroup HTTP_response
 * Handles generation of both static and dynamic HTTP repsponse frames.
 * Currently supported HTTP versions: HTTP/0.9 and HTTP/1.1.
 * @{
 */

#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <stdint.h>
#include <stdlib.h>
#include "http_defs.h"


#define HTTP_RESPONSE_CONTENT_LENGTH_MAX_LENGTH 5


/**
 * @brief Whether the HTTP response conveys static data or not.
 * 		  This is very importnt as it will be used to allocate and free
 * 		  the response's data pointer in case the response data needs to
 * 		  be dynamimc
 */
typedef enum{
	HTTP_RESPONSE_IS_DYNAMIC,			/**< Sets the HTTP response to be dynamic (enable data pointer memory allocation)*/
	HTTP_RESPONSE_IS_STATIC				/**< Sets the HTTP response to be static (disable data pointer memory allocation)*/
}http_response_static_state;


/**
 * @brief HTTP response structure object
 */
typedef struct {
	char *data_ptr;						/**< Pointer to TX data*/
	uint32_t data_ptr_index;			/**< Current index of TX data*/
	uint32_t data_len;					/**< Length of TX data*/
	uint8_t is_static;					/**< Do we need to deallocate data ?*/
}http_response_s;


http_response_s *http_response_new(void);
void http_response_free(http_response_s *res);
uint32_t http_response_get_bytes_left(http_response_s *res);
void http_response_prepare_dynamic(http_response_s *res, http_status_code_e status_code, http_content_types_e content_type, char *content);

#endif

/**
 * @} Grouping in http_response submodule ends
 * @} Grouping in http module ends
 **/
