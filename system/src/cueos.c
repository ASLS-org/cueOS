/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "net.h"
#include "leds_driver.h"
#include "Q_client.h"
#include "web_application.h"
#include "rest_api.h"
#include "DMX512_engine.h"
#include "device.h"
#include "cueos_config.h"
#include "cueos.h"

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see cueos.h for declarations
 *=============================================================================================================================*/

/**
 * @ingroup system
 * @fn cueos_init
 * @brief initilises OS
 */
//TODO: implement global error handler
void cueos_init(void){

	osKernelInitialize();

	device_init();

	leds_driver_init();
	net_init(NET_MODE_ETHERNET, Q_client_bind);
	rest_api_start();

#if cueOS_CONFIG_NODETYPE   == cueOS_NODETYPE_MASTER
	Q_server_init();
	webapp_static_start();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	Q_client_init();
	DMX512_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	Q_client_init();
	pyro_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_MEDIA
	Q_client_init();
	media_engine_init();
#endif

	leds_driver_set(LED_STATE, LED_BLINK);

	osKernelStart();

}
