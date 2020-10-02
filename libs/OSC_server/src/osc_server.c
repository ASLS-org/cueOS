/***============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "osc_server.h"
#include "cueos_config.h"
#include "cmsis_os.h"
#include "lwip/udp.h"
#include "ip.h"
#include <string.h>


OSC_server_s osc_server = {
	.is_initialised = 0
};

/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Appends a new path to the URI string buffer
 *
 *TODO: this is a bit dodgy,find a better way to forge URI string.
 *
 * @param **osc_packt_uri pointer to the URI string buffer
 * @param *uri_path string path to be appended to the URI string buffer
 *
 */
static void _osc_server_uri_append(char **osc_packt_uri, char *uri_path){
	memmove(*osc_packt_uri + strlen(*osc_packt_uri), "/", 1);
	memmove(*osc_packt_uri + strlen(*osc_packt_uri), uri_path, strlen(uri_path));
}

/**
 * @brief Rounds up packet length to a multiple of 4 by appending NULL bytes.
 *
 * @param **osc_packet pointer to packet to be rounded up
 * @param osc_packet_len length of the packet in bytes
 * @return uint8_t new length of the packet in bytes
 */
static uint8_t _osc_server_roundup_packet(char **osc_packet, uint8_t osc_packet_len){


	/*
	 * FIXME!! Buffer may be overflowing here...
	 */

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

/**
 * @brief Initialises the OSC server instance
 *
 * @param input_port Input port of the OSC Server
 * @param output_port Output port of the OSC Server
 */
void osc_server_init(void){
	osc_server._pcb = udp_new();
	udp_bind(osc_server._pcb, IP_ADDR_ANY, cueOS_CONFIG_OSC_IN_PORT);
	udp_recv(osc_server._pcb, _osc_server_receive, NULL);
}

/**
 * @brief Brodcasts an OSC control command over UDP
 *
 * @param media_type Type of media to be controlled @see osc_media_type_e.
 * @param osc_control_type_e Type of control to be modified @see osc_control_type_e.
 * @param ctrl_id Identifier of the control to be modified.
 * @param ctrl_val value to be applied
 */
void osc_server_control(osc_media_type_e media_type, osc_control_type_e ctrl_type, uint16_t ctrl_id, uint32_t ctrl_val){

	uint32_t swapped_ctrl_val = ((ctrl_val>>24)&0xff) | ((ctrl_val<<8)&0xff0000) | ((ctrl_val>>8)&0xff00) | ((ctrl_val<<24)&0xff000000);

	char media_type_buf[1];
	char ctrl_type_buf[1];
	char ctrl_id_buf[5];

	itoa(media_type, media_type_buf, 10);
	itoa(ctrl_type, ctrl_type_buf, 10);
	itoa(ctrl_id, ctrl_id_buf, 10);

	char *osc_packet_uri = pvPortMalloc(OSC_MAX_URI_LEN * sizeof(uint8_t));
	char *osc_packet_payload = pvPortMalloc(OSC_MAX_PAYLOAD_LEN * sizeof(uint8_t));

	/*
	 * TODO: Create function to handle URI string creation
	 */
	_osc_server_uri_append(&osc_packet_uri, media_type_buf);
	_osc_server_uri_append(&osc_packet_uri, ctrl_type_buf);
	_osc_server_uri_append(&osc_packet_uri, ctrl_id_buf);

	/*
	 * TODO: Create function to handle payload creation
	 */
	memmove(osc_packet_payload,",i", 2);
	memmove(osc_packet_payload + 2,&swapped_ctrl_val, 4);

	uint8_t raw_uri_len = strlen(osc_packet_uri);
	uint8_t uri_len = _osc_server_roundup_packet(&osc_packet_uri, raw_uri_len);
	uint8_t payload_len = _osc_server_roundup_packet(&osc_packet_payload,6);
	uint8_t packet_len = uri_len + payload_len;

	char osc_packet[packet_len];

	memmove(osc_packet, osc_packet_uri, uri_len);
	memmove(osc_packet + uri_len, osc_packet_payload, payload_len);

	osc_server._p_tx = pbuf_alloc(PBUF_TRANSPORT, packet_len, PBUF_POOL);
	pbuf_take(osc_server._p_tx, osc_packet, packet_len);
	udp_sendto(osc_server._pcb, osc_server._p_tx, IP_ADDR_BROADCAST, cueOS_CONFIG_OSC_OUT_PORT);

	vPortFree(osc_packet_uri);
	vPortFree(osc_packet_payload);
	pbuf_free(osc_server._p_tx);


}
