/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include "cmsis_os.h"
#include "ms_chronometer.h"


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Resets an mschronometer instance time delta to zero
 *
 * @param ms_chronometer ms_chronometer instance to be reseted
 */
void ms_chronometer_reset(ms_chronometer_s *ms_chronometer){
	ms_chronometer->_ms_delta = osKernelGetTickCount() - 1;
}

/**
 * @brief Returns elapsed time in milliseconds since last reset
 *
 * @param ms_chronometer ms_chronometer instance to be reseted
 * @return uint32_t elapsed time in milliseconds
 */
uint32_t ms_chronometer_get_elapsed_ms(ms_chronometer_s *ms_chronometer){
	return osKernelGetTickCount() - ms_chronometer->_ms_delta;
}

#endif
