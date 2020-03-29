//#ifndef HTTP_SERVER_H_
//#define HTTP_SERVER_H_
//
//#include "altcp.h"
//#include "lwip_fs.h"
//#include "httpd_opts.h"	//TODO: create separate config file. maybe cueOS config ?
//
//#define HTTP_MIN_REQ_LEN 7
//#define HTTP_CRLF "\r\n"
//
//#define HTTP_MESSAGE_HEADER_END (HTTP_CRLF HTTP_CRLF)
//#define HTTP_URI_MAX_PARAMETERS 16
//#define HTTP_HDR_CONTENT_LEN    "Content-Length: "
//
//#define DEFAULT_HTTP_REQUEST {0, HTTP_GET, NULL, NULL, NULL, {{NULL, NULL}}, 0}
//static const uint8_t http_methods_len[5] = {3, 4, 3, 5, 6};
//
//typedef enum{
//	HTTP_GET,
//	HTTP_POST,
//	HTTP_PUT,
//	HTTP_PATCH,
//	HTTP_DELETE
//}http_method_e;
//
//typedef struct{
//	char *name;
//	char *value;
//}http_uri_param_s;
//
//typedef struct{
//	uint16_t len;
//	http_method_e method;
//	char *raw;
//	const char *uri;
//	char *content;
//	http_uri_param_s params[HTTP_URI_MAX_PARAMETERS];
//	uint8_t content_length;
//}http_request_s;
//
//typedef struct{
//	uint32_t len;
//	char *data;
//}http_response_s;
//
//typedef struct http_server{
//	struct altcp_pcb *pcb;
//	struct pbuf *req;
//	char req_buf[LWIP_HTTPD_MAX_REQ_LENGTH + 1];
//	const char *data_buffer;
//	uint32_t data_left;
//	uint16_t port;
//	err_t(*req_parsed_clbck)(http_request_s *req);
//	err_t(*poll_clbck)(void *arg, struct altcp_pcb *pcb);
//	err_t(*err_clbck)(void *arg, err_t err);
//	err_t(*sent_clbck)();
//	http_request_s *parsed_req;
//	struct fs_file *file;
//	struct fs_file file_handle;
//	uint8_t retries;
//}http_server_s;
//
//void http_init(uint16_t port);
////err_t http_end(http_server_s *this, uint8_t abort);
////err_t http_write(http_server_s *this,const void *data, u16_t *length);
//void http_snd(http_server_s *this,char *data, uint32_t length);
//void http_send_file(http_server_s *this,const char *uri);
//err_t poll(void *arg, struct altcp_pcb *pcb);
//
//#endif
