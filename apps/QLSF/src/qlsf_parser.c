///**============================================================================================================================
// * Depedencies inclusion
// * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
// *=============================================================================================================================*/
//
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include "fs.h"
//#include "DMX512_fixture_pool.h"
//#include "DMX512_scene_pool.h"
//#include "DMX512_chaser.h"
//#include "qlsf_parser.h"
//
//
//
///**============================================================================================================================
// * Private functions definitions
// * These functions are only accessible from within the file's scope
// *=============================================================================================================================*/
//
////DMX512_engine_err_e _DMX512_qlsf_dump_patch(DMX512_qlsf_s *qlsf){
////
////	uint8_t tmp_path_sz = strlen(qlsf->path) + strlen(QLSF_TEMP_FILE_EXT);
////	TCHAR tmp_path[tmp_path_sz];
////	FIL tmp_file;
////
////	sprintf(tmp_path, "%s%s", qlsf->path, QLSF_TEMP_FILE_EXT);
////
////	f_unlink(tmp_path);
////
////	if(f_open(&tmp_file, tmp_path, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK){
////
////		DMX512_fixture_s *fixtures = DMX512_fixture_pool_get_all();
////		uint16_t fixture_count = DMX512_fixture_pool_get_size();
////		uint16_t patch_buf[QLSF_PATCH_DATA_SIZE];
////
////		for(uint16_t i=0; i<fixture_count; i++){
////			UINT btr;
////			patch_buf[QLSF_PATCH_CHUNK_ID_INDEX]   = fixtures[i].id;
////			patch_buf[QLSF_PATCH_CHUNK_ADDR_INDEX] = fixtures[i].addr;
////			patch_buf[QLSF_PATCH_CHUNK_CHN_INDEX]  = fixtures[i].ch_count;
////			f_write(&tmp_file, patch_buf, QLSF_PATCH_DATA_SIZE * sizeof(uint16_t), &btr);
////		}
////
////		f_close(&tmp_file);
////		f_unlink(qlsf->path);
////		f_rename(tmp_path, qlsf->path);
////
////	}
////
////}
//
//
///**
// * Loads header information into the parser instance
// *
// * Data contained within a QLSF file header conveys information regarding
// * the size of each groups of data chunks, thus enabling a parser to lookup
// * for show-relative information fast.
// * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
// * @see qlsf_defs.h for further information regarding error codes
// */
//static DMX512_engine_err_e _DMX512_qlsf_parser_load_header(DMX512_qlsf_s *qlsf){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//	uint16_t buf[QLSF_HEADER_SIZE];
//	UINT br;
//
//	f_lseek(&qlsf->file, 0);
//
//	if(f_read(&qlsf->file, buf, QLSF_HEADER_SIZE * sizeof(uint16_t), &br) == FR_OK){
//		qlsf->header.fixture_count	= buf[QLSF_HEADER_PATCH_CNT_INDEX];
//		qlsf->header.scene_count 	= buf[QLSF_HEADER_SCENE_CNT_INDEX];
//		qlsf->header.chaser_count 	= buf[QLSF_HEADER_CHASE_CNT_INDEX];
//	}else{
//		err = 1;
//	}
//
//	return err;
//
//}
//
///**
// * Loads patch defined within the config file into the DMX51 engine
// *
// * Patch information is conveyed into 6 bytes data chunk:
// * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
// * @see DMX512_qlsf_defs.h for further information regarding error codes
// */
//static DMX512_engine_err_e _DMX512_qlsf_parser_load_patch(DMX512_qlsf_s *qlsf){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//
//	for(int i=0; i< qlsf->header.fixture_count; i++){
//
//		uint16_t patch_buf[QLSF_PATCH_DATA_SIZE];
//		UINT br;
//
//		if(f_read(&qlsf->file, patch_buf, QLSF_PATCH_DATA_SIZE * sizeof(uint16_t), &br) != FR_OK){
//			err = QLFS_PATCH_DEF_ERR; //TODO: rename err enumeration
//		}else if(br < QLSF_PATCH_DATA_SIZE){
//			err = QLFS_PATCH_DEF_ERR; //TODO: rename err enumeration
//		}else{
//			uint16_t id   = patch_buf[QLSF_PATCH_CHUNK_ID_INDEX];
//			uint16_t addr = patch_buf[QLSF_PATCH_CHUNK_ADDR_INDEX];
//			uint16_t chn  = patch_buf[QLSF_PATCH_CHUNK_CHN_INDEX];
//			DMX512_fixture_pool_add(id, addr, chn);	//TODO: try and define universal error enumeration values for better error arsing
//		}
//
//	}
//
//	return err;
//
//}
//
///**
// * Loads scenes defined within the config file into the DMX512 engine
// *
// * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
// * @see qlsf_defs.h for further information regarding error codes
// */
//static DMX512_engine_err_e _DMX512_qlsf_parser_load_scenes(DMX512_qlsf_s *qlsf){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//
//	for(uint16_t i=0; i<qlsf->header.scene_count; i++){
//
//		uint16_t header[QLSF_SCENE_HEADER_SIZE];
//
//		f_read(&qlsf->file, header, QLSF_SCENE_HEADER_SIZE * sizeof(uint16_t), NULL);
//
//		uint16_t scene_id 	  = header[QLSF_SCENE_ID_INDEX];
//		uint16_t preset_count = header[QLSF_SCENE_FIXTURE_PRESET_CNT_INDEX];
//		DMX512_scene_s scene = DMX512_scene_new(scene_id, 100, 100);
//
//		for(uint16_t j=0; j<preset_count;j++){
//
//			uint16_t preset_header[QLSF_FIXTURE_PRESET_HEADER_SIZE];
//
//			f_read(&qlsf->file, preset_header, QLSF_FIXTURE_PRESET_HEADER_SIZE * sizeof(uint16_t), NULL);
//
//			uint16_t fixture_id = preset_header[QLSF_FIXTURE_PRESET_ID_INDEX];
//			uint16_t ch_count 	= preset_header[QLSF_FIXTURE_PRESET_CHANNEL_CNT_INDEX];
//			uint16_t channels[ch_count];
//			uint8_t values[ch_count];
//
//			f_read(&qlsf->file, channels, ch_count * sizeof(uint16_t), NULL);
//			f_read(&qlsf->file, values, ch_count * sizeof(uint8_t), NULL);
//
//			DMX512_fixture_s *fixture 		= DMX512_fixture_pool_ge(fixture_id);
//			DMX512_fixture_preset_s preset 	= DMX512_fixture_preset_new(fixture, ch_count, channels, values);
//
//			if((err = DMX512_scene_add(&scene, preset) != DMX512_ENGINE_OK )){
//				break;
//			}
//
//		}
//
//		DMX512_scene_pool_add(scene);
//		//if(err != DMX512_ENGINE_OK || (err = DMX512_scene_pool_add(scene)) != DMX512_ENGINE_OK){ break; }
//
//	}
//
//	return err;
//}
//
//static DMX512_engine_err_e _DMX512_qlsf_parser_load_chasers(DMX512_qlsf_s *qlsf){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//
//	for(uint16_t i=0; i<qlsf->header.chaser_count; i++){
//
//		uint16_t chaser_header[QLSF_CHASER_HEADER_SIZE];
//
//		f_read(&qlsf->file, chaser_header, QLSF_CHASER_HEADER_SIZE * sizeof(uint16_t), NULL);
//
//		uint16_t chaser_id     = chaser_header[QLSF_CHASER_ID_INDEX];
//		uint16_t chaser_mode   = chaser_header[QLSF_CHASER_RUN_MODE_INDEX];
//		uint16_t chaser_dir    = chaser_header[QLSF_CHASER_DIR_MODE_INDEX];
//		uint16_t step_count    = chaser_header[QLSF_CHASER_STEP_COUNT_INDEX];
//		DMX512_chaser_s chaser = DMX512_chaser_init(chaser_id, chaser_mode, chaser_dir);
//
//		for(uint16_t j=0; j<step_count; j++){
//
//			uint16_t step_buffer[4];
//
//			f_read(&qlsf->file, step_buffer, 4 * sizeof(uint16_t), NULL);
//
//			uint16_t scene_id = step_buffer[QLSF_STEP_ID_INDEX];
//			uint16_t fadein   = step_buffer[QLSF_STEP_FADEIN_INDEX];
//			uint16_t fadeout  = step_buffer[QLSF_STEP_FADEOUT_INDEX];
//			uint16_t hold  	  = step_buffer[QLSF_STEP_HOLD_INDEX];
//
//			DMX512_scene_s *scene 	  = DMX512_scene_pool_get(scene_id);
//			DMX512_chaser_step_s step = DMX512_chaser_step_init(scene, fadein, fadeout, hold);
//
//			if((err = DMX512_chaser_add(&chaser, step) != DMX512_ENGINE_OK )){ break; }
//
//		}
//
//		//if(err != DMX512_ENGINE_OK || (err = DMX512_chaser_pool_add(chaser)) != DMX512_ENGINE_OK){ break; }
//
//	}
//
//
//	return err;
//
//}
//
//
///**============================================================================================================================
// * Public functions definitions
// * These functions can be accessed outside of the file's scope
// * @see qlsf_parser.h for declarations
// *=============================================================================================================================*/
//
///**
// * Initialises the "Q Lightweight Show File" parser
// *
// * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
// * @see qlsf_defs.h for further information regarding error codes
// */
//DMX512_engine_err_e DMX512_qlsf_parser_load(DMX512_qlsf_s *qlsf){
//
//	DMX512_engine_err_e err = DMX512_ENGINE_OK;
//
//	if(fs_get_mount_status() != FS_MOUNTED){
//		//err = DMX512_QLSF_FS_NOT_READY;
//	}else if(f_open(&qlsf->file, qlsf->path, FA_OPEN_ALWAYS | FA_READ ) != FR_OK){
//		err = 2;
//	}else if(_DMX512_qlsf_parser_load_header(qlsf) != DMX512_ENGINE_OK){
//		err = 2;
//	}else if(_DMX512_qlsf_parser_load_patch(qlsf) != DMX512_ENGINE_OK){
//		err = 2;
//	}else if(_DMX512_qlsf_parser_load_scenes(qlsf) != DMX512_ENGINE_OK){
//		err = 2;
//	}else if(_DMX512_qlsf_parser_load_chasers(qlsf) != DMX512_ENGINE_OK){
//		err = 2;
//	}else{
//		f_close(&qlsf->file);
//	}
//
//
//	return err;
//}
//
////DMX512_engine_err_e DMX512_qlsf_dump(DMX512_qlsf_s qlsf){
////
////	DMX512_engine_err_e err = DMX512_ENGINE_OK;
////
////	uint8_t tmp_path_sz = strlen(qlsf->path) + strlen(QLSF_TEMP_FILE_EXT);
////	TCHAR tmp_path[tmp_path_sz];
////	FIL *tmp_file;
////
////	sprintf(tmp_path, "%s%s", qlsf->path, QLSF_TEMP_FILE_EXT);
////
////	f_unlink(tmp_path);
////
////	if(fs_get_mount_status() != FS_MOUNTED){
////		err = DMX512_QLSF_FS_NOT_READY;
////	}else if(f_open(tmp_file, tmp_path, FA_WRITE | FA_OPEN_ALWAYS) != FR_OK){
////		err = DMX512_QLSF_CANNOT_CREATE_TMP_FILE;
////	}else if(DMX512_qlfs_dump_header(qlsf) != DMX512_ENGINE_OK){
////
////	}else if(DMX512_qlfs_dump_patch(qlsf) != DMX512_ENGINE_OK){
////
////	}else if(DMX512_qlfs_dump_scenes(qlsf) != DMX512_ENGINE_OK){
////
////	}else if(DMX512_qlfs_dump_chasers(qlsf) != DMX512_ENGINE_OK){
////
////	}
////
////	return err;
////
////}
