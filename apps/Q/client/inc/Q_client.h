
#ifndef _Q_UDP_CLIENT_H_
#define _Q_UDP_CLIENT_H_

#include "lwip/api.h"
#include "Q_packet.h"
#include "cueos_config.h"

#define Q_UDP_CLIENT_DEFAULT_PORT 5214
#define Q_UDP_CLIENT_DEFAULT {NULL,NULL,NULL,{},{},{},Q_UDP_CLIENT_UNBOUND,NULL}

#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_MASTER
	#define Q_CLIENT_GROUPCFG Q_CLIENT_GROUPCFG_CTRL
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	#define Q_CLIENT_GROUPCFG Q_CLIENT_GROUPCFG_DMX
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	#define Q_CLIENT_GROUPCFG  Q_CLIENT_GROUPCFG_PYRO
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_MEDIA
	#define Q_CLIENT_GROUPCFG Q_CLIENT_GROUPCFG_MEDIA
#endif


/**
 * @ingroup Q_client
 * @enum Q_client_groupcfg_e
 * @brief List of node group configurations used to
 * 		  determine a node's multicast address
 */
typedef enum{
	Q_CLIENT_GROUPCFG_CTRL 	= 200,					/*< Node multicast address will be set to 224.0.0.200 (master)*/
	Q_CLIENT_GROUPCFG_DMX 	= 201,					/*< Node multicast address will be set to 224.0.0.201 (DMX nodes)*/
	Q_CLIENT_GROUPCFG_PYRO 	= 202,					/*< Node multicast address will be set to 224.0.0.202 (pyro nodes)*/
	Q_CLIENT_GROUPCFG_MEDIA = 203,					/*< Node multicast address will be set to 224.0.0.203 (media nodes*/
	Q_CLIENT_GROUPCFG_OTHER = 204					/*< Node multicast address will be set to 224.0.0.204 (undefined - other nodes)*/
}Q_client_groupcfg_e;


/**
 * @ingroup Q_client
 * @struct Q_client_s
 * @brief Defines a Q client. Q is a UDP-only protocol specifically
 *  	  deficated to show control an automatisation.
 */
typedef struct Q_client{
	struct udp_pcb *_pcb;							/*< UDP control block*/
	struct pbuf *_p_tx;								/*< TX  data buffer*/
	ip4_addr_t ip_addr;								/*< Client's IP address*/
	ip4_addr_t mcast_addr;							/*< Client's multicast group address*/
}Q_client_s;

void Q_client_init(void);
void Q_client_send(char *raw_packet, uint16_t len);
void Q_client_bind(void);

#endif
