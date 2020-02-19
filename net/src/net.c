/*
 * ASLS_network_config.c
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

#include "cueOS_config.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "ethernet_driver.h"
#include "net.h"

static net_s this = NET_DEFAULT;
osSemaphoreId ethernetif_LinkSemaphore = NULL;
struct link_str ethernetif_link_arg;

static void _net_setup_if(void *arg);
static void _net_set_ip(void);
static void _net_setup_ethernetif(void);
static void _net_setup_wirelessif(void);

void net_init(net_mode_e mode){
	IP4_ADDR(&this.ip_addr, 0, 0, 0, 0);
	IP4_ADDR(&this.gateway, 0, 0, 0, 0);
	IP4_ADDR(&this.netmask, 0, 0, 0, 0);
	tcpip_init(_net_setup_if, NULL );

}

static void _net_setup_if(void *arg){
	_net_setup_ethernetif();
	_net_setup_wirelessif();
	net_set_mode(this.mode);
}

static void _net_setup_ethernetif(void){
	netif_add(&this.ethernetif, &this.ip_addr, &this.netmask, &this.gateway, NULL, &ethernetif_init, &tcpip_input);
	netif_set_link_callback(&this.ethernetif, ethernetif_update_config);
	netif_is_link_up(&this.ethernetif) ? netif_set_up(&this.ethernetif) : netif_set_down(&this.ethernetif);

	osSemaphoreDef(ethernetif_SEM);
	ethernetif_LinkSemaphore = osSemaphoreCreate(osSemaphore(ethernetif_SEM) , 1 );

	ethernetif_link_arg.netif = &this.ethernetif;
	ethernetif_link_arg.semaphore = ethernetif_LinkSemaphore;

	osThreadDef(ethernetif_LinkThr, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate(osThread(ethernetif_LinkThr), &ethernetif_link_arg);

	netif_set_up(&this.ethernetif);

	dhcp_start(&this.ethernetif);

	osThreadDef(ethernetif_DHCPThr, _net_set_ip, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate(osThread(ethernetif_DHCPThr), &_net_set_ip);

}

static void _net_set_ip(void){
	for(;;){
		if(dhcp_supplied_address(&this.ethernetif)){
			this.ip_addr = this.ethernetif.ip_addr;
			this.gateway = this.ethernetif.gw;
			this.netmask = this.ethernetif.netmask;
			this.bound_state = NET_BOUND;
		}else{
			this.bound_state = NET_UNBOUND;
		}
	}
	osDelay(200);
}

static void _net_setup_wirelessif(void){

}

ip4_addr_t net_get_ip_addr(void){

	ip4_addr_t ip_addr = {};

	switch(this.mode){
		case NET_MODE_ETHERNET:
			return ip_addr = this.ethernetif.ip_addr;
			break;
		case NET_MODE_WIRELESS:
			return ip_addr = this.wirelessif.ip_addr;
			break;
	}

	return ip_addr;

}

ip4_addr_t net_get_gateway(void){

	ip4_addr_t ip_addr = {};

	switch(this.mode){
		case NET_MODE_ETHERNET:
			return ip_addr = this.ethernetif.gw;
			break;
		case NET_MODE_WIRELESS:
			return ip_addr = this.wirelessif.gw;
			break;
	}

	return ip_addr;

}

uint8_t net_is_bound(void){
	return this.bound_state;
}

uint8_t net_is_link_up(void){
	return this.link_state;
}

void net_set_mode(net_mode_e mode){

	this.mode = mode;

	switch(this.mode){
		case NET_MODE_ETHERNET:
			netif_set_down(&this.wirelessif);
			netif_set_default(&this.ethernetif);
			netif_set_up(&this.ethernetif);
			ethernetif_notify_conn_changed(&this.ethernetif);
			break;
		case NET_MODE_WIRELESS:
			netif_set_down(&this.ethernetif);
			netif_set_default(&this.wirelessif);
			netif_set_up(&this.wirelessif);
			wirelessif_notify_conn_changed(&this.wirelessif);
			break;
	}

}

void ethernetif_notify_conn_changed(struct netif *netif){
	this.link_state = netif_is_link_up(netif) ? NET_LINK_UP : NET_LINK_DOWN;
	dhcp_network_changed(netif);
}

void wirelessif_notify_conn_changed(struct netif *netif){
	this.link_state = netif_is_link_up(netif) ? NET_LINK_UP : NET_LINK_DOWN;
}
