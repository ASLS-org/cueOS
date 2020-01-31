/*
 * ASLS_network.h
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

#ifndef ASLS_DRIVERS_INC_ASLS_NETW_IFACES_H_
#define ASLS_DRIVERS_INC_ASLS_NETW_IFACES_H_

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "ethernet_driver.h"

/* Includes for RTOS ---------------------------------------------------------*/
#if WITH_RTOS
#include "lwip/tcpip.h"
#endif /* WITH_RTOS */

typedef enum{
	IFACE_UP,
	IFACE_DOWN
}ASLS_ntw_iface_state;

typedef struct{
	struct netif _gnetif;
	ip4_addr_t _localAddr;
	ip4_addr_t _netmask;
	ip4_addr_t _gateway;
	ASLS_ntw_iface_state _state;
	ASLS_ntw_iface_state( *getState)();
	void( *_onConCallback)();
	void( *setNetmask)();
	void( *setGateway)();
	void( *setAddr)();
	void( *setConCallback)();
	void( *up)();
	void( *down)();
}ASLS_ntw_iface;


/* QNode private functions */
ASLS_ntw_iface *ASLS_ntw_iface_init(uint8_t localAddr[4], uint8_t netmask[4], uint8_t gateway[4]);

#endif /* ASLS_DRIVERS_INC_ASLS_NETW_IFACES_H_ */
