/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "DMX512_utils.h"
#include "cmsis_os.h"


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * Resets an mschronometer instance time delta to zero
 *
 * @param this mschronometer instance to be reseted
 */
void DMX512_utils_mschronometer_reset(DMX512_utils_mschronometer_s *this){
	this->_ms_delta = osKernelGetTickCount() - 1;
}

/**
 * Returns elapsed time in milliseconds since last reset
 *
 * @param this mschronometer instance to be reseted
 * @return uint32_t elapsed time in milliseconds
 */
uint32_t DMX512_utils_mschronometer_get_elapsed_ms(DMX512_utils_mschronometer_s *this){
	return osKernelGetTickCount() - this->_ms_delta;
}
