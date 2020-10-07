/**
 * @ingroup libs
 * @defgroup OSC_server Open Sound Control Server
 * Handles inter-system communication through Open SOund Control protocol.
 * @{
 */

#ifndef OSC_SERVER_H_
#define OSC_SERVER_H_

#include <stdint.h>
#include <stdlib.h>
#include "lwip/api.h"

#define OSC_PACKET_URI_PATH_DELIMITOR 	'/'
#define OSC_PACKET_MESSAGE_DELIMITOR 	','
#define OSC_PACKET_DATA_TYPE_INTEGER 	'i'
#define OSC_PACKET_DATA_TYPE_FLOAT		'f'
#define OSC_PACKET_DATA_TYPE_STTRING	's'
#define OSC_PACKET_NULL_CHAR	 		'\0'
#define OSC_PACKET_URI_FORMAT			"/%d/%d/%d"

#define OSC_PACKET_PAYLOAD_LEN 			8
#define OSC_PACKET_URI_BASEPATH_LEN 	4

/**
 * @ingroup OSC_server
 * @enum osc_media_type_e
 * @brief List of OSC-controllable media types (As implemented by ASLS)
 */
typedef enum{
	OSC_MEDIATYPE_LIGHTING,				/**< Targets OSC-Compatible lighting systems on the network*/
	OSC_MEDIATYPE_PYROTECHNICS,			/**< Targets OSC-Compatible pyrotechnic systems on the network*/
	OSC_MEDIATYPE_MEDIASERVER			/**< Targets OSC-Compatible media servrs on the network*/
}osc_media_type_e;

/**
 * @ingroup OSC_server
 * @enum osc_control_type_e
 * @brief List of OSC control types (As implemented by ASLS)
 */
typedef enum{
	OSC_CTRLTYPE_CUE,					/**< The parameter to be controlled is a cue (value may either be 0 or 255)*/
	OSC_CTRLTYPE_VAL					/**< The parameter to be controlled is a value (value may be defined between 0-255 range)*/
}osc_control_type_e;


/**
 * @brief DEfines en OSC packet instance.
 * TODO: this is not a RAM-efficient implementation... EIther leave packet data as raw or slice it in URI/Payload but not both.
 */
typedef struct{
	char *data;							/**< Packet data buffer*/
	char *sp;							/**< String pointer */
}OSC_packet_s;


/**
 * @brief Defines an OSC server instance. Open Sound Control (OSC) is a protocol
 * for networking sound synthesizers, computers, and other multimedia devices for
 * purposes such as musical performance or show control.
 */
typedef struct{
	struct udp_pcb *_pcb;							/**< UDP control block*/
	struct pbuf *_p_tx;								/**< TX  data buffer*/
	uint8_t is_initialised;							/**< Server initialisation state*/
}OSC_server_s;


void osc_server_init(void);
void osc_server_control(char *media_type, char *ctrl_type, char *ctrl_id, uint32_t ctrl_val);


/**
 * @} Grouping in osc_server submodule ends
 **/

#endif
