#ifndef CUEOS_CONFIG_H_
#define CUEOS_CONFIG_H_

#include "cueos.h"

//TODO: DOn't forget to implement new configurations here

#define cueOS_CONFIG_NODETYPE 	  cueOS_NODETYPE_SLAVE_DMX
#define cueOS_CONFIG_NET_DEFAULT  cueOS_NET_ETHERNET
#define cueOS_CONFIG_NET_USE_DHCP 0
#define cueOS_CONFIG_NET_TIMEOUT  3000
#define cueOS_CONFIG_OSC_IN_PORT  5215
#define cueOS_CONFIG_OSC_OUT_PORT  5214


#if !cueOS_CONFIG_NET_USE_DHCP
	#define cueOS_CONFIG_NET_STATIC_IP_0 192
	#define cueOS_CONFIG_NET_STATIC_IP_1 168
	#define cueOS_CONFIG_NET_STATIC_IP_2 100
	#define cueOS_CONFIG_NET_STATIC_IP_3 175

	#define cueOS_CONFIG_NET_STATIC_GW_0 192
	#define cueOS_CONFIG_NET_STATIC_GW_1 168
	#define cueOS_CONFIG_NET_STATIC_GW_2 100
	#define cueOS_CONFIG_NET_STATIC_GW_3 50

	#define cueOS_CONFIG_NET_STATIC_NM_0 255
	#define cueOS_CONFIG_NET_STATIC_NM_1 255
	#define cueOS_CONFIG_NET_STATIC_NM_2 255
	#define cueOS_CONFIG_NET_STATIC_NM_3 0
#endif


#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	#define cueOS_CONFIG_SLAVE_DMX_USE_RDM		1
	#define cueOS_CONFIG_SLAVE_DMX_MAX_SCENE	255
	#define cueOS_CONFIG_SLAVE_DMX_MAX_CHASER	255
	#define cueOS_CONFIG_SLAVE_DMX_MAX_EFX		255
	#define cueOS_CONFIG_SLAVE_DMX_MAX_RGBMX	255
	#define cueOS_CONFIG_SLAVE_DMX_MAX_CUES		255
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	#define cueOS_CONFIG_SLAVE_PYRO_TEST_ENABLE 	1
	#define cueOS_CONFIG_SLAVE_PYRO_MAX_CUES		16
#endif

#endif
