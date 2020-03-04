#ifndef _QLSF_PARSER_H_
#define _QLSF_PARSER_H_

#include "ff.h"
#include "DMX512_fixture.h"
#include "DMX512_scene.h"
#include "qlsf_defs.h"

#define QLSF_HEADER_SIZE	 				0x03U
#define QLSF_PATCH_DATA_SIZE				0x03U
#define QLSF_SCENE_HEADER_SIZE 				0x02U
#define QLSF_FIXTURE_PRESET_HEADER_SIZE 	0x02U

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

typedef struct{
	FIL _cur_file;
	TCHAR *_cur_file_name;
	FSIZE_t _cur_file_pointer;
	FSIZE_t _patch_size;
	FSIZE_t _scene_pool_size;
	FSIZE_t _chaser_pool_size;
	uint16_t _fixture_count;
	uint16_t _scene_count;
	uint16_t _chaser_count;
}qlsf_parser_s;

void qlsf_parser_dump(void);
void test(void);
qlfs_err_e qlsf_parser_init(char *file_path);

#endif
