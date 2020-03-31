/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "altcp.h"
#include "http_defs.h"
#include "http_request.h"
#include "cmsis_os.h"
#include <string.h>


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

	if(req->content !=NULL){
		vPortFree(req->uri);
	}

	if(req->content !=NULL){
		vPortFree(req->content);
	}

	req->params 		= NULL;
	req->router			= NULL;
	req->content 	 	= NULL;
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
void http_request_parse(http_request_s *req, struct pbuf *p){

	char *pbuf_data = p->payload;
	uint16_t pbuf_len = p->len;

	pbuf_ref(p);

	if(lwip_strnstr(pbuf_data , HTTP_HEADER_DELIMITOR, pbuf_len) != NULL){

		http_method_e method = HTTP_GET;
		uint8_t len 		 = 0;

		for(method=HTTP_GET; method<HTTP_DELETE; method++){
			uint8_t method_len = strlen(http_methods[method]);
			if(!strncmp(pbuf_data, http_methods[method], method_len - 1)){
				req->method = method;
				pbuf_data += method_len;
				break;
			}
		}

		do{
			if(pbuf_data[len] == 0){ break; }
		}while(pbuf_data[len++] != ' ');

		uint8_t param_count = 0;
		req->params = pvPortMalloc(sizeof(http_param_s));
		char *value_stop;
		char *params_str = lwip_strnstr(pbuf_data, HTTP_PARAMS_DELIMITOR, len);

		if(params_str !=NULL){

			len = strlen(pbuf_data) - strlen(params_str);
			params_str++;

			do{
				char *value_start = lwip_strnstr(params_str, "=", len) + 1;
				value_stop = lwip_strnstr(params_str, "&", len);
				req->params = pvPortRealloc(req->params, sizeof(http_param_s) * ++param_count);
				uint8_t arg_len = &params_str - &value_start - 1;
				uint8_t val_len = &value_stop - &value_start;
				req->params[param_count-1].arg = pvPortMalloc(arg_len * sizeof(char));
				req->params[param_count-1].val = pvPortMalloc(val_len * sizeof(char));
				memmove(req->params[param_count-1].arg, params_str, arg_len);
				memmove(req->params[param_count-1].val, value_start, val_len);
				params_str = value_stop + 1;
			}while(value_stop != NULL);

		}

		req->uri = pvPortMalloc(len-1);
		memmove(req->uri, pbuf_data, len-1);
		req->uri[len-1] = 0;

		pbuf_data += len;

		char *content_length_index = lwip_strnstr(pbuf_data, http_header_field_str[HTTP_HEADER_FIELD_CONTENT_LENGTH], strlen(pbuf_data));
		char *content_length_end   = lwip_strnstr(content_length_index, HTTP_HEADER_SEPARATOR, strlen(content_length_index));

		uint8_t content_length_str_len = content_length_end - content_length_index;
		char content_length_str[content_length_str_len];
		memmove(content_length_str, content_length_index, content_length_str_len);
		req->content_length = atoi(content_length_str);


		char *content = lwip_strnstr(pbuf_data, HTTP_HEADER_DELIMITOR, strlen(pbuf_data));
		content  += strlen(HTTP_HEADER_DELIMITOR);

		req->content = pvPortMalloc(strlen(content));
		memmove(req->content, content, strlen(content));

		pbuf_free(p);

	}

}
