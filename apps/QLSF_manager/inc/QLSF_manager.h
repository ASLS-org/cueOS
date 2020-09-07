/**
 * @ingroup cueos_apps
 * @defgroup QLSF_manager QLSF Manager
 * Q Lightweight Show File load/dump utility to store/restore a show configuration
 * from file system.
 * @see fs
 * @{
 */

#ifndef QLSF_MANAGER_H_
#define QLSF_MANAGER_H_

#include <stdint.h>

#define QLFS_HEADER_SIZE	 			 0x03U 		/**< size of a QLSF header (uint16 buffer)*/
#define QLFS_PATCH_DATA_SIZE			 0x03U		/**< size of a QLSF patch header (uint16 buffer)*/
#define QLFS_SCENE_HEADER_SIZE 			 0x04U		/**< size of a QLSF scene header (uint16 buffer)*/
#define QLFS_FIXTURE_PRESET_HEADER_SIZE  0x02U		/**< size of a QLSF fixture preset header (uint16 buffer*/
#define QLFS_CHASER_HEADER_SIZE 		 0x04U		/**< size of a QLSF chaser header (uint16 buffer)*/
#define QLFS_FILE_EXT 					 ".qlsf"	/**< QLSF file extension*/
#define QLFS_TEMP_FILE_EXT 				 ".tmp"		/**< temporary file extension*/


/**
 * @brief QLSF header byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF file.
 */
typedef enum{

	QLFS_HEADER_PATCH_CNT_INDEX,					/**< Index of the patch count byte contained in a QLSF header*/
	QLFS_HEADER_SCENE_CNT_INDEX,					/**< Index of the scene count byte contained in a QLSF header*/
	QLFS_HEADER_CHASE_CNT_INDEX,					/**< Index of the chaser count byte contained in a QLSF header*/
	QLFS_HEADER_EFFEX_CNT_INDEX,					/**< Index of the effect count byte contained in a QLSF header*/
}QLFS_header_byte_indexes_e;

/**
 * @brief QLSF patch chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF patch chunk.
 */
typedef enum{
	QLFS_PATCH_CHUNK_ID_INDEX,						/**< Index of a patch chunk "id" byte*/
	QLFS_PATCH_CHUNK_ADDR_INDEX,					/**< Index of a patch chunk "address" byte*/
	QLFS_PATCH_CHUNK_CHN_INDEX,						/**< Index of a patch chunk "channel count" byte*/
}QLFS_patch_chunk_byte_indexes_e;

/**
 * @brief  QLSF scene chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF scene chunk.
 */
typedef enum{
	QLFS_SCENE_ID_INDEX,							/**< Index of a scene chunk "id" byte*/
	QLFS_SCENE_FIXTURE_PRESET_CNT_INDEX,			/**< Index of a scene chunk "fixture preset count" byte*/
	QLFS_SCENE_FADEIN_TIME_INDEX,					/**< Index of a scene chunk "fade-in time" byte*/
	QLFS_SCENE_FADEOUT_TIME_INDEX					/**< Index of a scene chunk "fade-out time" byte*/
}QLFS_scene_byte_indexes_e;

/**
 * @brief  QLSF fixture preset chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF fixture preset chunk.
 */
typedef enum{
	QLFS_FIXTURE_PRESET_ID_INDEX,					/**< Index of a fixture preset chunk "fixture id" byte*/
	QLFS_FIXTURE_PRESET_CHANNEL_CNT_INDEX,			/**< Index of a fixture preset chunk "channel count" byte*/
}QLFS_fixture_preset_byte_indexes_e;

/**
 * @brief QLSF chaser chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF chaser chunk.
 */
typedef enum{
	QLFS_CHASER_ID_INDEX,							/**< Index of a chaser chunk "chaser id" byte*/
	QLFS_CHASER_DIR_MODE_INDEX,						/**< Index of a chaser chunk "run direction" byte*/
	QLFS_CHASER_RUN_MODE_INDEX,						/**< Index of a chaser chunk "run mode" byte*/
	QLFS_CHASER_STEP_COUNT_INDEX,					/**< Index of a chaser chunk "step count" byte*/
}QLFS_chaser_byte_indexes_e;

/**
 * @brief QLSF chaser step chunk byte indexes
 *
 * Series of pre-defined constant byte index values to be used
 * during parsing of a QLSF chaser step.
 */
typedef enum{
	QLFS_STEP_ID_INDEX,								/**< Index of a chaser step chunk "chaser step id" byte*/
	QLFS_STEP_FADEIN_INDEX,							/**< Index of a chaser step chunk "fade in time" byte*/
	QLFS_STEP_FADEOUT_INDEX,						/**< Index of a chaser step chunk "fade out time" byte*/
	QLFS_STEP_HOLD_INDEX							/**< Index of a chaser step chunk "hold time" byte*/
}QLFS_chaser_step_byte_indexes_e;


/**
 * @brief QLFS_manager structure object
 */
typedef struct{
	uint16_t fixture_count;							/**< Expected fixture count extracted from a QLSF header*/
		uint16_t scene_count;						/**< Expected scene count extracted from a QLSF header*/
		uint16_t chaser_count;						/**< Expected chaser count extracted from a QLSF header*/
		uint16_t effect_count;						/**< Expected effect count extracted from a QLSF header*/
}QLFS_manager_s;

#endif

/**
 * @} QLSF_manager grouping ends here
 */
