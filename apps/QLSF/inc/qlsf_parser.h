#ifndef _QLSF_PARSER_H_
#define _QLSF_PARSER_H_

#include "ff.h"
#include "qlsf_parser.h"
#include "DMX512_fixture.h"
#include "DMX512_scene.h"

#define QLSF_HEADER_BYTESIZE 		0x08U
#define QLSF_PATCH_CHUNK_BYTESIZE	0x06U

typedef enum{
	QLSF_HEADER_FIXTURECOUNT_LO_INDEX,
	QLSF_HEADER_FIXTURECOUNT_HI_INDEX,
	QLSF_HEADER_SCENECOUNT_LO_INDEX,
	QLSF_HEADER_SCENECOUNT_HI_INDEX,
	QLSF_HEADER_CHASERCOUNT_LO_INDEX,
	QLSF_HEADER_CHASERCOUNT_HI_INDEX,
}qlsf_header_byte_indexes_e;

typedef enum{
	QLSF_PATCH_CHUNK_ID_LO_INDEX,
	QLSF_PATCH_CHUNK_ID_HI_INDEX,
	QLSF_PATCH_CHUNK_ADDR_LO_INDEX,
	QLSF_PATCH_CHUNK_ADDR_HI_INDEX,
	QLSF_PATCH_CHUNK_CHN_LO_INDEX,
	QLSF_PATCH_CHUNK_CHN_HI_INDEX
}qlsf_patch_chunk_byte_indexes_e;

typedef struct{
	FIL _cur_file;
	FSIZE_t _cur_file_pointer;
	uint16_t _fixture_pool_index;
	uint16_t _scene_pool_index;
	uint16_t _chaser_pool_index;
}qlsf_parser_s;

DMX512_fixture_s qlsf_parser_load_fixture(void);
DMX512_fixture_s qlsf_parser_get_scene(uint8_t scene_id);

#endif
