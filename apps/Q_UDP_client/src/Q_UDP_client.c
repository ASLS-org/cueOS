/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "igmp.h"
#include "net.h"
#include "leds_driver.h"
#include "Q_UDP_client.h"
#include "Q_packet.h"


/**============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static Q_client_s this;


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

static void _Q_UDP_client_poll_reply(void){
	char *reply_packet = Q_packet_forge_poll_reply(65);
	Q_client_send(reply_packet, strlen(reply_packet));
	vPortFree(reply_packet);
}

static void _Q_UDP_client_discover_reply(void){
	char *reply_packet = Q_packet_forge_discover_reply(65);
	Q_client_send(reply_packet, strlen(reply_packet));
	vPortFree(reply_packet);
}

//FIXME: clean and update switch statement
static void _Q_client_receive(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){

	Q_packet_s *packet = Q_packet_parse(p->payload, p->len);

	if(packet != NULL){

		switch(packet->opcode){
			case Q_CMD_TRIGGER_START:  /* Complete cue triggering */	break;
			case Q_CMD_TRIGGER_PAUSE:  /* Complete cue triggering */	break;
			case Q_CMD_TRIGGER_RESUME: /* Complete cue triggering */	break;
			case Q_CMD_TRIGGER_STOP:   /* Complete cue triggering */	break;
			case Q_CMD_PROBE_DISCOVER: _Q_UDP_client_discover_reply(); 	break;
			case Q_CMD_PROBE_POLL: 	   _Q_UDP_client_poll_reply(); 		break;
		}

		Q_packet_free(packet);

	}

	pbuf_free(p);

}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser_pool.h for declarations
 *=============================================================================================================================*/

/**
 * Initialises Q client.
 *
 * @param groupcfg the group configuration byte to be used
 * @see Q_client.h for further information regarding group configuration byte
 */
void Q_client_init(void){

	IP4_ADDR(&this.mcast_addr, 224, 0, 0, Q_UDP_CLIENT_GROUPCFG);
	IP4_ADDR(&this.remote_addr, 224, 0, 0, Q_UDP_CLIENT_GROUPCFG_CTRL);

	this._pcb = udp_new();

	udp_bind(this._pcb, IP_ADDR_ANY, Q_UDP_CLIENT_DEFAULT_PORT);
	udp_recv(this._pcb, _Q_client_receive, NULL);

}

/**
 * Sends a packet over UDP
 *
 * @param *raw_packet pointer to the raw packet data
 * @len length of the packet in bytes
 */
void Q_client_send(char *raw_packet, uint16_t len){
	this._p_tx = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL);
	pbuf_take(this._p_tx, raw_packet, len);
	udp_send(this._pcb, this._p_tx);
	pbuf_free(this._p_tx);
}

/**
 * Etstbliches a multicast join request over UDP
 * Multicast address is determined by the group config byte provided during initialisation
 * @see Q_client.h for further information regarding group configuration options
 */
void Q_client_bind(void){
	//FIXME: force join/leave is a bit ugly, I think it could be improved
	igmp_leavegroup(&this.ip_addr, &this.mcast_addr);
	this.ip_addr = net_get_ip_addr();
	igmp_joingroup(&this.ip_addr, &this.mcast_addr);
}

