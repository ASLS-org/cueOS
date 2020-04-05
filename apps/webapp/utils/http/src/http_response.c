/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "pbuf.h"
#include "cmsis_os.h"
#include "http_response.h"
#include <string.h>


/**
 * @ingroup http_response
 * @fn _http_response_dynamic_cat
 * @brief Concatenates a provided string to the response's data pointer and
 * automatically reallocates data pointer's size to fit the provided
 * string.
 *
 * @param res HTTP response instance to be reallocated
 * @param data string data to be concatenated
 */
static void _http_response_dynamic_cat(http_response_s *res, char *data){
	uint16_t data_len = strlen(data);
	res->data_ptr = pvPortRealloc(res->data_ptr, res->data_len + data_len);
	memmove(res->data_ptr+res->data_len, data, data_len);
	res->data_len+=data_len;
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see http_response.h for declarations
 *=============================================================================================================================*/

/**
 * @ingroup http_response
 * @fn http_response_new
 * @brief Creates a new HTTP response instance
 *
 * @return http_response_s* pointer to the created HTTP response instance
 */
http_response_s *http_response_new(void){

	http_response_s *res = pvPortMalloc(sizeof(http_response_s));

	res->data_ptr 		= NULL;
	res->is_static      = 1;
	res->buf_index 		= 0;
	res->data_ptr_index = 0;
	res->data_len		= 0;

	return res;

}

/**
 * @ingroup http_response
 * @fn http_response_free
 * @brief Safely frees an HTTP response instance
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
 * @ingroup http_response
 * @fn http_response_get_bytes_left
 * @brief Returns the amount of bytes left to be processed
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

/**
 * @ingroup http_response
 * @fn http_response_prepare_dynamic
 * @brief Pre-formats response to HTTP response using provided response header parameters
 *
 * @param *res pointer to the response to be prepared
 * @param status code response's HTTP status code
 * @param content_type the response's content type
 * @param content_length the response's content length in bytes
 * @param content the response's content
 * @warning while it might be tempting, DO NOT USE STRING FUNCTIONS, they are not thread safe
 * 			and therefore not suitable to be used within the execution of the TCP/IP task thread.
 * 			http_response_dynamic_cat function will help dynamically allocating and concatenating
 * 			new string valuehttp_response_prepare_dynamics to the responses' data pointer.
 * @see http_defs.h for further information regarding HTTP header definitions
 *
 */
void http_response_prepare_dynamic(http_response_s *res, http_status_code_e status_code, http_content_types_e content_type, char *content){

	char content_length_str[HTTP_RESPONSE_CONTENT_LENGTH_MAX_LENGTH];

	res->is_static = HTTP_RESPONSE_IS_DYNAMIC;

	itoa(strlen(content), content_length_str, 10);

	_http_response_dynamic_cat(res, http_header_field_str[HTTP_HEADER_FIELD_STATUS_CODE]);
	_http_response_dynamic_cat(res, http_status_codes_str[status_code]);
	_http_response_dynamic_cat(res, HTTP_HEADER_SEPARATOR);
	_http_response_dynamic_cat(res, http_header_field_str[HTTP_HEADER_FIELD_CONTENT_TYPE]);
	_http_response_dynamic_cat(res, http_content_types_str[content_type]);
	_http_response_dynamic_cat(res, HTTP_HEADER_SEPARATOR);
	_http_response_dynamic_cat(res, http_header_field_str[HTTP_HEADER_FIELD_CONTENT_LENGTH]);
	_http_response_dynamic_cat(res, content_length_str);
	_http_response_dynamic_cat(res, HTTP_HEADER_DELIMITOR);
	_http_response_dynamic_cat(res, content);

}
