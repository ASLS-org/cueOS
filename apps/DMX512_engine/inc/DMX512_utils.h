#ifndef DMX512_UTILS_H_
#define DMX512_UTILS_H_

#include <stdint.h>


/**
 * @ingroup DMX512_utils
 * @struct DMX512_utils_mschronometer_s
 * @brief Defines a DMX512 mschronometer instance
 *
 * In order to keep track of time, a chronometer capable of producing accurate millisecond count
 * is required. this mschonometer instance conveys time information using a time delta value which
 * can be set and reset throughout time. Once reset, elapsed time may be processed using the time delta
 * value as a start offset
 */
typedef struct{
	uint32_t _ms_delta;						/**< Time delta to be set using the system tick counter*/
}DMX512_utils_mschronometer_s;


void DMX512_utils_mschronometer_reset(DMX512_utils_mschronometer_s *this);
uint32_t DMX512_utils_mschronometer_get_elapsed_ms(DMX512_utils_mschronometer_s *this);

#endif
