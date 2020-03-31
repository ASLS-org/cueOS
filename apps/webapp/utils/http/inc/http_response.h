#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct http_response{
	struct pbuf *buf;			//TX buffer
	char *data_ptr;				//Pointer to TX data
	uint16_t buf_index;			//Current index of TX buffer
	uint32_t data_ptr_index;	//Current index od TX data
	uint32_t data_len;			//Length of TX data
	uint8_t is_static;			//Do we need to deallocate data ?
}http_response_s;

http_response_s *http_response_new(void);
void http_response_free(http_response_s *res);
uint32_t http_response_get_bytes_left(http_response_s *res);

#endif
