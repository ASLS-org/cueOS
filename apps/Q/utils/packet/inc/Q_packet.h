/**
 * @ingroup Q
 * @defgroup Q_utils
 * Useful code snippets to be used within Q server/client implementation
 * @{
 *
 * @defgroup Q_packet
 * Q is an UDP based protocol which aims for a more universal approach of Show Control. Q packets are simply made of a single byte OPCODE
 * and a variable length data payload conveying cue-specific information.
 * @{
 */

#ifndef _Q_PACKET_H_
#define _Q_PACKET_H_

#include <stdint.h>
#include <stdlib.h>

#define Q_PACKETID_BYTELENGTH 		6
#define Q_OPCODE_BYTELENGTH	  		1
#define Q_REQUESTID_BYTELENGTH		2
#define Q_CONFIGERR_BYTELENGTH		1
#define Q_NODETYPE_BYTELENGTH		1
#define Q_NODEID_BYTELENGTH			1

#define Q_HEADER_BYTELENGTH			Q_PACKETID_BYTELENGTH + Q_OPCODE_BYTELENGTH
#define Q_CONFIGREPLY_BYTELENGTH	Q_HEADER_BYTELENGTH + Q_REQUESTID_BYTELENGTH + Q_CONFIGERR_BYTELENGTH
#define Q_DISCOVERREPLY_BYTELENGTH	Q_HEADER_BYTELENGTH + Q_NODETYPE_BYTELENGTH + Q_NODEID_BYTELENGTH
#define Q_POLLREPLY_BYTELENGTH		Q_DISCOVERREPLY_BYTELENGTH

#define Q_OPCODE_INDEX		 		Q_PACKETID_BYTELENGTH
#define Q_PAYLOAD_INDEX				Q_OPCODE_INDEX

#define Q_PACKETID_STRING 			"ASLS-Q"

/**
 * @ingroup Q_packet
 * @enum Q_packet_opcode_cmd_e
 * @brief List of Q packet opcodes
 */
typedef enum{
	Q_CMD_TRIGGER_START,						/*< Starts a cue*/
	Q_CMD_TRIGGER_PAUSE,						/*< Pauses a cue*/
	Q_CMD_TRIGGER_RESUME,						/*< Resumes a paused cue*/
	Q_CMD_TRIGGER_STOP,							/*< Stops a started cue*/
	Q_CMD_CONTROL_ALLOFF,						/*< Sets all outputs to OFF state*/
	Q_CMD_CONTROL_RESTORE,						/*< Restores all outputs to their state prior to ALLOFF*/
	Q_CMD_CONTROL_ACTUATOR,						/*< Controls an individual actuator channe value*/
	Q_CMD_PROBE_DISCOVER,						/*< Discover all nodes on the network*/
	Q_CMD_PROBE_POLL,							/*< Poll a specific probe*/
	Q_CMD_PROBE_DIAGNOSTIC,						/*< Diagnostic a specific node*/ //TODO: maybe remove this and pass everything to poll
	Q_CMD_REPLY_DISCOVER,						/*< Reply to a discover command*/
	Q_CMD_REPLY_POLL,							/*< Reply to a poll command*/
	Q_CMD_REPLY_DIAGNOSTIC,						/*< Reply to a diagnostic command*/
}Q_packet_opcode_cmd_e;


/**
 * @ingroup Q_packet
 * @struct Q_packet
 * @brief Defines a Q packet object
 */
typedef struct Q_packet{
	Q_packet_opcode_cmd_e opcode;				/*< packet opcode @see Q_packet_opcode_cmd_e*/
	char *raw_data;								/*< packet raw unparsed data*/
	char *payload;								/*< packet variable length payload*/
}Q_packet_s;


Q_packet_s *Q_packet_parse(void *data, uint8_t len);
char *Q_packet_forge_discover_reply(uint8_t node_id);
char *Q_packet_forge_poll_reply(uint8_t node_id);
char *Q_packet_forge_config_reply(uint16_t request_id, uint8_t err_code);
void Q_packet_free(Q_packet_s *packet);

#endif

/**
 * @} Grouping in Q_packet submodule ends
 * @} Grouping in Q_utils module ends
 **/

