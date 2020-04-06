/**
 * @defgroup apps
 * Higher layer applications
 * @{
 *
 * @defgroup DMX512_engine
 * The DMX512 engine is in charge of managing the whole DMX show creation and show playback process.
 * To do so, it wraps the functionalities described throughout the apps' submodules into higher level
 * functions and data structures to provide users with an easy and intuitive way to create and manage
 * DMX512 show instances.
 * @{
 */

#ifndef DMX512_engine_H
#define DMX512_engine_H

#include <stdint.h>
#include "fs.h"
#include "DMX512_chaser_pool.h"
#include "DMX512_defs.h"
#include "DMX512_fixture_pool.h"
#include "DMX512_scene_pool.h"


//TODO: move these in a separate file, Project manager for example ?


#define DMX512_ENGINE_CONFIG_HEADER_SIZE	 			 0x03U 		//size of a QLSF header
#define DMX512_ENGINE_CONFIG_PATCH_DATA_SIZE			 0x03U		//DMX512 byte size of a QLSF patch header
#define DMX512_ENGINE_CONFIG_SCENE_HEADER_SIZE 			 0x02U		//byte size of a QLSF scene header
#define DMX512_ENGINE_CONFIG_FIXTURE_PRESET_HEADER_SIZE  0x02U		//byte size of a QLSF fixture preset headers
#define DMX512_ENGINE_CONFIG_CHASER_HEADER_SIZE 		 0x04U		//byte size of a QLSF chaser header
#define DMX512_ENGINE_CONFIG_FILE_EXT 					 ".qlsf"	//QLSF file extension
#define DMX512_ENGINE_CONFIG_TEMP_FILE_EXT 				 ".tmp"		//temporary file extension*
#define DMX512_ENGINE_THREAD_DELAY 						 30			//engine thread execution delay


/**
 * @brief QLSF header byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF file.
 */
typedef enum{

	DMX512_ENGINE_CONFIG_HEADER_PATCH_CNT_INDEX,					/**< Index of the patch count byte contained in a QLSF header*/
	DMX512_ENGINE_CONFIG_HEADER_SCENE_CNT_INDEX,					/**< Index of the scene count byte contained in a QLSF header*/
	DMX512_ENGINE_CONFIG_HEADER_CHASE_CNT_INDEX,					/**< Index of the chaser count byte contained in a QLSF header*/
	DMX512_ENGINE_CONFIG_HEADER_EFFEX_CNT_INDEX,					/**< Index of the effect count byte contained in a QLSF header*/
}DMX512_engine_config_header_byte_indexes_e;

/**
 * @brief QLSF patch chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF patch chunk.
 */
typedef enum{
	DMX512_ENGINE_CONFIG_PATCH_CHUNK_ID_INDEX,						/**< Index of a patch chunk "id" byte*/
	DMX512_ENGINE_CONFIG_PATCH_CHUNK_ADDR_INDEX,					/**< Index of a patch chunk "address" byte*/
	DMX512_ENGINE_CONFIG_PATCH_CHUNK_CHN_INDEX,						/**< Index of a patch chunk "channel count" byte*/
}DMX512_engine_config_patch_chunk_byte_indexes_e;

/**
 * @brief  QLSF scene chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF scene chunk.
 */
typedef enum{
	DMX512_ENGINE_CONFIG_SCENE_ID_INDEX,							/**< Index of a scene chunk "id" byte*/
	DMX512_ENGINE_CONFIG_SCENE_FIXTURE_PRESET_CNT_INDEX,			/**< Index of a scene chunk "fixture preset count" byte*/
}DMX512_engine_config_scene_byte_indexes_e;

/**
 * @brief  QLSF fixture preset chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF fixture preset chunk.
 */
typedef enum{
	DMX512_ENGINE_CONFIG_FIXTURE_PRESET_ID_INDEX,					/**< Index of a fixture preset chunk "fixture id" byte*/
	DMX512_ENGINE_CONFIG_FIXTURE_PRESET_CHANNEL_CNT_INDEX,			/**< Index of a fixture preset chunk "channel count" byte*/
}DMX512_engine_config_fixture_preset_byte_indexes_e;

/**
 * @brief QLSF chaser chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF chaser chunk.
 */
typedef enum{
	DMX512_ENGINE_CONFIG_CHASER_ID_INDEX,							/**< Index of a chaser chunk "chaser id" byte*/
	DMX512_ENGINE_CONFIG_CHASER_DIR_MODE_INDEX,						/**< Index of a chaser chunk "run direction" byte*/
	DMX512_ENGINE_CONFIG_CHASER_RUN_MODE_INDEX,						/**< Index of a chaser chunk "run mode" byte*/
	DMX512_ENGINE_CONFIG_CHASER_STEP_COUNT_INDEX,					/**< Index of a chaser chunk "step count" byte*/
}DMX512_engine_config_chaser_byte_indexes_e;

/**
 * @brief QLSF chaser step chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF chaser step.
 */
typedef enum{
	DMX512_ENGINE_CONFIG_STEP_ID_INDEX,								/**< Index of a chaser step chunk "chaser step id" byte*/
	DMX512_ENGINE_CONFIG_STEP_FADEIN_INDEX,							/**< Index of a chaser step chunk "fade in time" byte*/
	DMX512_ENGINE_CONFIG_STEP_FADEOUT_INDEX,						/**< Index of a chaser step chunk "fade out time" byte*/
	DMX512_ENGINE_CONFIG_STEP_HOLD_INDEX							/**< Index of a chaser step chunk "hold time" byte*/
}DMX512_engine_config_chaser_step_byte_indexes_e;


/**
 * @brief Defines a DMX512 engine instance
 */
typedef struct{
	DMX512_fixture_pool_s *fixtures;				/**< list of the engine's fixtures (patch)*/
	DMX512_scene_pool_s *scenes;					/**< list of the engine's scenes*/
	DMX512_chaser_pool_s *chasers;					/**< list of the engine's chasers*/
	uint16_t fixture_count;							/**< Expected fixture count extracted from a QLSF header*/ 	//TODO: maybe remove this, it is confusing
	uint16_t scene_count;							/**< Expected scene count extracted from a QLSF header*/ 	//TODO: maybe remove this, it is confusing
	uint16_t chaser_count;							/**< Expected chaser count extracted from a QLSF header*/ 	//TODO: maybe remove this, it is confusing
	uint16_t effect_count;							/**< Expected effect count extracted from a QLSF header*/ 	//TODO: maybe remove this, it is confusing
}DMX512_engine_s;


void DMX512_engine_init(void);
void DMX512_engine_start(void);
void DMX512_engine_stop(void);
DMX512_engine_err_e DMX512_engine_patch_add(uint16_t fixture_id, uint16_t address, uint16_t ch_count);
DMX512_engine_err_e DMX512_engine_patch_get(uint16_t fixture_id, DMX512_fixture_s **fixture);
DMX512_fixture_pool_s *DMX512_engine_patch_get_all(void);
DMX512_engine_err_e DMX512_engine_patch_delete(uint16_t fixture_id);

#endif

/**
 * @} Grouping in DMX512_engine module ends
 * @} Grouping in apps module ends
 **/
