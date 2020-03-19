#ifndef DMX512_engine_H
#define DMX512_engine_H

#include <stdint.h>
#include "fs.h"
#include "DMX512_defs.h"
#include "DMX512_fixture_pool.h"
#include "DMX512_scene_pool.h"
#include "DMX512_chaser_pool.h"

#define QLSF_HEADER_SIZE	 				0x03U
#define QLSF_PATCH_DATA_SIZE				0x03U
#define QLSF_SCENE_HEADER_SIZE 				0x02U
#define QLSF_FIXTURE_PRESET_HEADER_SIZE 	0x02U
#define QLSF_CHASER_HEADER_SIZE 			0x04U

#define QLSF_TEMP_FILE_EXT 					".tmp"

typedef enum{
	QLSF_HEADER_PATCH_CNT_INDEX,
	QLSF_HEADER_SCENE_CNT_INDEX,
	QLSF_HEADER_CHASE_CNT_INDEX,
	QLSF_HEADER_EFFEX_CNT_INDEX,
}qlsf_header_byte_indexes_e;

typedef enum{
	QLSF_PATCH_CHUNK_ID_INDEX,
	QLSF_PATCH_CHUNK_ADDR_INDEX,
	QLSF_PATCH_CHUNK_CHN_INDEX,
}qlsf_patch_chunk_byte_indexes_e;

typedef enum{
	QLSF_SCENE_ID_INDEX,
	QLSF_SCENE_FIXTURE_PRESET_CNT_INDEX,
}qlsf_scene_byte_indexes_e;

typedef enum{
	QLSF_FIXTURE_PRESET_ID_INDEX,
	QLSF_FIXTURE_PRESET_CHANNEL_CNT_INDEX,
}qlsf_fixture_preset_byte_indexes_e;

typedef enum{
	QLSF_CHASER_ID_INDEX,
	QLSF_CHASER_RUN_MODE_INDEX,
	QLSF_CHASER_DIR_MODE_INDEX,
	QLSF_CHASER_STEP_COUNT_INDEX,
}qlsf_chaser_byte_indexes_e;

typedef enum{
	QLSF_STEP_ID_INDEX,
	QLSF_STEP_FADEIN_INDEX,
	QLSF_STEP_FADEOUT_INDEX,
	QLSF_STEP_HOLD_INDEX
}qlsf_chaser_step_byte_indexes_e;

typedef struct{
	DMX512_fixture_pool_s *fixtures;
	DMX512_scene_pool_s *scenes;
	DMX512_chaser_pool_s *chasers;
	uint16_t fixture_count;
	uint16_t scene_count;
	uint16_t chaser_count;
	uint16_t effect_count;
}DMX512_engine_s;

void DMX512_engine_init(TCHAR *config_file_name);
DMX512_engine_err_e DMX512_engine_load_config(TCHAR *config_file_path);

#endif
