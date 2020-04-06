/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "Q_packet.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Checks the integrity of a packet by checting packet id string and opcode
 *
 * @param *packet pointer to packet instance to be checked
 * @return uint8_t 1 on success, 0 otherwise.
 */
static uint8_t _Q_packet_check_validity(Q_packet_s *packet){
	if(strncmp(packet->raw_data, Q_PACKETID_STRING, Q_PACKETID_BYTELENGTH) == 0){
		if(packet->opcode >= Q_CMD_TRIGGER_START && packet->opcode <= Q_CMD_REPLY_DIAGNOSTIC){
			return 1;
		}
	}
	return 0;
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see Q_packet.h for declarations
 *=============================================================================================================================*/

/**
 * @brief parses and converts received data into a Q_packet instance
 *
 * @param *data pointer to data buffer
 * @param len length of the data contained within the buffer in bytes
 * @return Q_packet_s* pointer to the created Q packet instance, NULL on fail
 */
Q_packet_s *Q_packet_parse(void *data, uint8_t len){

	Q_packet_s *packet = pvPortMalloc(sizeof(Q_packet_s));

	packet->raw_data = pvPortMalloc(sizeof(char) * len);
	memcpy(packet->raw_data, data, len);

	packet->opcode 	= packet->raw_data[Q_OPCODE_INDEX];

	if(_Q_packet_check_validity(packet)){
		packet->payload  = pvPortMalloc(sizeof(char) * (len - Q_PAYLOAD_INDEX));
		memcpy(packet->payload, packet->raw_data + Q_PAYLOAD_INDEX, len - Q_PAYLOAD_INDEX);
		return packet;
	}else{
		vPortFree(packet->raw_data);
		vPortFree(packet);
	}

	return NULL;

}

/**
 * @brief Frees packet's dynamically allocated ressources
 *
 * @param *packet pointer to packet instance to be freed
 */
void Q_packet_free(Q_packet_s *packet){
	if(packet != NULL){
		vPortFree(packet->raw_data);
		vPortFree(packet->payload);
		vPortFree(packet);
	}
}

/**
 * @brief Forges a discover reply packet using provided node's id
 *
 * @param node_id node identifier
 * @return char* pointer to raw data buffer to be sent
 */
char *Q_packet_forge_discover_reply(uint8_t node_id){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * Q_DISCOVERREPLY_BYTELENGTH);
	sprintf(raw_data, "%s%c%c", Q_PACKETID_STRING, Q_CMD_REPLY_DISCOVER, node_id);
	return raw_data;
}

/**
 * @brief Forges a poll reply packet using provided node's id
 *
 * @param node_id node identifier
 * @return char* pointer to raw data buffer to be sent
 */
char *Q_packet_forge_poll_reply(uint8_t node_id){
	char *raw_data = pvPortMalloc(sizeof(uint8_t) * Q_POLLREPLY_BYTELENGTH);
	sprintf(raw_data, "%s%c%c", Q_PACKETID_STRING, Q_CMD_REPLY_POLL, node_id);
	return raw_data;
}
