/**
 * @ingroup libs
 *
 * @defgroup ms_chronometer MS Chronometer
 * In order to keep track of time, a chronometer capable of producing accurate millisecond count
 * is required. mschonometer conveys time information using a time delta value which can be set
 * and reset throughout time. Once reset, elapsed time may be processed using the time delta value
 * as a start offset
 * @{
 *
 */

#ifndef DMX512_UTILS_H_
#define DMX512_UTILS_H_

#include <stdint.h>


/**
 * @brief ms_chronometer structure object
 */
typedef struct{
	uint32_t _ms_delta;						/**< Time delta to be set using the system tick counter*/
}ms_chronometer_s;


void ms_chronometer_reset(ms_chronometer_s *ms_chronometer);
uint32_t ms_chronometer_get_elapsed_ms(ms_chronometer_s *ms_chronometer);

#endif

/**
 * @} Grouping in ms_chronometer submodule ends
 **/
