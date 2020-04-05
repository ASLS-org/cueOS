#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <stdint.h>
#include <stdlib.h>
#include "http_defs.h"

#define HTTP_RESPONSE_CONTENT_LENGTH_MAX_LENGTH 5

/**
 * @ingroup http_response
 * @enum http_response_static_state
 * @brief Whether the HTTP response conveys static data or not.
 * 		  This is very importnt as it will be used to allocate and free
 * 		  the response's data pointer in case the response data needs to
 * 		  be dynamimc
 */
typedef enum{
	HTTP_RESPONSE_IS_DYNAMIC,			/**< Sets the HTTP reponse to be dynamic (enable data pointer memory allocation)*/
	HTTP_RESPONSE_IS_STATIC				/**< Sets the HTTP reponse to be static (disable data pointer memory allocation)*/
}http_response_static_state;


/**
 * @ingroup http_response
 * @struct http_response_s
 * @brief HTTP response object
 *
 * An HTTP response may be static (eg. serving static files) or
 * dynamic (eg. forging dynamic API responses).
 */
typedef struct http_response{
	char *data_ptr;				/**< Pointer to TX data*/
	uint16_t buf_index;			/**< Current index of TX buffer*/
	uint32_t data_ptr_index;	/**< Current index od TX data*/
	uint32_t data_len;			/**< Length of TX data*/
	uint8_t is_static;			/**< Do we need to deallocate data ?*/
}http_response_s;

http_response_s *http_response_new(void);
void http_response_free(http_response_s *res);
uint32_t http_response_get_bytes_left(http_response_s *res);
void http_response_prepare_dynamic(http_response_s *res, http_status_code_e status_code, http_content_types_e content_type, char *content);

#endif
