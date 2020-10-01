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


/**
 * @brief Defines an OSC server instance. Open Sound Control (OSC) is a protocol
 * for networking sound synthesizers, computers, and other multimedia devices for
 * purposes such as musical performance or show control.
 */
typedef struct{
	struct udp_pcb *_pcb;							/**< UDP control block*/
	struct pbuf *_p_tx;								/**< TX  data buffer*/
	struct pbuf *_p_rx;								/**< RX  data buffer*/
	ip4_addr_t ip_addr;								/**< Server's IP address*/
	ip4_addr_t mcast_addr;							/**< Server's multicast group address*/
}OSC_server_s;


void osc_server_init(void);
void osc_server_send(char *raw_packet, uint16_t len);


/**
 * @} Grouping in osc_server submodule ends
 **/

#endif
