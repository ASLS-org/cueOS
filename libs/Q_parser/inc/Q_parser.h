/**
 * @ingroup libs
 * @defgroup Q_parser Q parser
 * Parses "Q" formated UDP packets. Q packets are simply made of a single OPCODE
 * byte and a variable length data payload conveying cue-specific information.
 * @{
 */

#ifndef _Q_PARSER_H_
#define _Q_PARSER_H_

#include <stdint.h>
#include <stdlib.h>

#define Q_PACKETID_BYTELENGTH 		6			/**< Q packet "packet id" field length in bytes*/
#define Q_OPCODE_BYTELENGTH	  		1			/**< Q packet "opcode" field length in bytes*/
#define Q_REQUESTID_BYTELENGTH		2			/**< Q packet "request id" field length in bytes*/
#define Q_NODETYPE_BYTELENGTH		1			/**< Q packet "node type" field length in bytes*/
#define Q_NODEID_BYTELENGTH			1			/**< Q packet "node id" field length in bytes*/

#define Q_HEADER_BYTELENGTH			Q_PACKETID_BYTELENGTH + Q_OPCODE_BYTELENGTH							/**< Q packet header length in bytes*/
#define Q_DISCOVERREPLY_BYTELENGTH	Q_HEADER_BYTELENGTH + Q_NODETYPE_BYTELENGTH + Q_NODEID_BYTELENGTH	/**< Q "discover" reply packet length in bytes*/
#define Q_POLLREPLY_BYTELENGTH		Q_DISCOVERREPLY_BYTELENGTH											/**< Q "poll" reply packet length in bytes*/

#define Q_OPCODE_INDEX		 		Q_PACKETID_BYTELENGTH	/**< Q packet "opcode" byte index*/
#define Q_PAYLOAD_INDEX				Q_OPCODE_INDEX			/**< Q packet "payload" index*/

#define Q_PACKETID_STRING 			"ASLS-Q"	/**< Q packet "packeid" string value*/

/**
 * @ingroup Q_packet
 * @enum Q_packet_opcode_cmd_e
 * @brief List of Q packet opcodes
 */
typedef enum{
	Q_CMD_TRIGGER_START,						/**< Starts a cue*/
	Q_CMD_TRIGGER_PAUSE,						/**< Pauses a cue*/
	Q_CMD_TRIGGER_RESUME,						/**< Resumes a paused cue*/
	Q_CMD_TRIGGER_STOP,							/**< Stops a started cue*/
	Q_CMD_CONTROL_ALLOFF,						/**< Sets all outputs to OFF state*/
	Q_CMD_CONTROL_RESTORE,						/**< Restores all outputs to their state prior to ALLOFF*/
	Q_CMD_CONTROL_ACTUATOR,						/**< Controls an individual actuator channe value*/
	Q_CMD_PROBE_DISCOVER,						/**< Discover all nodes on the network*/
	Q_CMD_PROBE_POLL,							/**< Poll a specific probe*/
	Q_CMD_PROBE_DIAGNOSTIC,						/**< Diagnostic a specific node*/ //TODO: maybe remove this and pass everything to poll
	Q_CMD_REPLY_DISCOVER,						/**< Reply to a discover command*/
	Q_CMD_REPLY_POLL,							/**< Reply to a poll command*/
	Q_CMD_REPLY_DIAGNOSTIC,						/**< Reply to a diagnostic command*/
}Q_packet_opcode_cmd_e;


/**
 * @brief Q packet structure object
 */
typedef struct {
	Q_packet_opcode_cmd_e opcode;				/**< packet opcode @see Q_packet_opcode_cmd_e*/
	char *raw_data;								/**< packet raw unparsed data*/
	char *payload;								/**< packet variable length payload*/
}Q_packet_s;


Q_packet_s *Q_parser_parse(void *data, uint8_t len);
char *Q_parser_forge_discover_reply(uint8_t node_id);
char *Q_parser_forge_poll_reply(uint8_t node_id);
void Q_parser_free(Q_packet_s *packet);

#endif

/**
 * @} Grouping in Q_parser submodule ends
 **/
