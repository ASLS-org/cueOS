/*
 * ASLS_tcp_client.h
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

/* Includes for RTOS ---------------------------------------------------------*/

#if WITH_RTOS
#include "lwip/tcpip.h"
#endif /* WITH_RTOS */

#ifndef ASLS_DRIVERS_INC_ASLS_TCP_CLIENT_H_
#define ASLS_DRIVERS_INC_ASLS_TCP_CLIENT_H_

#define TCP_CLIENT_POLLINTERVAL 10


typedef enum{
	TCP_CLIENT_DISCONNECTED,
	TCP_CLIENT_CONNECTING,
	TCP_CLIENT_CONNECTED,
	TCP_CLIENT_ERROR
}ASLS_ntw_tcpClient_State;

typedef struct{
	struct tcp_pcb *_pcb;
	struct pbuf *_p_tx;
	ip4_addr_t _remoteAddr;
	uint16_t _remotePort;
	ASLS_ntw_tcpClient_State _state;
	void( *_onRecvCallback)();
	void( * _onErrCallback)();
	ASLS_ntw_tcpClient_State( *getState)();
	void( *setRemoteAddr)();
	void( *setRemotePort)();
	void( *setRecvCallback)();
	void( *connect)();
	void( *reset)();
	void( *send)();
}ASLS_ntw_tcpClient;


//CONSTRUCTOR
ASLS_ntw_tcpClient *ASLS_ntw_tcpClient_init(uint8_t remoteAddr[4], uint16_t remotePort);

#endif /* ASLS_DRIVERS_INC_ASLS_TCP_CLIENT_H_ */
