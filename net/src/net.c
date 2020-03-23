/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "cueos_config.h"
#include "leds_driver.h"
#include "ethernet_driver.h"
#include "net.h"

/**============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static net_s this = NET_DEFAULT;
osSemaphoreId_t ethernetif_LinkSemaphore = NULL;
struct link_str ethernetif_link_arg;
const osThreadAttr_t ethernetif_LinkThr_attr = ETHERNETIF_LINKTHR_ATTR;


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

#if cueOS_CONFIG_NET_USE_DHCP
/**
 * DHCP thread. Checks wether an IP has been provided or not and initiates callback on success
 *
 * @param *arg required by osThreadNew, not used here.
 */
static void _net_set_ip(void *arg){

	struct netif gnetif;

	for(;;){

		gnetif = this.mode == NET_MODE_ETHERNET ? this.ethernetif : this.wirelessif;

		if(this.bound_state == NET_UNBOUND){
			if(netif_is_link_up(&gnetif) && dhcp_supplied_address(&gnetif)){
				this.ip_addr = gnetif.ip_addr;
				this.gateway = gnetif.gw;
				this.netmask = gnetif.netmask;
				this.net_ready_callback();
				this.bound_state = NET_BOUND;
				leds_driver_set(LED_NETWORK, LED_BLINK);
			}else{
				leds_driver_set(LED_NETWORK, LED_OFF);
			}
		}

		osDelay(10);

	}

}
#endif

/**
 * setup ethernet interface
 * @warning ethernet driver must be provided in ethernet_driver.h
 * @see ethernet_driver.h for further information regarding the ethernet interface driver
 */
static void _net_setup_ethernetif(void){

	netif_add(&this.ethernetif, &this.ip_addr, &this.netmask, &this.gateway, NULL, &ethernetif_init, &tcpip_input);
	netif_set_default(&this.ethernetif);
	netif_is_link_up(&this.ethernetif) ? netif_set_up(&this.ethernetif) : netif_set_down(&this.ethernetif);

	netif_set_link_callback(&this.ethernetif, ethernetif_update_config);

	ethernetif_LinkSemaphore = osSemaphoreNew(1, 1, NULL);

	const osThreadAttr_t attr = {
		.priority = osPriorityNormal,
		.stack_size = configMINIMAL_STACK_SIZE * 2
	};

	ethernetif_link_arg.netif = &this.ethernetif;
	ethernetif_link_arg.semaphore = ethernetif_LinkSemaphore;

	osThreadNew(ethernetif_set_link, &ethernetif_link_arg, &attr);

#if cueOS_CONFIG_NET_USE_DHCP
	osThreadNew(_net_set_ip, NULL, NULL);
	netif_set_up(&this.ethernetif);
	dhcp_start(&this.ethernetif);
#else
	this.net_ready_callback();
	this.bound_state = NET_BOUND;
	leds_driver_set(LED_NETWORK, LED_BLINK);
#endif

	ethernetif_notify_conn_changed(&this.ethernetif);

}

/**
 * setup wireless interface
 * @warning wireless driver must be provided in ethernet_driver.h
 * @see wireless_driver.h for further information regarding the wireless interface driver
 * TODO: implement wireless interface driver
 */
static void _net_setup_wirelessif(void){

}

/**
 * Setup both wireless end ethernet network interfaces
 *
 * @param *arg required by tcp_init, not used here.
 */
static void _net_setup_if(void *arg){
	_net_setup_ethernetif();
	_net_setup_wirelessif();
	net_set_mode(this.mode);
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser_pool.h for declarations
 *=============================================================================================================================*/

/**
 * Initialises network.
 *
 * @param mode the network mode (either ethernet or wireless)
 * @param net_ready_callback function to be called on initialisation success
 */
void net_init(net_mode_e mode, void *net_ready_callback){
#if cueOS_CONFIG_NET_USE_DHCP
	IP4_ADDR(&this.ip_addr, 0, 0, 0, 0);
	IP4_ADDR(&this.gateway, 0, 0, 0, 0);
	IP4_ADDR(&this.netmask, 0, 0, 0, 0);
#else
	IP4_ADDR(&this.ip_addr, cueOS_CONFIG_NET_STATIC_IP_0,
							cueOS_CONFIG_NET_STATIC_IP_1,
							cueOS_CONFIG_NET_STATIC_IP_2,
							cueOS_CONFIG_NET_STATIC_IP_3);

	IP4_ADDR(&this.gateway, cueOS_CONFIG_NET_STATIC_GW_0,
							cueOS_CONFIG_NET_STATIC_GW_1,
							cueOS_CONFIG_NET_STATIC_GW_2,
							cueOS_CONFIG_NET_STATIC_GW_3);

	IP4_ADDR(&this.netmask, cueOS_CONFIG_NET_STATIC_NM_0,
							cueOS_CONFIG_NET_STATIC_NM_1,
							cueOS_CONFIG_NET_STATIC_NM_2,
							cueOS_CONFIG_NET_STATIC_NM_3);
#endif
	this.net_ready_callback = net_ready_callback;
	tcpip_init(_net_setup_if, NULL);
}

/**
 * Returns the active network interface IP address
 *
 * @return ip4_addr_t the interface's IP address
 */
ip4_addr_t net_get_ip_addr(void){
	return (this.mode == NET_MODE_ETHERNET ? this.ethernetif.ip_addr : this.wirelessif.ip_addr);
}

/**
 * Returns the active network interface gateway address
 *
 * @return ip4_addr_t the interface's gateway address
 */
ip4_addr_t net_get_gateway(void){
	return (this.mode == NET_MODE_ETHERNET ? this.ethernetif.gw : this.wirelessif.gw);
}

/**
 * Sets the network mode to either ethernet or wireless
 *
 * @param mode the mode to be set
 * @see net.h for further information regarding available modes
 */
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
			//wirelessif_notify_conn_changed(&this.wirelessif);
			break;
	}

}

/**
 * Function called on ethernet interface link change
 *
 * @param *netif pointer to the network interface structure
 * @see ethernet_driver.h for further information regarding this callback
 */
void ethernetif_notify_conn_changed(struct netif *netif){
	this.bound_state = NET_UNBOUND;
	this.link_state = netif_is_link_up(netif) ? NET_LINK_UP : NET_LINK_DOWN;
	dhcp_network_changed(netif);
}

/**
 * Function called on wireless interface link change
 *
 * @param *netif pointer to the network interface structure
 * @see wireless_driver.h for further information regarding this callback
 */
//void wirelessif_notify_conn_changed(struct netif *netif){
//	this.link_state = netif_is_link_up(netif) ? NET_LINK_UP : NET_LINK_DOWN;
//}
