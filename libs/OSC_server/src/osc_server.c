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
#include <math.h>

OSC_server_s osc_server = {
	.is_initialised = 0
};

/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/


/**
 * @brief Determines an integer's digit count.
 *
 * @param i Integer which digit count needs to be determinated.
 * @return uint8_t digit count
 */
static uint8_t _i_to_len(uint16_t i){
	return floor(log10((double)i) + 1);
}

/**
 * @brief Rounds up a predetermined value to its closest multiple of 4
 *
 * @param val value to be rounded up.
 * @return uint8_t rouded up value.
 */
static uint8_t _roundup_4(uint8_t val){
	return ((val + 4 - 1) / 4) * 4;
}

/**
 * @brief Appends data to an OSC packet's data buffer
 *
 * @param *osc_packet Pointer to an OSC packet instance.
 * @param *data Data to be appended to the packet buffer.
 * @param size Size of the data to be appended to the buffer.
 */
static void _osc_packet_cat(OSC_packet_s *osc_packet, const void *data, size_t size){
	memmove(osc_packet->sp, data, size);
	osc_packet->sp += size;
}

/**
 * @brief Rounds up an OSC packet's data buffer size to a multiple of 4
 * by appending NULL characters if needed.
 *
 * @param *osc_packet Pointer to an OSC packet instance.
 */
static void _osc_packet_roundup4(OSC_packet_s *osc_packet){

	uint8_t cur_size = osc_packet->sp - osc_packet->data;
	uint8_t new_size = _roundup_4(cur_size);

	for(size_t i=0; i<(new_size - cur_size);i++){
		_osc_packet_cat(osc_packet, OSC_PACKET_NULL_CHAR, 1);
	}

}

/**
 * @brief OSC packet reception handler.
 *
 * @param arg user supplied argument (udp_pcb.recv_arg)
 * @param pcb the udp_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 */
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
 * @brief Prepares and send an OSC packet over UDP.
 *
 * @param media_type Type of media to be controlled over OSC @see osc_server.h for further informations.
 * @param ctrl_type Type of control to be modified over OSC @see osc_server.h for further informations.
 * @param ctrl_id Identifier of the control to be modified OSC @see osc_server.h for further informations.
 * @param ctrl_val Value of the control to be modified.
 */
void osc_packet_send(osc_media_type_e media_type, osc_control_type_e ctrl_type, uint16_t ctrl_id, uint32_t ctrl_val){

	OSC_packet_s osc_packet;

	uint32_t swapped_ctrl_val 	= ((ctrl_val>>24)&0xff) | ((ctrl_val<<8)&0xff0000) | ((ctrl_val>>8)&0xff00) | ((ctrl_val<<24)&0xff000000);
	uint8_t uri_len 			= _i_to_len(media_type) +  _i_to_len(ctrl_type) + _i_to_len(ctrl_id) + 3;
	uint8_t tot_len 			= OSC_PACKET_PAYLOAD_LEN + _roundup_4(uri_len);

	osc_packet.data	= pvPortMalloc(tot_len * sizeof(uint8_t));
	osc_packet.sp 	= osc_packet.data;

	/**
	 * WARNING: the use of sprintf tends to cause memory leaks...
	 * FIXED by implementing thread-safe printf-stdarg library.
	 */
	osc_packet.sp += sprintf(osc_packet.sp,OSC_PACKET_URI_FORMAT, media_type, ctrl_type, ctrl_id);
	_osc_packet_roundup4(&osc_packet);
	_osc_packet_cat(&osc_packet, ",i", 2);
	_osc_packet_cat(&osc_packet, &swapped_ctrl_val, 4);
	_osc_packet_roundup4(&osc_packet);

	osc_server._p_tx = pbuf_alloc(PBUF_TRANSPORT, tot_len, PBUF_POOL);
	pbuf_take(osc_server._p_tx, osc_packet.data, tot_len);
	udp_sendto(osc_server._pcb, osc_server._p_tx, IP_ADDR_BROADCAST, cueOS_CONFIG_OSC_OUT_PORT);

	vPortFree(osc_packet.data);

	pbuf_free(osc_server._p_tx);

}






























