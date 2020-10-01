/***============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "osc_server.h"
#include "cmsis_os.h"
#include "ip.h"
#include <string.h>


OSC_server_s osc_server;

/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

static void _osc_server_receive(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){
	if(p!=NULL){
		pbuf_free(p);
	}
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see osc_server.h for declarations
 *=============================================================================================================================*/

void osc_server_init(void){
	osc_server._pcb = udp_new();
	//ip_set_option(osc_server._pcb, SOF_BROADCAST);
	udp_bind(osc_server._pcb, IP_ADDR_ANY, 5214);
	udp_recv(osc_server._pcb, _osc_server_receive, NULL);
	osDelay(500);
	//osc_server_send("/oscillator/4/frequency\0,i\0\0\0F",32);
	osc_server_send("/oscillator/4/frequency\0,i\0\0\xFF\x00",32);
	osDelay(250);
	osc_server_send("/oscillator/4/frequency\0,i\0\0\x00\x00",32);
}

void osc_server_send(char *raw_packet, uint16_t len){
	osc_server._p_tx = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL);
	pbuf_take(osc_server._p_tx, raw_packet, len);
//	udp_send(osc_server._pcb, osc_server._p_tx);
	udp_sendto(osc_server._pcb, osc_server._p_tx, IP_ADDR_BROADCAST, 5214);
	pbuf_free(osc_server._p_tx);
}
