/*
* ASLS_network.h
 *
 *  Created on: 21 Nov 2019
 *      Author: root
 */

#ifndef ASLS_DRIVERS_INC_NETWORK_ASLS_NTW_DRIVER_H_
#define ASLS_DRIVERS_INC_NETWORK_ASLS_NTW_DRIVER_H_

#include "ASLS_ntw_interface.h"
#include "ASLS_ntw_tcpClient.h"

//ASLS_ntw_tcp_QParser MACROS definition
#define Q_PACKETID_LENGTH 	6
#define Q_OPCODE_LENGTH		1
#define Q_OPCODE_INDEX 		Q_PACKETID_LENGTH
#define Q_PAYLOAD_INDEX 	Q_OPCODE_INDEX + Q_OPCODE_LENGTH - 1
#define Q_PACKET_TERMBYTE 	1


enum Q_opCode{
	Q_OPCODE_START		= 0x00,
	Q_OPCODE_PAUSE		= 0x01,
	Q_OPCODE_RESUME		= 0x02,
	Q_OPCODE_STOP		= 0x03,
	Q_OPCODE_LOAD		= 0x04,
	Q_OPCODE_CLEAR		= 0x05,
	Q_OPCODE_ALL_OFF	= 0x06,
	Q_OPCODE_RESTORE	= 0x07,
	Q_OPCODE_CONTROL	= 0x10,
	Q_OPCODE_TEST		= 0x20,
	Q_OPCODE_TEST_REPLY	= 0x21,
	Q_OPCODE_POLL 		= 0x30,
	Q_OPCODE_POLLREPLY 	= 0x31,
};

typedef struct{
	ASLS_ntw_iface *iface;
	ASLS_ntw_tcpClient *tcpClient;
	osThreadId connectThreadId;
	void( *onError)();
	void( *onCueStart)();
	void( *onCuePause)();
	void( *onCueResume)();
	void( *onCueStop)();
	void( *onTest)();
	void( *onPoll)();
	void( *setLocal)();
	void( *setRemote)();
}ASLS_ntw_driver;

ASLS_ntw_driver *ASLS_ntw_driver_init(uint8_t localAddr[4], uint8_t gateway[4], uint8_t netmask[4], uint8_t remoteAddr[4], uint16_t remotePort);

#endif /* ASLS_DRIVERS_INC_NETWORK_ASLS_NTW_DRIVER_H_ */
