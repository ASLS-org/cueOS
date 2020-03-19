
#ifndef _NET_H_
#define _NET_H_

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

#define NET_DEFAULT {{},{},{},{},{},NET_LINK_DOWN,NET_UNBOUND,NET_MODE_ETHERNET};
#define ETHERNETIF_LINKTHR_ATTR {.stack_size = 128,.priority = (osPriority_t) osPriorityNormal}

typedef enum{
	NET_BOUND 	= 1,
	NET_UNBOUND = 0
}net_bound_state_e;

typedef enum{
	NET_LINK_UP   = 1,
	NET_LINK_DOWN = 0
}net_link_state_e;

typedef enum{
	NET_MODE_ETHERNET,
	NET_MODE_WIRELESS
}net_mode_e;

typedef struct net{
	struct netif ethernetif;
	struct netif wirelessif;
	ip4_addr_t ip_addr;
	ip4_addr_t netmask;
	ip4_addr_t gateway;
	net_link_state_e link_state;
	net_bound_state_e bound_state;
	net_mode_e mode;
	void( *net_ready_callback)(void);
}net_s;

void net_init(net_mode_e mode, void *net_ready_callback);
uint8_t net_is_bound(void);
uint8_t net_is_link_up(void);
ip4_addr_t net_get_ip_addr(void);
ip4_addr_t net_get_gateway(void);
void net_set_mode(net_mode_e mode);

#endif
