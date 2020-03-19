#include "Q_packet.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

//FIXME: changed opcode enumeration. code should be cleaned.
static Q_packet_validity_e Q_packet_check_validity(Q_packet_s *packet);

Q_packet_s *Q_packet_parse(void *data, uint8_t len){

	Q_packet_s *packet = pvPortMalloc(sizeof(Q_packet_s));

	packet->raw_data = pvPortMalloc(sizeof(char) * len);
	memcpy(packet->raw_data, data, len);

	packet->opcode 	= packet->raw_data[Q_OPCODE_INDEX];

	if(Q_packet_check_validity(packet) == Q_PACKET_VALID){
		packet->payload  = pvPortMalloc(sizeof(char) * (len - Q_PAYLOAD_INDEX));
		memcpy(packet->payload, packet->raw_data + Q_PAYLOAD_INDEX, len - Q_PAYLOAD_INDEX);
		return packet;
	}else{
		vPortFree(packet->raw_data);
		vPortFree(packet);
	}

	return NULL;

}

void Q_packet_free(Q_packet_s *packet){
	if(packet != NULL){
		vPortFree(packet->raw_data);
		vPortFree(packet->payload);
		vPortFree(packet);
	}
}

/** TODO: For compatibility needs, replace sprintf use with a more abstract solution */
char *Q_packet_forge_discover_reply(uint8_t node_id){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * Q_DISCOVERREPLY_BYTELENGTH);
	sprintf(raw_data, "%s%c%c", Q_PACKETID_STRING, Q_CMD_REPLY_DISCOVER, node_id);
	return raw_data;
}

char *Q_packet_forge_poll_reply(uint8_t node_id){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * Q_POLLREPLY_BYTELENGTH);
	sprintf(raw_data, "%s%c%c", Q_PACKETID_STRING, Q_CMD_REPLY_POLL, node_id);
	return raw_data;
}

char *Q_packet_forge_config_reply(uint16_t request_id, uint8_t err_code){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * (Q_CONFIGREPLY_BYTELENGTH));
	sprintf(raw_data, "%s%c%d%c", Q_PACKETID_STRING, Q_CMD_REPLY_CONFIG, request_id, err_code);
	return raw_data;
}

static Q_packet_validity_e Q_packet_check_validity(Q_packet_s *packet){

	Q_packet_validity_e  packet_validity = Q_PACKET_INVALID;

	if(strncmp(packet->raw_data, Q_PACKETID_STRING, Q_PACKETID_BYTELENGTH) == 0){
		if(packet->opcode >= Q_CMD_TRIGGER_START && packet->opcode <= Q_CMD_REPLY_CONFIG){
			packet_validity = Q_PACKET_VALID;
		}
	}

	return packet_validity;

}
