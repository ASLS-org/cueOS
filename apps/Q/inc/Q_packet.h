#ifndef _Q_PACKET_H_
#define _Q_PACKET_H_

#include <stdint.h>
#include <stdlib.h>
#include "Q_defs.h"

#define Q_PACKETID_BYTELENGTH 	6
#define Q_OPCODE_BYTELENGTH	  	1
#define Q_HEADER_BYTELENGTH		Q_PACKETID_BYTELENGTH + Q_OPCODE_BYTELENGTH
#define Q_OPCODE_INDEX		 	Q_PACKETID_BYTELENGTH
#define Q_PAYLOAD_INDEX			Q_OPCODE_INDEX

static const uint8_t Q_PACKETID_STRING[Q_PACKETID_BYTELENGTH] = "ASLS-Q";

typedef enum{
	Q_PACKET_INVALID,
	Q_PACKET_VALID
}Q_packet_validity_e;

typedef enum{
	Q_OPCODE_TRIGGER	= 0x00,
	Q_OPCODE_CONTROL	= 0x10,
	Q_OPCODE_LOAD		= 0x20,
	Q_OPCODE_CLEAR		= 0x30,
	Q_OPCODE_PROBE		= 0x40,
	Q_OPCODE_REPLY		= 0x50,
}Q_packet_opcode_cmdtype_e;

typedef enum{
	Q_TRIGGER_CMD_START 	= 0x00,
	Q_TRIGGER_CMD_PAUSE		= 0x01,
	Q_TRIGGER_CMD_RESUME	= 0x02,
	Q_TRIGGER_CMD_STOP		= 0x03,
	Q_CONTROL_CMD_ALLOFF	= 0x00,
	Q_CONTROL_CMD_RESTORE	= 0x01,
	Q_CONTROL_CMD_ACTUATOR	= 0x02,
	Q_LOAD_CMD_PATCH		= 0x00,
	Q_LOAD_CMD_SCENE		= 0x01,
	Q_LOAD_CMD_CHASER		= 0x02,
	Q_LOAD_CMD_EFFECT		= 0x03,
	Q_CLEAR_CMD_PATCH		= 0x00,
	Q_CLEAR_CMD_SCENE		= 0x01,
	Q_CLEAR_CMD_CHASER		= 0x02,
	Q_CLEAR_CMD_EFFECT		= 0x03,
	Q_PROBE_CMD_DISCOVER	= 0x00,
	Q_PROBE_CMD_PATCH		= 0x01,
	Q_PROBE_CMD_POLL		= 0x02,
	Q_REPLY_CMD_DISCOVER	= 0x00,
	Q_REPLY_CMD_PATCH		= 0x01,
	Q_REPLY_CMD_TRIGGER		= 0x02,
	Q_REPLY_CMD_CONTROL		= 0x03,
	Q_REPLY_CMD_LOAD		= 0x04,
	Q_REPLY_CMD_CLEAR		= 0x05,
	Q_REPLY_CMD_POLL		= 0x06,
}Q_packet_opcode_cmd_e;

typedef struct Q_packet_opcode{
	Q_packet_opcode_cmdtype_e type;
	Q_packet_opcode_cmd_e cmd;
	uint8_t raw;
}Q_packet_opcode_s;

typedef struct Q_packet{
	Q_packet_opcode_s opcode;
	uint8_t *raw_data;
	uint8_t *payload;
}Q_packet_s;

Q_packet_s *Q_packet_parse(void *data, uint8_t len);
void Q_packet_free(Q_packet_s *packet);

#endif
