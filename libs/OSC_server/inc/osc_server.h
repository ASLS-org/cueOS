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

typedef enum{
	OSC_MEDIATYPE_LIGHTING,
	OSC_MEDIATYPE_PYROTECHNICS,
	OSC_MEDIATYPE_MEDIASERVER
}osc_media_type_e;

typedef enum{
	OSC_CTRLTYPE_CUE,
	OSC_CTRLTYPE_VAL
}osc_control_type_e;


/**
 * @brief Defines an OSC server instance. Open Sound Control (OSC) is a protocol
 * for networking sound synthesizers, computers, and other multimedia devices for
 * purposes such as musical performance or show control.
 */
typedef struct{
	struct udp_pcb *_pcb;							/**< UDP control block*/
	struct pbuf *_p_tx;								/**< TX  data buffer*/
	uint16_t in_port;
	uint16_t out_port;
}OSC_server_s;


void osc_server_init(uint16_t input_port, uint16_t output_port);
void osc_server_control(osc_media_type_e media_type, osc_control_type_e ctrl_type, uint16_t ctrl_id, uint32_t ctrl_val);


/**
 * @} Grouping in osc_server submodule ends
 **/

#endif
