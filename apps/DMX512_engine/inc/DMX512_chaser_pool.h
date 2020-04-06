/**
 * @ingroup DMX512_engine
 * @defgroup DMX512_chaser_pool
 * Chaser pools are used to store and manage a lists of pre-configured chasers.
 * Up to 65535 DMX512 chasers may be stored for each individual DMX512 universe.
 * @{
 */

#ifndef DMX512_CHASER_POOL_H_
#define DMX512_CHASER_POOL_H_

#include <stdint.h>
#include "DMX512_chaser.h"
#include "DMX512_defs.h"


/**
 * @ingroup DMX512_chaser_pool
 * @struct DMX512_chaser_pool_s
 * @brief Defines a DMX512 chaser pool object
 */
typedef struct{
	DMX512_chaser_s *chasers;				/**< pointer to a list of chasers*/
	uint16_t chaser_count;					/**< The amount of chasers contained within the pool */
}DMX512_chaser_pool_s;


DMX512_chaser_pool_s *DMX512_chaser_pool_new(void);
DMX512_engine_err_e DMX512_chaser_pool_add(DMX512_chaser_pool_s *this, DMX512_chaser_s chaser);
DMX512_engine_err_e DMX512_chaser_pool_del(DMX512_chaser_pool_s *this, uint16_t id);
DMX512_chaser_s *DMX512_chaser_pool_get(DMX512_chaser_pool_s *this, uint16_t id);
void DMX512_chaser_pool_manage(DMX512_chaser_pool_s *this);


#endif

/**
 * @} Grouping in DMX512_chaser_pool submodule ends
 * @} Grouping in DMX512_engine module ends
 */
