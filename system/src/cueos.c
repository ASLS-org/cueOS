/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "stm32f4xx_hal.h"
#include "net.h"
#include "leds_driver.h"
#include "Q_UDP_client.h"
#include "DMX512_engine.h"
#include "device.h"
#include "cueos_config.h"
#include "cueos.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see cueos.h for declarations
 *=============================================================================================================================*/
//TODO: implement global error handler
void cueos_init(void){

	device_init();

	leds_driver_init();
	net_init(NET_MODE_ETHERNET, Q_client_bind);
	Q_client_init();

#if cueOS_CONFIG_NODETYPE   == cueOS_NODETYPE_MASTER
	master_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	DMX512_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	pyro_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_MEDIA
	media_engine_init();
#endif

	leds_driver_set(LED_STATE, LED_ON);

}
