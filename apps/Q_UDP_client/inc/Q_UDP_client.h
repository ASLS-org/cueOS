
#ifndef _Q_CLIENT_H_
#define _Q_CLIENT_H_

#include "lwip/api.h"
#include "Q_defs.h"
#include "Q_packet.h"


#define Q_CLIENT_DEFAULT_PORT 5214
#define Q_CLIENT_DEFAULT {NULL,NULL,NULL,{},{},{},Q_CLIENT_UNBOUND,NULL}


typedef void(*Q_client_frwrd_pckt_f)(Q_packet_s *packet);

typedef enum{
	Q_CLIENT_GROUPCFG_CTRL 	= 200,
	Q_CLIENT_GROUPCFG_DMX 	= 201,
	Q_CLIENT_GROUPCFG_PYRO 	= 202,
	Q_CLIENT_GROUPCFG_MEDIA = 203,
	Q_CLIENT_GROUPCFG_OTHER = 204
}Q_client_groupcfg_e;

typedef struct Q_client{
	struct udp_pcb *_pcb;
	struct pbuf *_p_tx;
	ip4_addr_t ip_addr;
	ip4_addr_t mcast_addr;
	ip4_addr_t remote_addr;
	Q_client_frwrd_pckt_f frwrd_pckt;
}Q_client_s;

void Q_client_init(Q_client_groupcfg_e groupcfg);
void Q_client_send(char *raw_packet, uint16_t len);
void Q_client_bind(void);

#endif
