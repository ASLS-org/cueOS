/**
 * @ingroup DMX512_engine
 * @defgroup DMX512_scene_pool Scene Pool
 * Scene pools are used to store and manage a lists of pre-configured scenes.
 * Up to 65535 DMX512 scenes may be stored per DMX512 universe.
 * @{
 */

#ifndef DMX512_SCENE_POOL_H_
#define DMX512_SCENE_POOL_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "DMX512_defs.h"
#include "DMX512_scene.h"


/**
 * @ingroup DMX512_scene_pool
 * @brief DMX512 scene pool structure object
 */
typedef struct{
	DMX512_scene_s *scenes;				/**< pointer to a list of scenes*/
	uint16_t scene_count;				/**< The amount of scenes contained within the pool*/
}DMX512_scene_pool_s;


DMX512_scene_pool_s *DMX512_scene_pool_new(void);
DMX512_engine_err_e DMX512_scene_pool_add(DMX512_scene_pool_s *scene_pool, DMX512_scene_s scene);
DMX512_engine_err_e DMX512_scene_pool_del(DMX512_scene_pool_s *scene_pool, uint16_t id);
DMX512_engine_err_e DMX512_scene_pool_get(DMX512_scene_pool_s *scene_pool, uint16_t id, DMX512_scene_s **scene);
void DMX512_scene_pool_manage(DMX512_scene_pool_s *scene_pool);
void DMX512_scene_pool_free(DMX512_scene_pool_s *scene_pool);

#endif

/**
 * @} Grouping in DMX512_scene_pool submodule ends
 */
