#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <stdint.h>
#include <stdlib.h>
#include "http_defs.h"

#define HTTP_RESPONSE_CONTENT_LENGTH_MAX_LENGTH 5

typedef enum{
	HTTP_RESPONSE_IS_DYNAMIC,
	HTTP_RESPONSE_IS_STATIC
}http_response_static_state;

typedef struct http_response{
	char *data_ptr;				//Pointer to TX data
	uint16_t buf_index;			//Current index of TX buffer
	uint32_t data_ptr_index;	//Current index od TX data
	uint32_t data_len;			//Length of TX data
	uint8_t is_static;			//Do we need to deallocate data ?
}http_response_s;

http_response_s *http_response_new(void);
void http_response_free(http_response_s *res);
uint32_t http_response_get_bytes_left(http_response_s *res);
void http_response_prepare_dynamic(http_response_s *res, http_status_code_e status_code, http_content_types_e content_type, char *content);

#endif
