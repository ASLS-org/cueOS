#include "Q_packet.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

static Q_packet_validity_e Q_packet_check_validity(Q_packet_s *packet);

Q_packet_s *Q_packet_parse(void *data, uint8_t len){

	Q_packet_s *packet = pvPortMalloc(sizeof(Q_packet_s));

	packet->raw_data = pvPortMalloc(sizeof(uint8_t) * len);
	memcpy(packet->raw_data, data, len);
	packet->opcode.raw 	= packet->raw_data[Q_OPCODE_INDEX];
	packet->opcode.type = packet->opcode.raw & 0xF0;
	packet->opcode.cmd 	= packet->opcode.raw & 0x0F;

	if(Q_packet_check_validity(packet) == Q_PACKET_VALID){
		packet->payload  = pvPortMalloc(sizeof(uint8_t) * (len - Q_PAYLOAD_INDEX));
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

char *Q_packet_forge_discover_reply(uint8_t id){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * (Q_HEADER_BYTELENGTH + 1));
	sprintf(raw_data, "%s%c%c", Q_PACKETID_STRING, Q_OPCODE_REPLY | Q_REPLY_CMD_DISCOVER, id);
	return raw_data;
}

char *Q_packet_forge_poll_reply(uint8_t id){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * (Q_HEADER_BYTELENGTH + 1));
	sprintf(raw_data, "%s%c%c", Q_PACKETID_STRING, Q_OPCODE_REPLY | Q_REPLY_CMD_POLL, id);
	return raw_data;
}

static Q_packet_validity_e Q_packet_check_validity(Q_packet_s *packet){

	Q_packet_validity_e  packet_validity = Q_PACKET_INVALID;

	if(strncmp(packet->raw_data, Q_PACKETID_STRING, Q_PACKETID_BYTELENGTH) == 0){
		switch(packet->opcode.type){
			case Q_OPCODE_TRIGGER:
				if(packet->opcode.cmd >= Q_TRIGGER_CMD_START &&
				   packet->opcode.cmd  <= Q_TRIGGER_CMD_STOP){
					packet_validity = Q_PACKET_VALID;
				}
				break;
			case Q_OPCODE_CONTROL:
				if(packet->opcode.cmd >= Q_CONTROL_CMD_ALLOFF &&
				   packet->opcode.cmd <= Q_CONTROL_CMD_ACTUATOR){
					packet_validity = Q_PACKET_VALID;
				}
				break;
			case Q_OPCODE_LOAD:
				if(packet->opcode.cmd >= Q_LOAD_CMD_PATCH &&
				   packet->opcode.cmd <= Q_LOAD_CMD_EFFECT){
					packet_validity = Q_PACKET_VALID;
				}
				break;
			case Q_OPCODE_CLEAR:
				if(packet->opcode.cmd >= Q_CLEAR_CMD_PATCH &&
				   packet->opcode.cmd <= Q_CLEAR_CMD_EFFECT){
					packet_validity = Q_PACKET_VALID;
				}
				break;
			case Q_OPCODE_PROBE:
				if(packet->opcode.cmd >= Q_PROBE_CMD_DISCOVER &&
				   packet->opcode.cmd <= Q_PROBE_CMD_POLL){
					packet_validity = Q_PACKET_VALID;
				}
				break;
			case Q_OPCODE_REPLY:
				if(packet->opcode.cmd >= Q_REPLY_CMD_DISCOVER &&
				   packet->opcode.cmd <= Q_REPLY_CMD_POLL){
					packet_validity = Q_PACKET_VALID;
				}
				break;
		}
	}

	return packet_validity;

}
