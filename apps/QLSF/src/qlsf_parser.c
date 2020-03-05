/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "fs.h"
#include "qlsf_parser.h"
#include "qlsf_defs.h"
#include "DMX512_fixture_pool.h"
#include "DMX512_fixture_preset.h"
#include "DMX512_scene_pool.h"
#include "DMX512_chaser.h"
#include "DMX512_chaser_step.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static qlsf_parser_s this;
DMX512_chaser_s mychaser;

/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

void qlsf_parser_dump(void){

	uint8_t tmp_path_sz = strlen(this._cur_file_name) + strlen(QLSF_TEMP_FILE_EXT);
	TCHAR tmp_path[tmp_path_sz];
	FIL tmp_file;

	sprintf(tmp_path, "%s%s", this._cur_file_name, QLSF_TEMP_FILE_EXT);

	f_unlink(tmp_path);

	if(f_open(&tmp_file, tmp_path, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK){

		DMX512_fixture_s *fixtures = DMX512_fixture_pool_get_all();
		uint16_t fixture_count = DMX512_fixture_pool_get_size();
		uint16_t patch_buf[QLSF_PATCH_DATA_SIZE];

		for(uint16_t i=0; i<fixture_count; i++){
			UINT btr;
			patch_buf[QLSF_PATCH_CHUNK_ID_INDEX]   = fixtures[i].id;
			patch_buf[QLSF_PATCH_CHUNK_ADDR_INDEX] = fixtures[i].addr;
			patch_buf[QLSF_PATCH_CHUNK_CHN_INDEX]  = fixtures[i].ch_count;
			f_write(&tmp_file, patch_buf, QLSF_PATCH_DATA_SIZE * sizeof(uint16_t), &btr);
		}
		f_close(&tmp_file);
		f_unlink(this._cur_file_name);
		f_rename(tmp_path, this._cur_file_name);

	}

}


/**
 * Loads header information into the parser instance
 *
 * Data contained within a QLSF file header conveys information regarding
 * the size of each groups of data chunks, thus enabling a parser to lookup
 * for show-relative information fast.
 * @return qlfs_err_e QLFS_OK on success, specific error code otherwise
 * @see qlsf_defs.h for further information regarding error codes
 */
static qlfs_err_e _qlsf_parser_load_header(void){

	qlfs_err_e err = QLFS_OK;
	uint16_t buf[QLSF_HEADER_SIZE];
	UINT br;

	f_lseek(&this._cur_file, 0);

	if(f_read(&this._cur_file, buf, QLSF_HEADER_SIZE * sizeof(uint16_t), &br) == FR_OK){
		this._fixture_count	= buf[QLSF_HEADER_PATCH_CNT_INDEX];
		this._scene_count 	= buf[QLSF_HEADER_SCENE_CNT_INDEX];
		this._chaser_count 	= buf[QLSF_HEADER_CHASE_CNT_INDEX];
	}else{
		err = 1;
	}

	return err;

}


/**
 * Loads patch defined within the config file into the DMX51 engine
 *
 * Patch information is conveyed into 6 bytes data chunk:
 * @return qlfs_err_e QLFS_OK on success, specific error code otherwise
 * @see qlsf_defs.h for further information regarding error codes
 */
static qlfs_err_e _qlsf_parser_load_patch(void){

	qlfs_err_e err = QLFS_OK;

	for(int i=0; i< this._fixture_count; i++){

		uint16_t patch_buf[QLSF_PATCH_DATA_SIZE];
		UINT br;

		if(f_read(&this._cur_file, patch_buf, QLSF_PATCH_DATA_SIZE * sizeof(uint16_t), &br) != FR_OK){
			err = QLFS_PATCH_DEF_ERR; //TODO: rename err enumeration
		}else if(br < QLSF_PATCH_DATA_SIZE){
			err = QLFS_PATCH_DEF_ERR; //TODO: rename err enumeration
		}else{
			uint16_t id   = patch_buf[QLSF_PATCH_CHUNK_ID_INDEX];
			uint16_t addr = patch_buf[QLSF_PATCH_CHUNK_ADDR_INDEX];
			uint16_t chn  = patch_buf[QLSF_PATCH_CHUNK_CHN_INDEX];
			DMX512_fixture_pool_add(id, addr, chn);	//TODO: try and define universal error enumeration values for better error arsing
		}

	}

	return err;

}

/**
 * Loads scenes defined within the config file into the DMX512 engine
 *
 * @return qlfs_err_e QLFS_OK on success, specific error code otherwise
 * @see qlsf_defs.h for further information regarding error codes
 */
static qlfs_err_e _qlsf_parser_load_scenes(void){

	qlfs_err_e err = QLFS_OK;

	for(uint16_t i=0; i<this._scene_count; i++){

		uint16_t header[QLSF_SCENE_HEADER_SIZE];

		f_read(&this._cur_file, header, QLSF_SCENE_HEADER_SIZE * sizeof(uint16_t), NULL);

		uint16_t scene_id 	  = header[QLSF_SCENE_ID_INDEX];
		uint16_t preset_count = header[QLSF_SCENE_FIXTURE_PRESET_CNT_INDEX];

		DMX512_scene_s scene = DMX512_scene_init(scene_id, 0, 0);

		for(uint16_t j=0; j<preset_count;j++){

			uint16_t preset_header[QLSF_FIXTURE_PRESET_HEADER_SIZE];

			f_read(&this._cur_file, preset_header, QLSF_FIXTURE_PRESET_HEADER_SIZE * sizeof(uint16_t), NULL);

			uint16_t fixture_id = preset_header[QLSF_FIXTURE_PRESET_ID_INDEX];
			uint16_t ch_count 	= preset_header[QLSF_FIXTURE_PRESET_CHANNEL_CNT_INDEX];
			uint16_t channels[ch_count];
			uint8_t values[ch_count];

			f_read(&this._cur_file, channels, ch_count * sizeof(uint16_t), NULL);
			f_read(&this._cur_file, values, ch_count * sizeof(uint8_t), NULL);

			DMX512_fixture_s *fixture 		= DMX512_fixture_pool_get(fixture_id);
			DMX512_fixture_preset_s preset 	= DMX512_fixture_preset_init(fixture, ch_count, channels, values);

			if(DMX512_scene_add(&scene, preset) != DMX512_ENGINE_OK){
				//TODO: report file parsing error, clear pools and report error to user
			}

		}
		DMX512_scene_pool_add(scene);
	}

	return err;
}

static qlfs_err_e _qlsf_parser_load_chasers(void){

	qlfs_err_e err = QLFS_OK;

	for(uint16_t i=0; i<this._chaser_count; i++){

		uint16_t chaser_header[QLSF_CHASER_HEADER_SIZE];

		f_read(&this._cur_file, chaser_header, QLSF_CHASER_HEADER_SIZE * sizeof(uint16_t), NULL);

		uint16_t chaser_id   = chaser_header[QLSF_CHASER_ID_INDEX];
		uint16_t chaser_mode = chaser_header[QLSF_CHASER_RUN_MODE_INDEX];
		uint16_t chaser_dir  = chaser_header[QLSF_CHASER_DIR_MODE_INDEX];
		uint16_t step_count  = chaser_header[QLSF_CHASER_STEP_COUNT_INDEX];

		mychaser = DMX512_chaser_init(chaser_id, chaser_mode, chaser_dir);

		for(uint16_t j=0; j<step_count; j++){

			uint16_t step_buffer[4];

			f_read(&this._cur_file, step_buffer, 4 * sizeof(uint16_t), NULL);

			uint16_t id 	 = step_buffer[QLSF_STEP_ID_INDEX];
			uint16_t fadein  = step_buffer[QLSF_STEP_FADEIN_INDEX];
			uint16_t fadeout = step_buffer[QLSF_STEP_FADEOUT_INDEX];
			uint16_t hold  	 = step_buffer[QLSF_STEP_HOLD_INDEX];

			DMX512_scene_s *scene = DMX512_scene_pool_get(id);
			DMX512_chaser_step_s step = DMX512_chaser_step_init(scene, fadein, fadeout, hold);

			DMX512_chaser_add(&mychaser, step);

		}

		//TODO: add to chaser pool
	}


	return err;

}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see qlsf_parser.h for declarations
 *=============================================================================================================================*/

/**
 * Initialises the "Q Lightweight Show File" parser
 *
 * @return qlfs_err_e QLFS_OK on success, specific error code otherwise
 * @see qlsf_defs.h for further information regarding error codes
 */
qlfs_err_e qlsf_parser_init(char *file_path){

	qlfs_err_e err = QLFS_OK;

	if(fs_init() != FS_OK){
		err = QLFS_FILE_SYSTEM_ERR;
	}else if(f_open(&this._cur_file, file_path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK){
		err = QLFS_FILE_NOT_FOUND;
	}else if(_qlsf_parser_load_header()){

	}else if(_qlsf_parser_load_patch() != QLFS_OK){
		err = QLFS_PATCH_DEF_ERR;
	}else if(_qlsf_parser_load_scenes() != QLFS_OK){
		err = QLFS_SCENE_DEF_ERR;
	}else if(_qlsf_parser_load_chasers() != QLFS_OK){
		err = QLFS_CHASER_DEF_ERR;
	}else{
		this._cur_file_name = file_path;
	}

	return err;

}
