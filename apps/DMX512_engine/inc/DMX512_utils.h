#ifndef DMX512_UTILS_H_
#define DMX512_UTILS_H_

#include <stdint.h>


typedef struct{
	uint32_t _ms_delta;
}DMX512_utils_mschronometer_s;


void DMX512_utils_mschronometer_reset(DMX512_utils_mschronometer_s *this);
uint32_t DMX512_utils_mschronometer_get_elapsed_ms(DMX512_utils_mschronometer_s *this);

#endif
