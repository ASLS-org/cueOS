
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


/**
 * @ingroup network
 * @enum net_bound_state_e
 * @brief conveys information regarding the network's DHCP state
 */
typedef enum{
	NET_BOUND 	= 1,					/*< DHCP state is bound (IP found)*/
	NET_UNBOUND = 0,					/*< DHCP state is bound (No IP found)*/
}net_bound_state_e;

/**
 * @ingroup network
 * @enum net_bound_state_e
 * @brief conveys information relative the network's link state
 */
typedef enum{
	NET_LINK_UP   = 1,					/*< The network is up*/
	NET_LINK_DOWN = 0					/*< The network is down*/
}net_link_state_e;

/**
 * @ingroup network
 * @enum net_bound_state_e
 * @brief used to set net's prefferred/current network interface
 */
typedef enum{
	NET_MODE_ETHERNET,					/*< Use ethernet */
	NET_MODE_WIRELESS					/*< Use wifi */
}net_mode_e;


/**
 * @ingroup network
 * @struct net_s
 * @brief Defines the system's network interfaces
 *
 * The system is able to switch between wirelless and wired ethernet configuration.
 * Network configuration address may be set static or provided through DHCP.
 * @see cueosCONFIG.h for further information relative to network configuration
 */
typedef struct net{
	struct netif ethernetif;					/*< Wired network interface*/
	struct netif wirelessif;					/*< Wireless network interface*/
	ip4_addr_t ip_addr;							/*< Active network interface IP address*/
	ip4_addr_t netmask;							/*< Active network interface netmask*/
	ip4_addr_t gateway;							/*< Active network interface gateway*/
	net_link_state_e link_state;				/*< Active network interface link state @see net_link_state_e*/
	net_bound_state_e bound_state;				/*< Active network interface DHCP state @see net_bound_state_e*/
	net_mode_e mode;							/*< Current network mode @snet_mode_e*/
	void( *net_ready_callback)(void);
}net_s;

void net_init(net_mode_e mode, void *net_ready_callback);
uint8_t net_is_bound(void);
uint8_t net_is_link_up(void);
ip4_addr_t net_get_ip_addr(void);
ip4_addr_t net_get_gateway(void);
void net_set_mode(net_mode_e mode);

#endif
