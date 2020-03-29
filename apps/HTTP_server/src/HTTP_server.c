///**============================================================================================================================
// * Depedencies inclusion
// * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
// *=============================================================================================================================*/
//
//#include "altcp.h"
//#include "HTTP_server.h"
//#include "httpd_opts.h"
//#include <string.h>
//#include "cmsis_os.h"
//#include "httpd_opts.h"
//#include "lwip_fs.h"
//
//#define HTTP_INDEX_FILENAME "index.html"
//http_server_s *this;
//
////TODO: VERY IMPORTANT !!!! VERIFY THAT NO MEMORY IS LEAKING !!
//
///**============================================================================================================================
// * Private variables definitions
// * These variables are only accessible from within the file's scope
// *=============================================================================================================================*/
//
///**============================================================================================================================
// * Private functions definitions
// * These functions are only accessible from within the file's scope
// *=============================================================================================================================*/
//
///**
// * Ends TCP connection to client
// *
// * @param abort wether the connection needs to be aborted or not
// * @return err_t error code following close operation
// */
//err_t static http_end(http_server_s *this, struct altcp_pcb *pcb, uint8_t abort){
//
//	err_t err = ERR_OK;
//
//	altcp_arg(pcb, NULL);
//	altcp_recv(pcb, NULL);
//	altcp_err(pcb, NULL);
//	altcp_poll(pcb, NULL, 0);
//	altcp_sent(pcb, NULL);
//
//	this->data_buffer = NULL;
//	this->data_left = 0;
//
//	if(this->file != NULL){
//		fs_close(&this->file_handle);
//		this->file = NULL;
//	}
//
//	if(this->parsed_req){
//	    vPortFree(this->parsed_req);
//	    this->parsed_req = NULL;
//	}
//
//	if(this->req){
//	    pbuf_free(this->req);
//	    this->req = NULL;
//	}
//
//	if(abort){
//		altcp_abort(pcb);
//		return ERR_OK;
//	}
//
//	err = altcp_close(pcb);
//	if(err != ERR_OK){
//		altcp_poll(pcb, poll, 4);
//	}
//
//	return err;
//
//}
//
//static void http_request_parse_method(http_request_s *req){
//
//	if(!strncmp(req->raw, "GET", http_methods_len[HTTP_GET])) {
//		req->method = HTTP_GET;
//	}else if(!strncmp(req->raw, "POST", http_methods_len[HTTP_POST])){
//		req->method = HTTP_POST;
//	}else if(!strncmp(req->raw, "PUT", http_methods_len[HTTP_PUT])){
//		req->method = HTTP_PUT;
//	}else if(!strncmp(req->raw, "PATCH", http_methods_len[HTTP_PATCH])){
//		req->method = HTTP_PATCH;
//	}else if(!strncmp(req->raw, "DELETE", http_methods_len[HTTP_DELETE])){
//		req->method = HTTP_DELETE;
//	}else{
//		//DO ERROR
//	}
//
//}
//
////TODO: Clean/improve this
////This is directly extracted from LWIP's library HTTPd app
//static uint8_t http_request_parse_uri_params(http_request_s *req){
//
//	char *pair;
//	char *equals;
//	int loop;
//
//	char *params = strchr(req->uri, '?') + 1;
//
//	if (!params || (params[0] == '\0')) {
//		return (0);
//	}
//
//	pair = params;
//
//	for (loop = 0; (loop < LWIP_HTTPD_MAX_CGI_PARAMETERS) && pair; loop++) {
//
//		req->params[loop].name = pair;
//		equals = pair;
//		pair = strchr(pair, '&');
//
//		if (pair) {
//			*pair = '\0';
//			pair++;
//		} else {
//			pair = strchr(equals, ' ');
//			if (pair) {
//				*pair = '\0';
//			}
//			pair = NULL;
//		}
//
//		equals = strchr(equals, '=');
//		if (equals) {
//			*equals = '\0';
//			req->params[loop].value = equals + 1;
//		} else {
//			req->params[loop].value = NULL;
//		}
//	}
//
//	return loop;
//}
//
//static void http_request_parse_uri(http_request_s *req){
//	char *tmp_req = req->raw + http_methods_len[req->method] + 1;
//	uint16_t len = 0;
//	while(tmp_req[len] != ' '){len++;}
//	tmp_req[len] = 0;
//	req->uri = tmp_req;
//}
//
//static void http_request_parse_message(http_request_s *req){
//
//	uint16_t offset = (strlen(req->uri) + http_methods_len[req->method] + 2);
//	uint16_t len 	= req->len - offset;
//	char *message   = req->raw + offset;
//
//	req->content = lwip_strnstr(message, HTTP_MESSAGE_HEADER_END, len) + strlen(HTTP_MESSAGE_HEADER_END);
//	req->content_length = atoi(lwip_strnstr(message, HTTP_HDR_CONTENT_LEN, len) + strlen(HTTP_HDR_CONTENT_LEN));
//	//TODO: ERROR on content length < 0;
//
//}
//
//static http_request_s *http_request_init(char *data, uint16_t len){
//	http_request_s *req = pvPortMalloc(sizeof(http_request_s));
//	req->raw = data;
//	req->len = len;
//	http_request_parse_method(req);
//	http_request_parse_uri(req);
//	http_request_parse_uri_params(req);
//	http_request_parse_message(req);
//	return req;
//}
//
//static void http_find_file(http_server_s *this){
//
//	struct fs_file *file = NULL;
//
//	uint16_t uri_len = strlen(this->parsed_req->uri);
//
//	if(uri_len > 0 &&  this->parsed_req->uri[uri_len - 1] == '/'){
//		char uri[uri_len + strlen("index.html")];
//		memmove(uri, this->parsed_req->uri, uri_len);
//		memmove(uri + uri_len, "index.html", strlen("index.html"));
//		uri[uri_len + strlen("index.html")] = 0;
//		fs_open(&this->file_handle, uri);
//	}else{
//		fs_open(&this->file_handle, this->parsed_req->uri);
//	}
//
//	file = &this->file_handle;
//
//	if(file != NULL){
//		this->file = &this->file_handle;
//		this->data_buffer = file->data;
//		this->data_left = (uint32_t)file->len;
//	}else{
//		this->file = NULL;
//	}
//
//}
//
///**
// * Parse HTTP request
// * @param *p pointer to packet buffer struct
// * @return err_t error code following parse operation
// */
//static err_t http_parse(http_server_s *this, struct pbuf *p) {
//
//	err_t err = ERR_OK;
//
//	char *data;
//	uint16_t len;
//
//	if (this->file != NULL || this->data_buffer != NULL) {
//		return ERR_USE;
//	}
//
//	if(this->req == NULL ){
//		this->req = p;
//	}else{
//		pbuf_cat(this->req, p);
//	}
//
//	pbuf_ref(p);
//
//	if (this->req->next != NULL) {
//		len = LWIP_MIN(this->req->tot_len, LWIP_HTTPD_MAX_REQ_LENGTH);
//		pbuf_copy_partial(this->req, this->req_buf, len, 0);
//		data = this->req_buf;
//	} else{
//		data = (char *)p->payload;
//		len = p->len;
//	}
//
//	if (len >= HTTP_MIN_REQ_LEN) {
//		if (lwip_strnstr(data, HTTP_CRLF, len) != NULL) {
//			this->parsed_req = http_request_init(data, len);
//			http_find_file(this);
//			//return this->req_parsed_clbck(this->parsed_req);
//			return ERR_OK;
//		}
//	}
//
//	if ((this->req->tot_len <= LWIP_HTTPD_REQ_BUFSIZE) && (pbuf_clen(this->req) <= LWIP_HTTPD_REQ_QUEUELEN)) {
//		return ERR_INPROGRESS;
//	} else{
//		//TODO: return errmessage
//		//err = could not parse request
//	}
//
//	return err;
//
//}
//
//
///** Call tcp_write() in a loop trying smaller and smaller length
// *
// * @param pcb altcp_pcb to send
// * @param ptr Data to send
// * @param length Length of data to send (in/out: on return, contains the
// *        amount of data sent)
// * @param apiflags directly passed to tcp_write
// * @return the return value of tcp_write
// */
////TODO: Clean/improve this
//static err_t http_write(struct altcp_pcb *pcb, const void *data, uint16_t *length) {
//
//  uint16_t len, max_len;
//  err_t err;
//
//  len = *length;
//  if (len == 0) {
//    return ERR_OK;
//  }
//
////  max_len = altcp_sndbuf(pcb);
////  if (max_len < len) {
////    len = max_len;
////  }
//
//  max_len = HTTPD_MAX_WRITE_LEN(pcb);
//  if (len > max_len) {
//    len = max_len;
//  }
//
//  do {
//    err = altcp_write(pcb, data, len, 0);
//    if (err == ERR_MEM) {
//      if ((altcp_sndbuf(pcb) == 0) || (altcp_sndqueuelen(pcb) >= TCP_SND_QUEUELEN)) {
//        len = 1;
//      } else {
//        len /= 2;
//      }
//    }
//  } while ((err == ERR_MEM) && (len > 1));
//
//  if (err == ERR_OK) {
//    *length = len;
//  } else {
//    *length = 0;
//  }
//
//  return err;
//
//}
//
//
//static err_t http_process(http_server_s *this, struct altcp_pcb *pcb){
//
////	if(this->file->len == 0){
////		http_end(this, pcb, 0);
////		return 0;
////	}
//
//	uint8_t data_to_send = 0;
//	uint16_t chunk_len;
//	chunk_len = (u16_t)LWIP_MIN(this->data_left, 0xffff);
//
//	err_t err = http_write(pcb, this->data_buffer, &chunk_len);
//	if (err == ERR_OK) {
//		this->data_buffer += chunk_len;
//		this->data_left -= chunk_len;
//		data_to_send = this->data_left;
//	}
//
////	if(chunk_len == 0){
////		http_end(this, pcb, 0);
////		return 0;
////	}
//
//	return data_to_send;
//
////	if(this->file->len <= 0){
////		http_write(this, this->file->data, &chunk_len);
////		//http_end(this, 0); 	//No more data to be sent
////		return 0;
////	}else{
////		this->file->data += chunk_len;
////		this->file->len -= chunk_len;
////		return 1;
////	}
//
//}
//
//
//
//
//void http_snd(http_server_s *this,char *data, uint32_t length){
//	this->data_buffer = data;
//	this->data_left   = length;
//}
//
//
//
// err_t poll(void *arg, struct altcp_pcb *pcb){
//
//	  http_server_s *this = arg;
//
////	  if (this == NULL) {
////	    http_end(this, 0);
////	    return ERR_OK;
////	  } else {
////
////	    this->retries++;
////
////	    if (this->retries == HTTPD_MAX_RETRIES) {
////		  http_end(this, 0);
////	      return ERR_OK;
////	    }
//
//	    /* If this connection has a file open, try to send some more data. If
//	     * it has not yet received a GET request, don't do this since it will
//	     * cause the connection to close immediately. */
//	   // if (this->file) {
////	      if (http_process(this)) {
////	        altcp_output(pcb);
////	        //this = NULL;
////	      }
////	    }
//
//
////        if(http_process(this)){
////            altcp_output(pcb);
////        }else{
////        	http_end(this, 0);
////        }
//
//	  //}
////		if(this->file == NULL){
////			http_end(this, pcb, 0);
////		}else{
//
//			this->retries++;
//
//			if (this->retries == HTTPD_MAX_RETRIES) {
//			  http_end(this, pcb, 0);
//			  return ERR_OK;
//			}
//
//			if(this->file){
//				if(http_process(this, pcb)){
//					 altcp_output(pcb);
//				}
//			}else{
//				  http_end(this, pcb, 0);
//				  return ERR_OK;
//			}
//		//}
//
//	  return ERR_OK;
//
//}
//
//
///**
// * TCP receive callback
// * @param arg arguments passed to altcp_arg (not used)
// * @param *pcb pointer to to the altcp control block
// * @param *p pointer to packet buffer struct
// * @return err_t error code following receive operation
// */
//static err_t http_recv(void *arg, struct altcp_pcb *pcb,struct pbuf *p, err_t err){
//
//	http_server_s *this = arg;
//
//	if(p == NULL || err != ERR_OK){
//		if(p != NULL){
//			altcp_recved(pcb, p->tot_len);
//			pbuf_free(p);
//		}
//		http_end(this, pcb, 0);
//		return ERR_OK;
//	}else{
//
//		altcp_recved(pcb, p->tot_len);
//
//		if(this->file == NULL){
//
//			err_t parsed = http_parse(this, p);
//
//			if (parsed != ERR_INPROGRESS) {
//				if (this->req != NULL) {
//				  pbuf_free(this->req);
//				  this->req = NULL;
//				}
//			}
//
//			pbuf_free(p); // FREE HERE WONT WORK OTHERWISE !!!!!!!!!!!
//
//			if (parsed == ERR_OK) {
//				http_process(this, pcb);
//				//http_send_file(this, this->parsed_req->uri);
//			} else if (parsed == ERR_ARG) {
//				http_end(this, pcb, 0);
//			}
//
//		}else{
//			//Already sending
//			pbuf_free(p);
//		}
//	}
//
//	return ERR_OK;
//}
//
///**
// * TCP error callback
// * @param arg arguments passed to altcp_arg (not used)
// * @param err TCP stack error code
// */
//static void http_err(void *arg, err_t err){
//
//}
//
///**
// * TCP sent callback
// * @param arg arguments passed to altcp_arg (not used)
// * @param *pcb pointer to to the altcp control block
// * @param len length of the sent packet
// * @return err_t error TCP stack error code
// */
//static err_t http_sent(void *arg, struct altcp_pcb *pcb, uint16_t len){
//	http_server_s *this = arg;
//	//if(this->data_left > 0){
////	if(this->data_buff == NULL){
////		return ERR_OK;
////	}
//
//	if(!http_process(this, pcb)){
//		http_end(this, pcb, 0);
//	}
//
//	this->retries = 0;
////	}else{
////		http_end(this, 0);
////	}
//	//TODO: CLOSE MIGHT BE NEEDED ON NULL DATA
//	return ERR_OK;
//}
//
///**
// * TCP accept callback
// * @param arg arguments passed to altcp_arg (not used)
// * @param *pcb pointer to to the altcp control block
// * @param err TCP stack error code
// * @param err TCP stack error code
// */
//static err_t http_accept(void *arg, struct altcp_pcb *pcb, err_t err){
//
//	if ((err != ERR_OK) || (pcb == NULL)) {
//		err = ERR_VAL;
//	}else{
//
//		  altcp_setprio(pcb, HTTPD_TCP_PRIO);
//
//		  this->pcb = pcb;
//
//		  altcp_arg(pcb,  this);
//		  altcp_recv(pcb, http_recv);
//		  altcp_err(pcb,  http_err);
//		  altcp_sent(pcb, http_sent);
//		  altcp_poll(pcb, poll, 10);
//	}
//
//	return err;
//
//}
//
//
//
///**============================================================================================================================
// * Public functions definitions
// * These functions can be accessed outside of the file's scope
// * @see DMX512_chaser_pool.h for declarations
// *=============================================================================================================================*/
//
///**
// * Initialise API server
// */
//void http_init(uint16_t port){
//
//	this = pvPortMalloc(sizeof(http_server_s));
//	err_t err;
////	this->port = port;
////	this->req_parsed_clbck = req_parsed_clbck;
////	this->sent_clbck = sent_clbck;
////	this->poll_clbck = poll_clbck;
////	this->err_clbck = err_clbck;
////
////	this->pcb = altcp_tcp_new_ip_type(IPADDR_TYPE_ANY);
////
////	altcp_setprio(this->pcb , 1);
////	altcp_bind(this->pcb , IP_ANY_TYPE, this->port );
////
////	this->pcb  = altcp_listen(this->pcb );
////
////	altcp_accept(this->pcb , http_accept);
////	altcp_arg(this->pcb ,  this);
////
////	return this;
//
//	struct altcp_pcb *pcb;
//
//	pcb = altcp_tcp_new_ip_type(IPADDR_TYPE_ANY);
//
//	altcp_setprio(pcb, HTTPD_TCP_PRIO);
//	/* set SOF_REUSEADDR here to explicitly bind httpd to multiple interfaces */
//	err = altcp_bind(pcb, IP_ANY_TYPE, port);
//	pcb = altcp_listen(pcb);
//	altcp_accept(pcb, http_accept);
//
//}
//
