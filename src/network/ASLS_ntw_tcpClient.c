/*
 * ASLS_ntw_tcp_client.c
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

#include "tcp.h"
#include "lwip/opt.h"
#include "ASLS_ntw_tcpClient.h"

static ASLS_ntw_tcpClient *this;

//GETTERS AND SETTERS
static ASLS_ntw_tcpClient_State ASLS_ntw_tcpClient_getState(void);
static void ASLS_ntw_tcpClient_setRemoteAddr(uint8_t remoteAddr[4]);
static void ASLS_ntw_tcpClient_setRemotePort(uint16_t remotePort);
static void ASLS_ntw_tcpClient_setRecvCallback(void *onRecvCallback);

//PUBLIC METHODS
static void ASLS_ntw_tcpClient_connect(void);
static void ASLS_ntw_tcpClient_reset(void);
static void ASLS_ntw_tcpClient_send(char *packet, size_t len);

//PRIVATE METHODS
static err_t _ASLS_ntw_tcpClient_handlePolling(void *arg, struct tcp_pcb *tpcb);
static void _ASLS_ntw_tcpClient_write(void);
static void _ASLS_ntw_tcpClient_handleError(void *arg, err_t err);
static err_t _ASLS_ntw_tcpClient_onConnectionCallback(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t _ASLS_ntw_tcpClient_onReceiveCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t _ASLS_ntw_tcpClient_onSentCallback(void *arg, struct tcp_pcb *tpcb, u16_t len);



/**
 * Initialises the tcp client.
 *
 * @return ASLS_ntw_tcpClient the current tcpClient instance.
 */
ASLS_ntw_tcpClient *ASLS_ntw_tcpClient_init(uint8_t remoteAddr[4], uint16_t remotePort){

	this = malloc(sizeof(ASLS_ntw_tcpClient));

	IP4_ADDR(&this->_remoteAddr, remoteAddr[0], remoteAddr[1], remoteAddr[2], remoteAddr[3]);

	this->_remotePort		= remotePort;
	this->_state 		  	= TCP_CLIENT_DISCONNECTED;
	this->_onRecvCallback 	= NULL;

	this->connect 			= ASLS_ntw_tcpClient_connect;
	this->reset				= ASLS_ntw_tcpClient_reset;
	this->send 				= ASLS_ntw_tcpClient_send;

	this->getState 			= ASLS_ntw_tcpClient_getState;
	this->setRemoteAddr 	= ASLS_ntw_tcpClient_setRemoteAddr;
	this->setRemotePort 	= ASLS_ntw_tcpClient_setRemotePort;
	this->setRecvCallback 	= ASLS_ntw_tcpClient_setRecvCallback;

	return this;
}



/**
 * returns the instance's "state" parameter
 *
 * @return tcpClient's state parameter value
 */
static ASLS_ntw_tcpClient_State ASLS_ntw_tcpClient_getState(void){
	return this->_state;
}

/**
 * Sets the callback to be called when a packet is received
 *
 * @param onRecvCallback callback function to be set
 */
static void ASLS_ntw_tcpClient_setRemoteAddr(uint8_t remoteAddr[4]){
	if(this->_state == TCP_CLIENT_DISCONNECTED){
		IP4_ADDR(&this->_remoteAddr, remoteAddr[0], remoteAddr[1], remoteAddr[2], remoteAddr[3]);
	}else{
		//TODO: handle error on addr change when TCP client is connected
	}
}

/**
 * Sets the callback to be called when a packet is received
 *
 * @param onRecvCallback callback function to be set
 */
static void ASLS_ntw_tcpClient_setRemotePort(uint16_t remotePort){
	this->_remotePort = remotePort;
}

/**
 * Sets the callback to be called when a packet is received
 *
 * @param onRecvCallback callback function to be set
 */
static void ASLS_ntw_tcpClient_setRecvCallback(void *onRecvCallback){
	this->_onRecvCallback = onRecvCallback;
}

/**
 * Attempts to connect to a specified host over TCP
 *
 * @param remoteAddr remote ip address
 * @param remotePort remote port
 */
static void ASLS_ntw_tcpClient_connect(void){
	this->_state = TCP_CLIENT_CONNECTING;
	this->_pcb 	 = tcp_new();
	tcp_arg(this->_pcb, NULL);
	tcp_recv(this->_pcb, _ASLS_ntw_tcpClient_onReceiveCallback);
	tcp_sent(this->_pcb, _ASLS_ntw_tcpClient_onSentCallback);
	tcp_poll(this->_pcb, _ASLS_ntw_tcpClient_handlePolling, TCP_CLIENT_POLLINTERVAL);
	tcp_err(this->_pcb, _ASLS_ntw_tcpClient_handleError);
	tcp_connect(this->_pcb,&this->_remoteAddr,this->_remotePort,_ASLS_ntw_tcpClient_onConnectionCallback);
}

/**
 * Closes a previously opened TCP connection
 */
static void ASLS_ntw_tcpClient_reset(void){
	this->_state = TCP_CLIENT_DISCONNECTED;
	tcp_arg(this->_pcb, NULL);
	tcp_recv(this->_pcb, NULL);
	tcp_sent(this->_pcb, NULL);
	tcp_poll(this->_pcb, NULL, 0);
	tcp_err(this->_pcb, NULL);
	tcp_abandon(this->_pcb, 1);
}

/**
 * Queues TCP packet and starts send procedure over a pre-established TCP connection
 *
 * @param ASLS_ntw_tcpPacket a TCP packet
 * @see ASLS_ntw_tcpPacket for more information about QPackets
 */
static void ASLS_ntw_tcpClient_send(char *packet, size_t len){
	this->_p_tx = pbuf_alloc(PBUF_TRANSPORT, len , PBUF_POOL);
	pbuf_take(this->_p_tx , packet, len);
	_ASLS_ntw_tcpClient_write();
}

/**
 * Queues TCP packet and starts send procedure over a pre-established TCP connection
 */
static void _ASLS_ntw_tcpClient_write(void){
	struct pbuf *p = this->_p_tx;
	err_t errVal = ERR_OK;
	while( errVal == ERR_OK && this->_p_tx != NULL && this->_p_tx->len <= tcp_sndbuf(this->_pcb)){
		errVal = tcp_write(this->_pcb, p->payload, p->len,1);
		if(errVal == ERR_OK){
			this->_p_tx = p->next;
			if(this->_p_tx != NULL){
				pbuf_ref(this->_p_tx);
			}
			pbuf_free(p);
		}else{
			_ASLS_ntw_tcpClient_handleError(NULL, errVal);
		}
	}
}

/**
 * Callback called when a a valid TCP connection has been established.
 *
 * @param arg TCP stack complementary arguments
 * @param tpcb a TCP control block handle
 * @param err an error code relative to the connection operation
 */
static err_t _ASLS_ntw_tcpClient_onConnectionCallback(void *arg, struct tcp_pcb *tpcb, err_t err){
	if(err != ERR_OK){
		_ASLS_ntw_tcpClient_handleError(NULL, err);
	}else{
		this->_state = TCP_CLIENT_CONNECTED;
	}
	return err;
}

/**
 * Callback called when a  TCP packet is received.
 *
 * @param arg TCP stack complementary arguments
 * @param tpcb a TCP control block handle
 * @param struct pbuf main packet buffer structure
 * @param err an error code relative to the reception operation
 */
static err_t _ASLS_ntw_tcpClient_onReceiveCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err){
	if(p == NULL){
		ASLS_ntw_tcpClient_reset();
	}else{
		tcp_recved(this->_pcb, p->tot_len);
		this->_onRecvCallback(p->payload, p->tot_len);
		pbuf_free(p);
	}
    return err;
}

/**
 * Callback called when a  TCP packet has been sent.
 *
 * @param arg TCP stack complementary arguments
 * @param tpcb a TCP control block handle
 * @param len the length of the packet which has been sent
 */
static err_t _ASLS_ntw_tcpClient_onSentCallback(void *arg, struct tcp_pcb *tpcb, u16_t len){
	LWIP_UNUSED_ARG(len);
	if(this->_p_tx != NULL){
		_ASLS_ntw_tcpClient_write();
	}
	return ERR_OK;
}

/**
  * @brief  This function implements the tcp_poll callback function
  * @retval err_t: error code
  */
static err_t _ASLS_ntw_tcpClient_handlePolling(void *arg, struct tcp_pcb *tpcb){
	if(this->_p_tx != NULL){
		_ASLS_ntw_tcpClient_write();
	}
	return ERR_OK;
}

/**
 * Callback called when an error has occured during TCP connection
 *
 * @param arg TCP stack complementary arguments
 * @err An lwip error code
 * @see err_enum_t in within "err.h" for more information regarding err codes
 */
static void _ASLS_ntw_tcpClient_handleError(void *arg, err_t err){
	switch(err){
		case ERR_ABRT:
			ASLS_ntw_tcpClient_reset();
			break;
		case ERR_RST:
			ASLS_ntw_tcpClient_reset();
			break;
	}
}
