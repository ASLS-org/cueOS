#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <stdint.h>
#include <stdlib.h>
#include "http_response.h"


typedef struct http_request http_request_s;
typedef void(*router_fn)(http_request_s *req);


/**
 * @ingroup http_request
 * @struct http_param_s
 * @brief HTTP uri parameter object
 *
 * HTTP request characterised by a GET method do not convey any content.
 * parameters of the request are passed through the URI instead. these key/value pairs
 * may be parsed and stoed within an HTTP param object.
 *
 */
typedef struct http_param{
	char *arg;									/**< the parameter's key string*/
	char *val;									/**< the parameter's value string*/
}http_param_s;

/**
 * @ingroup http_request
 * @struct http_request_s
 * @brief HTTP request object
 *
 * An HTTP request contains information relative to a received
 * HTTP frame such as its method, header fields and content. Once
 * HTTP requests should be processed through a router function.
 * During routing step it is up to the user to forge response data
 * and forward it to the http request instance to be sent back over
 * to the sender of the request
 */
typedef struct http_request{
	char *raw_data;								/**< pointer to the raw data as forwarded by the TCP/IP stack */
	uint16_t raw_len;							/**< Length of the raw data forwarded by the TCP/IP stack */
	char *uri;									/**< The requested URI buffer*/
	char *content;								/**< The requeste's content buffer*/
	http_method_e method;						/**< HTTP method conveyed in the HTTP request frame*/
	http_version_e http_version;				/**< HTTP version conveyed in the HTTP request frame*/
	uint8_t param_count;						/**< Counter for URI parameters*/
	uint8_t retry_count;						/**< Count for the number of retries for processing request*/
	uint16_t content_length;					/**< Length of the request's content*/
	router_fn router;							/**< Function callback used to process request */
	http_param_s *params;						/**< List of URI paramters */
	http_response_s *res;						/**< POinter to HTTP response which should be processed within the router function */
}http_request_s;


http_request_s *http_request_new(router_fn router);
void http_request_free(http_request_s *req);
uint8_t http_request_parse(http_request_s *req, struct pbuf *p);

#endif
