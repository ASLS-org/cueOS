/***============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "osc_server.h"
#include "cmsis_os.h"
#include "lwip/udp.h"
#include "ip.h"
#include <string.h>


OSC_server_s osc_server;

/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

static uint8_t _osc_server_roundup_packet(char **osc_packet, uint8_t osc_packet_len){

	uint8_t rounded_packet_len = ((osc_packet_len + 4 - 1) / 4) * 4;

	*osc_packet = pvPortRealloc(*osc_packet, rounded_packet_len * sizeof(uint8_t));

	for(uint8_t i=0; i<rounded_packet_len - osc_packet_len; i++){
		memmove(*osc_packet + osc_packet_len + i, '\0', 1);
	}

	return rounded_packet_len;

}

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

void osc_server_init(uint16_t input_port, uint16_t output_port){

	osc_server.in_port 	= input_port;
	osc_server.out_port = output_port;
	osc_server._pcb 	= udp_new();

	udp_bind(osc_server._pcb, IP_ADDR_ANY, osc_server.in_port);
	udp_recv(osc_server._pcb, _osc_server_receive, NULL);

	/**
	 * TODO: THis is for test purposes only.
	 */
	osDelay(250);													//TODO: check if delay-before-use is strictly required

	osc_server_control(1,2,3,1000);

}

void osc_server_control(osc_media_type_e media_type, osc_control_type_e ctrl_type, uint16_t ctrl_id, uint32_t ctrl_val){

	uint32_t swapped_ctrl_val = ((ctrl_val>>24)&0xff) | ((ctrl_val<<8)&0xff0000) | ((ctrl_val>>8)&0xff00) | ((ctrl_val<<24)&0xff000000);

	char *osc_packet_uri = pvPortMalloc(OSC_MAX_URI_LEN * sizeof(uint8_t));
	char *osc_packet_payload = pvPortMalloc(OSC_MAX_PAYLOAD_LEN * sizeof(uint8_t));

	sprintf(osc_packet_uri,"/%d/%d/%d", media_type, ctrl_type, ctrl_id);

	memmove(osc_packet_payload,",i", 2);
	memmove(osc_packet_payload + 2,&swapped_ctrl_val, 4);

	uint8_t raw_uri_len = strlen(osc_packet_uri);
	uint8_t uri_len = _osc_server_roundup_packet(&osc_packet_uri, raw_uri_len);
	uint8_t payload_len = _osc_server_roundup_packet(&osc_packet_payload,6);
	uint8_t packet_len = uri_len + payload_len;

	char osc_packet[uri_len + payload_len];

	memmove(osc_packet, osc_packet_uri, uri_len);
	memmove(osc_packet + uri_len, osc_packet_payload, payload_len);

	osc_server._p_tx = pbuf_alloc(PBUF_TRANSPORT, packet_len, PBUF_POOL);

	pbuf_take(osc_server._p_tx, osc_packet, packet_len);
	udp_sendto(osc_server._pcb, osc_server._p_tx, IP_ADDR_BROADCAST, osc_server.out_port);

	pbuf_free(osc_server._p_tx);

	vPortFree(osc_packet_uri);
	vPortFree(osc_packet_payload);

}
