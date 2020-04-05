
#ifndef _Q_UDP_CLIENT_H_
#define _Q_UDP_CLIENT_H_

#include "lwip/api.h"
#include "Q_packet.h"
#include "cueos_config.h"

#define Q_UDP_CLIENT_DEFAULT_PORT 5214
#define Q_UDP_CLIENT_DEFAULT {NULL,NULL,NULL,{},{},{},Q_UDP_CLIENT_UNBOUND,NULL}

#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_MASTER
	#define Q_UDP_CLIENT_GROUPCFG Q_UDP_CLIENT_GROUPCFG_CTRL
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	#define Q_UDP_CLIENT_GROUPCFG Q_UDP_CLIENT_GROUPCFG_DMX
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	#define Q_UDP_CLIENT_GROUPCFG  Q_UDP_CLIENT_GROUPCFG_PYRO
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_MEDIA
	#define Q_UDP_CLIENT_GROUPCFG Q_UDP_CLIENT_GROUPCFG_MEDIA
#endif

typedef enum{
	Q_UDP_CLIENT_GROUPCFG_CTRL 	= 200,
	Q_UDP_CLIENT_GROUPCFG_DMX 	= 201,
	Q_UDP_CLIENT_GROUPCFG_PYRO 	= 202,
	Q_UDP_CLIENT_GROUPCFG_MEDIA = 203,
	Q_UDP_CLIENT_GROUPCFG_OTHER = 204
}Q_client_groupcfg_e;

typedef struct Q_client{
	struct udp_pcb *_pcb;
	struct pbuf *_p_tx;
	ip4_addr_t ip_addr;
	ip4_addr_t mcast_addr;
	ip4_addr_t remote_addr;
}Q_client_s;

void Q_client_init(void);
void Q_client_send(char *raw_packet, uint16_t len);
void Q_client_bind(void);

#endif
