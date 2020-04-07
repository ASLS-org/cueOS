/**
 * @ingroup cueos_apps
 * @defgroup Q_client Q Client
 * WIP, to be detailed
 * @{
 */

#ifndef _Q_UDP_CLIENT_H_
#define _Q_UDP_CLIENT_H_

#include "lwip/api.h"
#include "Q_parser.h"
#include "cueos_config.h"

#define Q_UDP_CLIENT_DEFAULT_PORT 5214	/**< UDP Port number on which the client will be communicating*/

/**
 * @brief Sets default Q_client_s structure parameter values to avoid conflicts
 */
#define Q_UDP_CLIENT_DEFAULT {NULL,NULL,NULL,{},{},{},Q_UDP_CLIENT_UNBOUND,NULL}

#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_MASTER
	#define Q_CLIENT_GROUPCFG Q_CLIENT_GROUPCFG_CTRL			/**< Client is configured as a controller*/
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	#define Q_CLIENT_GROUPCFG Q_CLIENT_GROUPCFG_DMX				/**< Client is configured as a DMX slave*/
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	#define Q_CLIENT_GROUPCFG  Q_CLIENT_GROUPCFG_PYROL			/**< Client is configured as a pyrotechnic slave*/
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_MEDIA
	#define Q_CLIENT_GROUPCFG Q_CLIENT_GROUPCFG_MEDIAL			/**< Client is configured as e media served slave*/
#endif


/**
 * @brief List of node group configurations used to
 * 		  determine a node's multicast address
 */
typedef enum{
	Q_CLIENT_GROUPCFG_CTRL 	= 200,					/**< Node multicast address will be set to 224.0.0.200 (master)*/
	Q_CLIENT_GROUPCFG_DMX 	= 201,					/**< Node multicast address will be set to 224.0.0.201 (DMX nodes)*/
	Q_CLIENT_GROUPCFG_PYRO 	= 202,					/**< Node multicast address will be set to 224.0.0.202 (pyro nodes)*/
	Q_CLIENT_GROUPCFG_MEDIA = 203,					/**< Node multicast address will be set to 224.0.0.203 (media nodes*/
	Q_CLIENT_GROUPCFG_OTHER = 204					/**< Node multicast address will be set to 224.0.0.204 (undefined - other nodes)*/
}Q_client_groupcfg_e;


/**
 * @brief Defines a Q client. Q is a UDP-only protocol specifically
 *  	  deficated to show control an automatisation.
 */
typedef struct Q_client{
	struct udp_pcb *_pcb;							/**< UDP control block*/
	struct pbuf *_p_tx;								/**< TX  data buffer*/
	ip4_addr_t ip_addr;								/**< Client's IP address*/
	ip4_addr_t mcast_addr;							/**< Client's multicast group address*/
}Q_client_s;

void Q_client_init(void);
void Q_client_send(char *raw_packet, uint16_t len);
void Q_client_bind(void);

#endif

/**
 * @} Grouping in Q_client submodule ends
 **/
