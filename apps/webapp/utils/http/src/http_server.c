/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "http_server.h"
#include "altcp.h"
#include "cmsis_os.h"
#include <string.h>
#include "htmlfs.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Closes TCP connection. finalises write operation and
 * sends FIN ACK to distant connection
 *
 * @param *pcb tcp procol control block
 * @param req http request instance
 */
static void http_close(struct altcp_pcb *pcb, http_request_s *req){

	altcp_arg(pcb, NULL);
	altcp_recv(pcb, NULL);
	altcp_sent(pcb, NULL);
	altcp_poll(pcb, NULL, 0);
	altcp_err(pcb,  NULL);

	if(req != NULL){
		http_request_free(req);
	}

	altcp_close(pcb);

}

/**
 * Handles response data writing over tcp.
 *
 * @param *pcb tcp procol control block
 * @param req http request instance
 * @return err_t error code to be forwarded to the stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
static err_t http_send(struct altcp_pcb *pcb, http_request_s *req){

	err_t err = ERR_OK;

	uint16_t max_len, chunk_len = LWIP_MIN(http_response_get_bytes_left(req->res), 0xffff);

	if(chunk_len <= 0){
		return ERR_OK;
	}

	max_len = altcp_sndbuf(pcb);
	if (max_len < chunk_len) {
		chunk_len = max_len;
	}

	do {
		err = altcp_write(pcb, req->res->data_ptr + req->res->data_ptr_index, chunk_len, 0);
		if (err == ERR_MEM) {
			if ((altcp_sndbuf(pcb) == 0) || (altcp_sndqueuelen(pcb) >= TCP_SND_QUEUELEN)) {
				chunk_len = 1;
			} else {
				chunk_len /= 2;
			}
		}
	} while ((err == ERR_MEM) && (chunk_len > 1));

	if (err == ERR_OK) {
		req->res->data_ptr_index += chunk_len;
	}

	if(!http_response_get_bytes_left(req->res)){
		http_close(pcb, req);
	}

	return err;

}


/**
 * Function called at data reception
 *
 * @param arg http server instance
 * @param *pcb tcp procol control block
 * @param err error forwarded by TCP/IP stack
 * @param *p pointer to the received data
 * @return err_t error code to be forwarded to the stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
static err_t http_receive(void *arg, struct altcp_pcb *pcb,struct pbuf *p, err_t err){

	http_request_s *req = arg;

	if(err != ERR_OK || p == NULL || req == NULL){
		if(p != NULL){ altcp_recved(pcb, p->tot_len); pbuf_free(p); }
		http_close(pcb, req);
	}else{
		altcp_recved(pcb, p->tot_len);
		if(http_request_parse(req, p)){
			pbuf_free(p);
			req->router(req);
			http_send(pcb, req);
		}else{
			pbuf_free(p);
			http_close(pcb, req);
		}
	}

	return ERR_OK;

}

/**
 * Function called following write operation
 *
 * @param arg http request instance
 * @param *pcb tcp procol control block
 * @param len length of the data sent during write
 * @return err_t error code to be forwarded to the stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
static err_t http_sent(void *arg, struct altcp_pcb *pcb, uint16_t len){
	http_request_s *req = arg;
	if(req != NULL){
		http_send(pcb, req);
	}
	return ERR_OK;
}

/**
 * Manages application state at pre-defined intervals. retries sending left response data
 * and handles connection closing once a maximum retry increment value has been reached
 *
 * @param arg http request instance
 * @param *pcb tcp procol control block
 * @return err_t error code to be forwarded to the stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
static err_t http_poll(void *arg, struct altcp_pcb *pcb){

	http_request_s *req = arg;

	  if (req == NULL) {
	    http_close(pcb, NULL);
	    return ERR_OK;
	  } else {
	    req->retry_count++;
	    if (req->retry_count == 10) {
	      http_close(pcb, req);
	      return ERR_OK;
	    }
	    if (req->res->data_ptr != NULL) {
	      if (http_send(pcb, req)) {
	        altcp_output(pcb);
	      }
	    }
	  }

	  return ERR_OK;

}

/**
 * Frees ressources on TCP/IP stack error
 *
 * @param arg http request instance
 * @param err error forwarded by TCP/IP stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
static void http_error(void *arg, err_t err){
	http_request_s *req = arg;
	if (req != NULL) {
		http_request_free(req);
	}
}

/**
 * Function called during new connection attempt to the server
 *
 * @param arg http server instance
 * @param *pcb tcp procol control block
 * @param err error forwarded by TCP/IP stack
 * @return err_t error code to be forwarded to the stack
 * @see err.h for further information regarding the TCIP/IP stack error codes
 */
static err_t http_accept(void *arg, struct altcp_pcb *pcb, err_t err){

	http_server_s *server = arg;

	if(err != ERR_OK || pcb == NULL){
		err = ERR_VAL;
	}else{

		http_request_s *req = http_request_new(server->router);

		if(req == NULL){
			return ERR_MEM;
		}

		altcp_arg(pcb, req);
		altcp_recv(pcb, http_receive);
		altcp_sent(pcb, http_sent);
		altcp_poll(pcb, http_poll, 4);
		altcp_err(pcb, http_error);

	}

	return err;

}

/**
 * Prepares a server's tcp procol control block
 *
 * @param server http server instance
 */
static void http_prepare_pcb(http_server_s *server){
//TODO: maybe set semaphore here
	struct altcp_pcb *pcb;

	pcb = altcp_tcp_new_ip_type(IPADDR_TYPE_ANY);

	altcp_bind(pcb, IP_ANY_TYPE, server->port);

	pcb = altcp_listen(pcb);

	server->pcb = pcb;

	altcp_setprio(pcb, 1);
	altcp_arg(pcb, server);
	altcp_accept(pcb, http_accept);

}

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser_pool.h for declarations
 *=============================================================================================================================*/

/**
 * Initialises a new HTTP server instance
 *
 * @param port the server's port number
 * @param router the router functions which will be used to generate
 *        response data following HTTP request
 */
http_server_s *http_server_init(uint16_t port, router_fn router){

	http_server_s *server = pvPortMalloc(sizeof(http_server_s));

	server->router = router;
	server->port = port;

	http_prepare_pcb(server);

	return server;

}
