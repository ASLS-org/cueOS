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

static net_s *this;
osSemaphoreId ethernetif_LinkSemaphore = NULL;
struct link_str ethernetif_link_arg;

static void _net_setup_if(void *arg);
static void _net_setup_ethernetif(void);
static void _net_setup_wirelessif(void);

void net_init(net_mode_e mode){

	this = pvPortMalloc(sizeof(net_s));

	this->state = NET_DOWN;

	IP4_ADDR(&this->localAddr, 0, 0, 0, 0);
	IP4_ADDR(&this->gateway, 0, 0, 0, 0);
	IP4_ADDR(&this->netmask, 0, 0, 0, 0);

	tcpip_init(_net_setup_if, NULL );

}

static void _net_setup_if(void *arg){
	_net_setup_ethernetif();
	_net_setup_wirelessif();
	net_set_mode(this->mode);
}

static void _net_setup_ethernetif(void){
	netif_add(&this->ethernetif, &this->localAddr, &this->netmask, &this->gateway, NULL, &ethernetif_init, &tcpip_input);
	netif_set_link_callback(&this->ethernetif, ethernetif_update_config);
	netif_is_link_up(&this->ethernetif) ? netif_set_up(&this->ethernetif) : netif_set_down(&this->ethernetif);

	osSemaphoreDef(ethernetif_SEM);
	ethernetif_LinkSemaphore = osSemaphoreCreate(osSemaphore(ethernetif_SEM) , 1 );

	ethernetif_link_arg.netif = &this->ethernetif;
	ethernetif_link_arg.semaphore = ethernetif_LinkSemaphore;

	osThreadDef(ethernetif_LinkThr, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(ethernetif_LinkThr), &ethernetif_link_arg);

	dhcp_start(&this->ethernetif);
}


static void _net_setup_wirelessif(void){

}


void net_set_mode(net_mode_e mode){

	this->mode = mode;

	switch(this->mode){
		case NET_MODE_ETHERNET:
			netif_set_down(&this->wirelessif);
			netif_set_default(&this->ethernetif);
			netif_set_up(&this->ethernetif);
			ethernetif_notify_conn_changed(&this->ethernetif);
			break;
		case NET_MODE_WIRELESS:
			netif_set_down(&this->ethernetif);
			netif_set_default(&this->wirelessif);
			netif_set_up(&this->wirelessif);
			wirelessif_notify_conn_changed(&this->wirelessif);
			break;
	}

}

void ethernetif_notify_conn_changed(struct netif *netif){
	this->state = netif_is_link_up(netif) ? NET_UP : NET_DOWN;
}

void wirelessif_notify_conn_changed(struct netif *netif){
	this->state = netif_is_link_up(netif) ? NET_UP : NET_DOWN;
}
