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

#define OSC_MAX_URI_LEN 12
#define OSC_MAX_PAYLOAD_LEN 8
#define OSC_MAX_PACKET_LEN OSC_MAX_URI_LEN + OSC_MAX_PAYLOAD_LEN

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
void osc_server_control(osc_media_type_e media_type, osc_control_type_e ctrl_type, uint16_t ctrl_id, uint32_t ctrl_val);


/**
 * @} Grouping in osc_server submodule ends
 **/

#endif
