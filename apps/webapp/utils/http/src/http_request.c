/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "altcp.h"
#include "http_defs.h"
#include "http_request.h"
#include "cmsis_os.h"
#include <string.h>

static uint8_t http_request_parse_method(http_request_s *req){

	uint8_t ret = 0;

	for(uint8_t i=HTTP_GET; i<HTTP_DELETE; i++){
		uint8_t method_len = strlen(http_methods[i]);
		if(!strncmp(req->raw_data, http_methods[i], method_len - 1)){
			req->method = i;
			req->raw_data += method_len + 1;
			req->raw_len -= method_len - 1;
			ret = 1;
			break;
		}
	}

	return ret;

}

static uint8_t http_request_parse_uri(http_request_s *req){

	uint8_t ret = 0;

	req->param_count = 0;

	char *params_start = lwip_strnstr(req->raw_data, STR_COMMON_QMRK, req->raw_len);
	char *params_end   = lwip_strnstr(req->raw_data + 1, STR_COMMON_SP, req->raw_len);
	char *arg_start    = NULL;
	char *val_start    = NULL;
	char *val_end 	   = NULL;

	if(params_end != NULL){

		ret = 1;

		uint8_t uri_len 	= params_start != NULL ? params_start - req->raw_data : params_end - req->raw_data;
		uint16_t params_len = params_end - params_start - 1;

		req->uri = pvPortMalloc(uri_len + 1);
		memmove(req->uri, req->raw_data, uri_len);
		req->uri[uri_len] = 0;

		req->raw_data = params_end + 1;
		req->raw_len -= uri_len + params_len + 1;

		//TODO: put this in separate function for cleanliness ?
		if(params_start != NULL){

			params_start++;

			do{

				arg_start = params_start;
				val_start = strnstr(arg_start, STR_COMMON_EQUL, params_len);
				val_end	  = strnstr(val_start, STR_COMMON_AMPR, params_len);
				if(val_end == NULL){ val_end = params_end; }

				if(arg_start != NULL && val_start != NULL){

					uint8_t arg_len = val_start++ - arg_start + 1;
					uint8_t val_len = val_end - val_start + 1;

					req->params = pvPortRealloc(req->params, sizeof(http_param_s) * (++req->param_count));

					req->params[req->param_count-1].arg = pvPortMalloc(sizeof(char) * arg_len);
					req->params[req->param_count-1].val = pvPortMalloc(sizeof(char) * val_len);

					memmove(req->params[req->param_count-1].arg, arg_start, arg_len);
					memmove(req->params[req->param_count-1].val, val_start, val_len);

					req->params[req->param_count-1].arg[arg_len - 1] = 0;	//Terminating string
					req->params[req->param_count-1].val[val_len - 1] = 0;	//Terminating string

					params_start = val_end + 1;

				}

			}while(arg_start != NULL && val_start != NULL && val_end != NULL);

		}
	}

	return ret;

}

static uint8_t http_request_parse_http_version(http_request_s *req){

	uint8_t ret = 0;

	char *http_version_end   = strnstr(req->raw_data, STR_COMMON_CRLF, req->raw_len);
	uint8_t http_version_len = http_version_end - req->raw_data;

	for(uint8_t i=HTTP_VERSION_0_9; i<HTTP_VERSION_2_0; i++){
		if(!strncmp(req->raw_data, http_versions[i], http_version_len)){
			req->http_version = i;
			req->raw_data += http_version_len + 2;
			req->raw_len -= http_version_len - 2;
			ret = 1;
			break;
		}
	}

	return ret;

}

static uint8_t http_request_parse_headers(http_request_s *req){

	uint8_t ret = 1;

	char *content_length_index = lwip_strnstr(req->raw_data, http_header_field_str[HTTP_HEADER_FIELD_CONTENT_LENGTH], req->raw_len);

	if(content_length_index != NULL){

		char *content_length_value 	   = lwip_strnstr(content_length_index, " ", req->raw_len) + 1;
		char *content_length_value_end = lwip_strnstr(content_length_value, STR_COMMON_CRLF, req->raw_len);

		uint8_t content_length_str_len = content_length_value_end - content_length_value;

		char *content_length_value_str[content_length_str_len];
		memmove(content_length_value_str, content_length_value, content_length_str_len);
		req->content_length = atoi(content_length_value);

	}

	return ret;

}

static uint8_t http_request_parse_content(http_request_s *req){

	uint8_t ret = 1;

	if(req->content_length > 0){

		char *content_data = lwip_strnstr(req->raw_data, HTTP_HEADER_DELIMITOR, req->raw_len) + strlen(HTTP_HEADER_DELIMITOR);

		req->content = pvPortMalloc(req->content_length + 1);
		memcpy(req->content, content_data, req->content_length);
		req->content[req->content_length] = 0;

	}

	return ret;	//TODO: maybe return error if content is too big ?

}

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see http_request.h for declarations
 *=============================================================================================================================*/

/**
 * Creates a new HTTP request instance
 *
 * @param router The router function through which the request should be
 * handled.
 * @return http_request_s the create HTTP request instance
 * @see http_init function for further information
 */
http_request_s *http_request_new(router_fn router){

	http_request_s *req = pvPortMalloc(sizeof(http_request_s));

	req->res 	 	 	= http_response_new();
	req->router			= router;
	req->method  	 	= HTTP_GET;
	req->content 	 	= NULL;
	req->uri	 	 	= NULL;
	req->params	 	 	= NULL;
	req->retry_count 	= 0;
	req->param_count 	= 0;
	req->content_length = 0;

	return req;

}

/**
 * Safely frees an HTTP request instance
 *
 * @param req HTTP request instance to be freed
 */
void http_request_free(http_request_s *req){

	http_response_free(req->res);

	for(uint8_t i=0; i<req->param_count; i++){
		vPortFree(req->params[i].arg);
		vPortFree(req->params[i].val);
	}

	if(req->params != NULL){
		vPortFree(req->params);
	}

	if(req->uri != NULL){
		vPortFree(req->uri);
	}

	if(req->content !=NULL){
		vPortFree(req->content);
	}

	req->params 		= NULL;
	req->router			= NULL;
	req->content 	 	= 0;
	req->uri	 	 	= NULL;
	req->params	 	 	= NULL;
	req->retry_count 	= 0;
	req->param_count 	= 0;
	req->content_length = 0;

	vPortFree(req);

}

/**
 * Extracts and parses HTTP method headers and content from received data and calls
 * the router function defined during initialisation. router function is in charge
 * of generating response data. It is up to the user to implement such function
 *
 * @param req http request instance
 * @param *p pointer to the received data
 * @return err_t error code to be forwarded to the stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
//FIXME: PROBLEM COMES FROM STRINGS TERMINATORS !
uint8_t http_request_parse(http_request_s *req, struct pbuf *p){

	uint8_t ret = 0;
	req->raw_data = p->payload;
	req->raw_len  = p->tot_len;

	pbuf_ref(p);

	if(lwip_strnstr(req->raw_data , HTTP_HEADER_DELIMITOR, req->raw_len) != NULL){
		if(!http_request_parse_method(req)){
		}else if(!http_request_parse_uri(req)){
		}else if(!http_request_parse_http_version(req)){
		}else if(!http_request_parse_headers(req)){
		}else if(!http_request_parse_content(req)){
		}else{
			ret = 1;
		}
	}

	pbuf_free(p);

	return ret;

}
