/**
 * @ingroup DMX512_engine
 * @defgroup DMX512_utils
 * Code snippets for non DMX512 related tools and helpers
 * used throughout the DMX512 app.
 * @{
 *
 * @defgroup mschronometer
 * In order to keep track of time, a chronometer capable of producing accurate millisecond count
 * is required. mschonometer conveys time information using a time delta value which can be set
 * and reset throughout time. Once reset, elapsed time may be processed using the time delta value
 * as a start offset
 *
 */

#ifndef DMX512_UTILS_H_
#define DMX512_UTILS_H_

#include <stdint.h>


/**
 * @brief mschronometer structure object
 */
typedef struct{
	uint32_t _ms_delta;						/**< Time delta to be set using the system tick counter*/
}DMX512_utils_mschronometer_s;


void DMX512_utils_mschronometer_reset(DMX512_utils_mschronometer_s *this);
uint32_t DMX512_utils_mschronometer_get_elapsed_ms(DMX512_utils_mschronometer_s *this);

#endif

/**
 * @} Grouping in DMX512_utils submodule ends
 * @} Grouping in DMX512_engine module ends
 **/
