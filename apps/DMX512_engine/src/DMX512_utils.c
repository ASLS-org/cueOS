/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cueos_config.h"
#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX
#include "cmsis_os.h"
#include "DMX512_utils.h"


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * @ingroup mschronometer
 * @brief Resets an mschronometer instance time delta to zero
 *
 * @param this mschronometer instance to be reseted
 */
void DMX512_utils_mschronometer_reset(DMX512_utils_mschronometer_s *mschronometer){
	mschronometer->_ms_delta = osKernelGetTickCount() - 1;
}

/**
 * @ingroup mschronometer
 * @brief Returns elapsed time in milliseconds since last reset
 *
 * @param this mschronometer instance to be reseted
 * @return uint32_t elapsed time in milliseconds
 */
uint32_t DMX512_utils_mschronometer_get_elapsed_ms(DMX512_utils_mschronometer_s *mschronometer){
	return osKernelGetTickCount() - mschronometer->_ms_delta;
}

#endif
