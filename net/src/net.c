/*
 * ASLS_network_config.c
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

#include "cueOS_config.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernet_driver.h"
#include "net.h"

static net_s *this;
osSemaphoreId Netif_LinkSemaphore = NULL;
struct link_str link_arg;


static void _net_setupIfaces(void);

void net_init(uint8_t localAddr[4], uint8_t netmask[4], uint8_t gateway[4], net_mode_e mode){

	this = malloc(sizeof(net_s));

	this->state = NET_DOWN;

	IP4_ADDR(&this->localAddr, localAddr[0], localAddr[1], localAddr[2], localAddr[3]);
	IP4_ADDR(&this->gateway, gateway[0], gateway[1], gateway[2], gateway[3]);
	IP4_ADDR(&this->netmask, netmask[0], netmask[1], netmask[2], netmask[3]);

	net_setMode(mode);

	tcpip_init(_net_setupIfaces, NULL );

}



static void _net_setupIfaces(void){

	#ifdef cueOS_NET_ETHERNET
		netif_add(&this->ethernetif, &this->localAddr, &this->netmask, &this->gateway, NULL, &ethernetif_init, &tcpip_input);
	#endif
	#ifdef cueOS_NET_WIRELESS
		netif_add(&this->wirelessif, &this->localAddr, &this->netmask, &this->gateway, NULL, &wirelessif_init, &tcpip_input);
	#endif

	netif_set_default(&this->gnetif);

	netif_is_link_up(&this->gnetif) ? netif_set_up(&this->gnetif) : netif_set_down(&this->gnetif);
	netif_set_link_callback(&this->gnetif, ethernetif_update_config);

	osSemaphoreDef(Netif_SEM);
	Netif_LinkSemaphore = osSemaphoreCreate(osSemaphore(Netif_SEM) , 1 );

	link_arg.netif = &this->gnetif;
	link_arg.semaphore = Netif_LinkSemaphore;

	osThreadDef(LinkThr, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(LinkThr), &link_arg);

	netif_set_up(&this->gnetif);

	ethernetif_notify_conn_changed(&this->gnetif);

}

void net_setConCallback(void(*conCallback)){
	this->conCallback = conCallback;
}

void net_setMode(net_mode_e mode){

	this->mode = mode;

	net_stop();

	if(this->mode == NET_MODE_ETHERNET){
		this->gnetif = this->ethernetif;
	}else{
		this->gnetif = this->wirelessif;
	}

	net_start();

}

void net_start(void){
	netif_set_up(&this->gnetif);
}

void net_stop(void){
	netif_set_down(&this->gnetif);
}

void ethernetif_notify_conn_changed(struct netif *netif){
	this->state = netif_is_link_up(netif) ? NET_UP : NET_DOWN;
	this->conCallback(this->state);
}
