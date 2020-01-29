/*
 * ASLS_ntw_driver.c
 *
 *  Created on: 25 Nov 2019
 *      Author: root
 */

#include "ASLS_ntw_interface.h"
#include "ASLS_ntw_tcpClient.h"
#include "ASLS_ntw_driver.h"
#include <string.h>
#include "cmsis_os.h"
#include "ASLS_ntw_driver.h"

static const char *Q_PACKETID_STRING = "ASLS-Q";
static ASLS_ntw_driver *this;
osThreadId connectThreadId;

//GETTERS AND SETTERS DEFINITION
static void _ASLS_ntw_driver_setLocal(uint8_t localAddr[4]);
static void _ASLS_ntw_driver_setRemote(uint8_t remoteAddr[4], uint16_t remotePort);

//PRIVATE METHODS DEFINITION
static void _ASLS_ntw_driver_handleServerConnection(void const *arg);
static void _ASLS_ntw_driver_handleData(void *payload, uint16_t len);

ASLS_ntw_driver *ASLS_ntw_driver_init(uint8_t localAddr[4], uint8_t gateway[4], uint8_t netmask[4], uint8_t remoteAddr[4], uint16_t remotePort){

	this = malloc(sizeof(ASLS_ntw_driver));

	this->setLocal 		= _ASLS_ntw_driver_setLocal;
	this->setRemote 	= _ASLS_ntw_driver_setRemote;

	this->connectThreadId = NULL;

	this->iface = ASLS_ntw_iface_init(localAddr, netmask, gateway);
	this->tcpClient = ASLS_ntw_tcpClient_init(remoteAddr, remotePort);
	this->tcpClient->setRecvCallback(_ASLS_ntw_driver_handleData);

	osThreadDef(ConnectThr, _ASLS_ntw_driver_handleServerConnection, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	connectThreadId = osThreadCreate (osThread(ConnectThr), NULL);

	return this;

}

static void _ASLS_ntw_driver_setLocal(uint8_t localAddr[4]){
	this->iface->setAddr(localAddr);
}

static void _ASLS_ntw_driver_setRemote(uint8_t remoteAddr[4], uint16_t remotePort){
	this->iface->down();
	this->tcpClient->setRemoteAddr(remoteAddr);
	this->tcpClient->setRemotePort(remotePort);
	this->iface->up();
}

static void _ASLS_ntw_driver_handleServerConnection(void const *arg){
	for(;;){
		ASLS_ntw_tcpClient_State tcpClientState = this->tcpClient->getState();
		ASLS_ntw_iface_state ifaceState = this->iface->getState();
		if(ifaceState == IFACE_UP){
			if(tcpClientState == TCP_CLIENT_DISCONNECTED){
				this->tcpClient->connect();
			}
		}else if(ifaceState == IFACE_DOWN){
			if(tcpClientState != TCP_CLIENT_DISCONNECTED){
				this->tcpClient->reset();
			}
		}
		osDelay(100);
	}
}

static void _ASLS_ntw_driver_handleData(void *payload, uint16_t len){

		char packet[len];
		char data[len - Q_PAYLOAD_INDEX];

		memcpy(packet, payload, len);
		memcpy(data, payload + Q_PAYLOAD_INDEX, len - Q_PAYLOAD_INDEX);

		if(strncmp(packet, Q_PACKETID_STRING, Q_PACKETID_LENGTH) == 0){
			//err = PARSER_ERR_INVALID_OPCODE;
			switch(packet[Q_OPCODE_INDEX]){
				case Q_OPCODE_START: 	ASLS_network_onStart(data);		break;
				case Q_OPCODE_PAUSE:	ASLS_network_onPause(data);		break;
				case Q_OPCODE_RESUME:	ASLS_network_onResume(data);	break;
				case Q_OPCODE_STOP:		ASLS_network_onStop(data);		break;
//				case Q_OPCODE_LOAD:		ASLS_network_onLoad(data);		break;
//				case Q_OPCODE_CLEAR:	ASLS_network_onClear(data);		break;
//				case Q_OPCODE_ALL_OFF:	ASLS_network_onAllOff(data);	break;
//				case Q_OPCODE_RESTORE:	ASLS_network_onRestore(data);	break;
//				case Q_OPCODE_CONTROL:	ASLS_network_onControl(data);	break;
//				case Q_OPCODE_TEST:		ASLS_network_onTest(data);		break;
				case Q_OPCODE_POLL:		ASLS_network_onPoll(data);		break;
			}
		}

}

__weak void ASLS_network_onStart(char *data){
	HAL_GPIO_WritePin(LED_SUCCESS_GPIO_Port, LED_SUCCESS_Pin, 1);
}

__weak void ASLS_network_onPause(char *data){
	//TO BE OVERRIDE BY USER
}

__weak void ASLS_network_onResume(char *data){
	//TO BE OVERRIDE BY USER
}

__weak void ASLS_network_onStop(char *data){
	HAL_GPIO_WritePin(LED_SUCCESS_GPIO_Port, LED_SUCCESS_Pin, 0);
}

__weak void ASLS_network_onPoll(char *data){
	char pollReplyPacket[Q_PACKETID_LENGTH + Q_OPCODE_LENGTH + Q_PACKET_TERMBYTE];
	sprintf(pollReplyPacket, "%s%c", Q_PACKETID_STRING, Q_OPCODE_POLLREPLY);
	this->tcpClient->send(pollReplyPacket, Q_PACKETID_LENGTH + Q_OPCODE_LENGTH + Q_PACKET_TERMBYTE);
}
