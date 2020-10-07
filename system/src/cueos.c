/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "net.h"
#include "leds_driver.h"
#include "osc_server.h"
#include "web_application.h"
#include "rest_api.h"
#include "DMX512_engine.h"
#include "device.h"
#include "cueos_config.h"
#include "cueos.h"
#include "Q_client.h"
#include "keys_driver.h"


osThreadId_t cueos_thread_id = NULL;

static void test(void){
	osDelay(250);
	osc_packet_send(1,2,3,256);
}


//TODO: node type configuration here is quite a bit confusing. Clean this whole file.
static void _cueos_thread(void *arg){

	fs_init();
	net_init(NET_MODE_ETHERNET, test);
	leds_driver_init();
	rest_api_start();
	web_application_start();
	//keys_init();

#if cueOS_CONFIG_NODETYPE   == cueOS_NODETYPE_MASTER
	Q_server_init();
	webapp_static_start();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
	osc_server_init();
	DMX512_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_PYRO
	Q_client_init();
	pyro_engine_init();
#elif cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_MEDIA
	Q_client_init();
	media_engine_init();
#endif

	leds_driver_set(LED_STATE, LED_BLINK);

	for(;;){
		osDelay(500);
	}

}

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see cueos.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Initilises and starts OS services based
 * 		  on configurations defined within cueos_config file
 * 		  @see cueos_config
 */
//TODO: implement global error handler
//TODO: I'm not happy with this file design, please look into clarifying all this mess
void cueos_start(void){

	device_init();

	osThreadAttr_t attr = {.stack_size = 4096,.priority = (osPriority_t) osPriorityNormal};
	cueos_thread_id = osThreadNew(_cueos_thread, NULL, &attr);

}
