/*
 * ASLS_network_config.c
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "ASLS_ntw_interface.h"

static ASLS_ntw_iface *this;
osSemaphoreId Netif_LinkSemaphore = NULL;
struct link_str link_arg;


static ASLS_ntw_iface_state ASLS_ntw_iface_getState(void);
//static void ASLS_ntw_iface_setConCallback(void * onConCallback);
static void ASLS_ntw_iface_setAddr(uint8_t addr[4]);
static void ASLS_ntw_iface_setGw(uint8_t gw[4]);
static void ASLS_ntw_iface_setMask(uint8_t mask[4]);
static void ASLS_ntw_iface_up();
static void ASLS_ntw_iface_down();
static void _ASLS_ntw_iface_setupPhy(void);

ASLS_ntw_iface *ASLS_ntw_iface_init(uint8_t localAddr[4], uint8_t netmask[4], uint8_t gateway[4]){

	this = malloc(sizeof(ASLS_ntw_iface));

	this->_state = IFACE_DOWN;

	IP4_ADDR(&this->_localAddr,
			 localAddr[0],
			 localAddr[1],
			 localAddr[2],
			 localAddr[3]);

	IP4_ADDR(&this->_gateway,
			 gateway[0],
			 gateway[1],
			 gateway[2],
			 gateway[3]);

	IP4_ADDR(&this->_netmask,
			 netmask[0],
			 netmask[1],
			 netmask[2],
			 netmask[3]);

	this->getState			= ASLS_ntw_iface_getState;
	this->setNetmask  		= ASLS_ntw_iface_setMask;
	this->setGateway  		= ASLS_ntw_iface_setGw;
	this->setAddr  	  		= ASLS_ntw_iface_setAddr;

	this->up				= ASLS_ntw_iface_up;
	this->down				= ASLS_ntw_iface_down;

	tcpip_init(_ASLS_ntw_iface_setupPhy, NULL );


	return this;

}


static ASLS_ntw_iface_state ASLS_ntw_iface_getState(void){
	return this->_state;
}


static void ASLS_ntw_iface_setAddr(uint8_t addr[4]){
	IP4_ADDR(&this->_localAddr, addr[0], addr[1], addr[2], addr[3]);
	netif_set_ipaddr(&this->_gnetif,&this->_localAddr);
}

static void ASLS_ntw_iface_setGw(uint8_t gw[4]){
	IP4_ADDR(&this->_gateway, gw[0], gw[1], gw[2], gw[3]);
	netif_set_gw(&this->_gnetif,&this->_gateway);
}

static void ASLS_ntw_iface_setMask(uint8_t mask[4]){
	IP4_ADDR(&this->_netmask, mask[0], mask[1], mask[2], mask[3]);
	netif_set_netmask(&this->_gnetif,&this->_netmask);
}


static void ASLS_ntw_iface_up(){
	netif_set_up(&this->_gnetif);
}

static void ASLS_ntw_iface_down(){
	netif_set_down(&this->_gnetif);
}


static void _ASLS_ntw_iface_setupPhy(void){
	netif_add(&this->_gnetif, &this->_localAddr, &this->_netmask, &this->_gateway, NULL, &ethernetif_init, &tcpip_input);
	netif_set_default(&this->_gnetif);

	netif_is_link_up(&this->_gnetif) ? netif_set_up(&this->_gnetif) : netif_set_down(&this->_gnetif);
	netif_set_link_callback(&this->_gnetif, ethernetif_update_config);

	osSemaphoreDef(Netif_SEM);
	Netif_LinkSemaphore = osSemaphoreCreate(osSemaphore(Netif_SEM) , 1 );

	link_arg.netif = &this->_gnetif;
	link_arg.semaphore = Netif_LinkSemaphore;

	osThreadDef(LinkThr, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(LinkThr), &link_arg);

	netif_set_up(&this->_gnetif);

	ethernetif_notify_conn_changed(&this->_gnetif);

}

void ethernetif_notify_conn_changed(struct netif *netif){
	this->_state = netif_is_link_up(netif) ? IFACE_UP : IFACE_DOWN;
}
