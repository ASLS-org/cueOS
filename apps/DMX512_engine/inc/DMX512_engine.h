#ifndef DMX512_engine_H
#define DMX512_engine_H

#include <stdint.h>
#include "fs.h"

#include "DMX512_chaser_pool.h"
#include "DMX512_defs.h"
#include "DMX512_fixture_pool.h"
#include "DMX512_scene_pool.h"

#define DMX512_ENGINE_CONFIG_HEADER_SIZE	 			 0x03U
#define DMX512_ENGINE_CONFIG_PATCH_DATA_SIZE			 0x03U
#define DMX512_ENGINE_CONFIG_SCENE_HEADER_SIZE 			 0x02U
#define DMX512_ENGINE_CONFIG_FIXTURE_PRESET_HEADER_SIZE  0x02U
#define DMX512_ENGINE_CONFIG_CHASER_HEADER_SIZE 		 0x04U

#define DMX512_ENGINE_CONFIG_FILE_EXT 		".qlsf"
#define DMX512_ENGINE_CONFIG_TEMP_FILE_EXT 	".tmp"

#define DMX512_ENGINE_THREAD_DELAY 	1


typedef enum{
	DMX512_ENGINE_CONFIG_HEADER_PATCH_CNT_INDEX,
	DMX512_ENGINE_CONFIG_HEADER_SCENE_CNT_INDEX,
	DMX512_ENGINE_CONFIG_HEADER_CHASE_CNT_INDEX,
	DMX512_ENGINE_CONFIG_HEADER_EFFEX_CNT_INDEX,
}DMX512_engine_config_header_byte_indexes_e;

typedef enum{
	DMX512_ENGINE_CONFIG_PATCH_CHUNK_ID_INDEX,
	DMX512_ENGINE_CONFIG_PATCH_CHUNK_ADDR_INDEX,
	DMX512_ENGINE_CONFIG_PATCH_CHUNK_CHN_INDEX,
}DMX512_engine_config_patch_chunk_byte_indexes_e;

typedef enum{
	DMX512_ENGINE_CONFIG_SCENE_ID_INDEX,
	DMX512_ENGINE_CONFIG_SCENE_FIXTURE_PRESET_CNT_INDEX,
}DMX512_engine_config_scene_byte_indexes_e;

typedef enum{
	DMX512_ENGINE_CONFIG_FIXTURE_PRESET_ID_INDEX,
	DMX512_ENGINE_CONFIG_FIXTURE_PRESET_CHANNEL_CNT_INDEX,
}DMX512_engine_config_fixture_preset_byte_indexes_e;

typedef enum{
	DMX512_ENGINE_CONFIG_CHASER_ID_INDEX,
	DMX512_ENGINE_CONFIG_CHASER_DIR_MODE_INDEX,
	DMX512_ENGINE_CONFIG_CHASER_RUN_MODE_INDEX,
	DMX512_ENGINE_CONFIG_CHASER_STEP_COUNT_INDEX,
}DMX512_engine_config_chaser_byte_indexes_e;

typedef enum{
	DMX512_ENGINE_CONFIG_STEP_ID_INDEX,
	DMX512_ENGINE_CONFIG_STEP_FADEIN_INDEX,
	DMX512_ENGINE_CONFIG_STEP_FADEOUT_INDEX,
	DMX512_ENGINE_CONFIG_STEP_HOLD_INDEX
}DMX512_engine_config_chaser_step_byte_indexes_e;


typedef struct{
	DMX512_fixture_pool_s *fixtures;
	DMX512_scene_pool_s *scenes;
	DMX512_chaser_pool_s *chasers;
	uint16_t fixture_count;
	uint16_t scene_count;
	uint16_t chaser_count;
	uint16_t effect_count;
}DMX512_engine_s;


void DMX512_engine_init(void);
void DMX512_engine_start(void);
void DMX512_engine_stop(void);
DMX512_engine_err_e DMX512_engine_load_config(TCHAR *config_file_path);


#endif
