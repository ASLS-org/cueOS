/***============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX

#include "cueos_config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_engine.h"
#include "leds_driver.h"
#include "QLSF_manager.h"


/***============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static DMX512_engine_s engine;
static osThreadId_t DMX512engineThread = NULL;


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Thread managing engine functions execution
 *
 * Currently available DMX512 functions include:
 * Scene: a set of predefined values for one or many fixtures
 * Chaser: a chained set of scenes defined as "steps" executed sequentially
 * Effect: a set of mathematical function used as individual channel actuator for one or many fixtures
 * @param *arg pointer to argument passed during thread creation. Not used here.
 */
static void _DMX512_engine_manage(void *arg){
	for(;;){
		DMX512_scene_pool_manage(engine.scenes);
		DMX512_chaser_pool_manage(engine.chasers);
		osDelay(DMX512_ENGINE_THREAD_DELAY);
	}
}

//TODO: add engine reset to clear everything ?

/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_engine.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Initialises the DMX512 engine singleton.
 */
void DMX512_engine_init(void){

	//TODO: modify pool instances to return value instead of pointer to value since engine is a singleton
	//and should keep the values safe until it is exited

	engine.fixtures 	  = DMX512_fixture_pool_new();
	engine.scenes 		  = DMX512_scene_pool_new();
	engine.chasers 		  = DMX512_chaser_pool_new();

	QLFS_manager_load("show.qlsf");

	if(DMX512_driver_get_status() != DMX512_DRIVER_INITIALISED){
		DMX512_driver_init();
	}

	DMX512_engine_start();

}

//TODO: maybe add a "started state" to prevent recursive calls to start/stop function

/**
 * @brief Starts the DMX512 driver and launches the DMX512 engine management thread
 * @warning FATTFS
 * TODO: decrease memory usage by modifying configuration file
 */
void DMX512_engine_start(void){
	osThreadAttr_t attr = {.stack_size = 2048,.priority = (osPriority_t) osPriorityNormal};
	DMX512_driver_start();
	DMX512engineThread = osThreadNew(_DMX512_engine_manage, NULL, &attr);
}

/**
 * @brief Stops the DMX512 driver and terminates the DMX512 engine management thread
 */
void DMX512_engine_stop(void){
	DMX512_driver_stop();
	osThreadTerminate(DMX512engineThread);
}

/**
 * @brief Wrapper for "DMX512_fixture_pool_add" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param fixture_id the fixture's identifier
 * @param address fixture's first channel address
 * @param ch_count fixture's channel count
 * @return DMX512_engine_err_e error code following the function call
 * @see DMX512_defs.h for further information regarding DMX512 engin error codes
 */
DMX512_engine_err_e DMX512_engine_patch_add(uint16_t fixture_id, uint16_t address, uint16_t ch_count){
	DMX512_fixture_s fixture = DMX512_fixture_new(fixture_id, address, ch_count);
	return DMX512_fixture_pool_add(engine.fixtures, fixture);
}

/**
 * @brief Wrapper for "DMX512_fixture_pool_get" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param fixture_id the fixture's identifier
 * @param **fixture pointer to the fixture
 * @return DMX512_engine_err_e ERR_OK if fixture was found, DMX512_ENGINE_INSTANCE_UNDEFINED otherwise
 */
DMX512_engine_err_e DMX512_engine_patch_get(uint16_t fixture_id, DMX512_fixture_s **fixture){
	return DMX512_fixture_pool_get(engine.fixtures, fixture_id, fixture);
}

/**
 * @brief Returns the current engine fixture patch
 *
 * @return DMX512_fixture_pool_s* pointer to the engine's fixture pool
 */
DMX512_fixture_pool_s *DMX512_engine_patch_get_all(void){
	return engine.fixtures;
}

/**
 * @brief Wrapper for "DMX512_fixture_pool_del" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param fixture_id the fixture's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_engine_patch_delete(uint16_t fixture_id){
	return DMX512_fixture_pool_del(engine.fixtures, fixture_id);
}

/**
 * @brief Wrapper for "DMX512_scene_pool_add" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param scene_id the scene identifier
 * @param fadein_time the scene fade-in time in milliseconds
 * @param fadeout_time the scene fade-out time in milliseconds
 * @return DMX512_engine_err_e error code following the function call
 * @see DMX512_defs.h for further information regarding DMX512 engin error codes
 */
DMX512_engine_err_e DMX512_engine_scene_add(uint16_t scene_id, uint16_t fadein_time, uint16_t fadeout_time){
	DMX512_scene_s scene = DMX512_scene_new(scene_id, fadein_time, fadeout_time);
	return DMX512_scene_pool_add(engine.scenes, scene);
}

/**
 * @brief Wrapper for "DMX512_scene_pool_add" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param scene_id the scene identifier
 * @param fadein_time the scene fade-in time in milliseconds
 * @param fadeout_time the scene fade-out time in milliseconds
 * @return DMX512_engine_err_e error code following the function call
 * @see DMX512_defs.h for further information regarding DMX512 engin error codes
 */
DMX512_engine_err_e DMX512_engine_scene_add_preset(uint16_t scene_id, uint16_t fixture_id, uint16_t channel_count, uint16_t *channels, uint8_t *values){

	DMX512_scene_s *scene		= NULL;
	DMX512_fixture_s *fixture 	= NULL;

	DMX512_fixture_pool_get(engine.fixtures, fixture_id, &fixture);
	DMX512_scene_pool_get(engine.scenes, scene_id, &scene);

	DMX512_fixture_preset_s preset = DMX512_fixture_preset_new(fixture, channel_count, channels, values);

	return DMX512_scene_add_fixture_preset(scene, preset);

}

/**
 * @brief Wrapper for "DMX512_scene_pool_get" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "scene" as argument.
 *
 * @param scene_id the scene identifier
 * @param **scene pointer to the scene
 * @return DMX512_engine_err_e ERR_OK if fixture was found, DMX512_ENGINE_INSTANCE_UNDEFINED otherwise
 */
DMX512_engine_err_e DMX512_engine_scene_get(uint16_t scene_id, DMX512_scene_s **scene){
	return DMX512_scene_pool_get(engine.scenes, scene_id, scene);
}

/**
 * @brief Returns the current engine scenes
 *
 * @return DMX512_scene_pool_s* pointer to the engine's scene pool
 */
DMX512_scene_pool_s *DMX512_engine_scene_get_all(void){
	return engine.scenes;
}

/**
 * @brief Wrapper for "DMX512_scene_pool_del" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "scenes" as argument.
 *
 * @param scene_id the scene's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_engine_scene_delete(uint16_t scene_id){
	return DMX512_fixture_pool_del(engine.fixtures, scene_id);
}

/**
 * @brief Wrapper for "DMX512_chaser_pool_add" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param chaser_id the chaser identifier (@see DMX512_chaser_pool.h)
 * @param mode the chaser trigger mode
 * @param direction the chaser step play direction
 * @return DMX512_engine_err_e error code following the function call
 * @see DMX512_defs.h for further information regarding DMX512 engin error codes
 */
DMX512_engine_err_e DMX512_engine_chaser_add(uint16_t chaser_id, DMX512_chaser_mode_e mode, DMX512_chaser_direction_e direction){
	DMX512_chaser_s chaser = DMX512_chaser_new(chaser_id, mode, direction);
	return DMX512_chaser_pool_add(engine.chasers, chaser);
}

/**
 * @brief Wrapper for "DMX512_chaser_pool_get" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "chaser" as argument.
 *
 * @param chaser_id the chaser identifier
 * @param **chaser pointer to the chaser
 * @return DMX512_engine_err_e ERR_OK if fixture was found, DMX512_ENGINE_INSTANCE_UNDEFINED otherwise
 */
DMX512_engine_err_e DMX512_engine_chaser_get(uint16_t chaser_id, DMX512_chaser_s **chaser){
	return DMX512_chaser_pool_get(engine.chasers, chaser_id, chaser);
}

/**
 * @brief Returns the current engine chasers
 *
 * @return DMX512_chaser_pool_s* pointer to the engine's chaser pool
 */
DMX512_chaser_pool_s *DMX512_engine_chaser_get_all(void){
	return engine.chasers;
}

/**
 * @brief Wrapper for "DMX512_chaser_pool_del" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "chasers" as argument.
 *
 * @param chaser_id the chaser's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_engine_chaser_delete(uint16_t chaser_id){
	return DMX512_fixture_pool_del(engine.fixtures, chaser_id);
}

/**
 * @briefs Frees dynamically allocated memory pools and re-initialises the engine
 */
void DMX512_engine_reset(void){

	DMX512_engine_stop();

	DMX512_chaser_pool_free(engine.chasers);
	DMX512_scene_pool_free(engine.scenes);
	DMX512_fixture_pool_free(engine.fixtures);

	engine.fixtures 	  = DMX512_fixture_pool_new();
	engine.scenes 		  = DMX512_scene_pool_new();
	engine.chasers 		  = DMX512_chaser_pool_new();

	DMX512_engine_start();

}

#endif
