
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
	NET_UP,
	NET_DOWN
}net_state_e;

typedef enum{
	NET_MODE_ETHERNET,
	NET_MODE_WIRELESS
}net_mode_e;

typedef struct{
	struct netif ethernetif;
	struct netif wirelessif;
	ip4_addr_t localAddr;
	ip4_addr_t netmask;
	ip4_addr_t gateway;
	net_state_e state;
	net_mode_e mode;
}net_s;

void net_init(net_mode_e mode);
void net_start(void);
void net_stop(void);
void net_set_mode(net_mode_e mode);

#endif
