/***============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "QLSF_manager.h"
#include "fs.h"
#include "DMX512_engine.h"


static QLFS_manager_s QLFS_manager;


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Loads header information into the parser instance
 *
 * Data contained within a QLSF file header conveys information regarding
 * the size of each groups of data chunks, thus enabling a parser to lookup
 * for show-relative information fast.
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see QLFS_manager.h for further information regarding error codes
 */
static DMX512_engine_err_e _QLFS_manager_load_header(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	uint16_t buf[QLFS_HEADER_SIZE];
	UINT br;

	f_lseek(config_file, 0);

	if(f_read(config_file, buf, QLFS_HEADER_SIZE * sizeof(uint16_t), &br) == FR_OK){
		QLFS_manager.fixture_count	= buf[QLFS_HEADER_PATCH_CNT_INDEX];
		QLFS_manager.scene_count 	= buf[QLFS_HEADER_SCENE_CNT_INDEX];
		QLFS_manager.chaser_count 	= buf[QLFS_HEADER_CHASE_CNT_INDEX];
	}else{
		err = 0;
	}

	return err;

}

/**
 * @brief Loads patch defined within the config file into the DMX51 QLFS_manager
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see QLFS_manager.h for further information regarding error codes
 */
static DMX512_engine_err_e _QLFS_manager_load_patch(FIL *config_file){

	DMX512_engine_err_e err = 0;

	for(int i=0; i< QLFS_manager.fixture_count; i++){

		uint16_t patch_buf[QLFS_PATCH_DATA_SIZE];
		UINT br;

		if(f_read(config_file, patch_buf, QLFS_PATCH_DATA_SIZE * sizeof(uint16_t), &br) != FR_OK){
		}else if(br < QLFS_PATCH_DATA_SIZE){
		}else{
			uint16_t id   = patch_buf[QLFS_PATCH_CHUNK_ID_INDEX];
			uint16_t addr = patch_buf[QLFS_PATCH_CHUNK_ADDR_INDEX];
			uint16_t chn  = patch_buf[QLFS_PATCH_CHUNK_CHN_INDEX];
			if(DMX512_engine_patch_add(id, addr, chn) != DMX512_ENGINE_OK){
				err = 0;
				break;
			}else{
				err = DMX512_ENGINE_OK;
			}
		}

	}

	return err;

}

/**
 * @brief Loads scenes defined within the config file into the DMX512 QLFS_manager
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see QLFS_manager.h for further information regarding error codes
 */
static DMX512_engine_err_e _QLFS_manager_load_scenes(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(uint16_t i=0; i<QLFS_manager.scene_count; i++){

		uint16_t header[QLFS_SCENE_HEADER_SIZE];

		f_read(config_file, header, QLFS_SCENE_HEADER_SIZE * sizeof(uint16_t), NULL);

		uint16_t scene_id 	  = header[QLFS_SCENE_ID_INDEX];
		uint16_t preset_count = header[QLFS_SCENE_FIXTURE_PRESET_CNT_INDEX];
		uint16_t fadein_time  = header[QLFS_SCENE_FADEIN_TIME_INDEX];
		uint16_t fadeout_time = header[QLFS_SCENE_FADEOUT_TIME_INDEX];

		DMX512_engine_scene_add(scene_id, fadein_time, fadeout_time);

		for(uint16_t j=0; j<preset_count;j++){

			uint16_t preset_header[QLFS_FIXTURE_PRESET_HEADER_SIZE];

			f_read(config_file, preset_header, QLFS_FIXTURE_PRESET_HEADER_SIZE * sizeof(uint16_t), NULL);

			uint16_t fixture_id = preset_header[QLFS_FIXTURE_PRESET_ID_INDEX];
			uint16_t ch_count 	= preset_header[QLFS_FIXTURE_PRESET_CHANNEL_CNT_INDEX];

			uint16_t channels[ch_count];
			uint8_t values[ch_count];

			f_read(config_file, channels, ch_count * sizeof(uint16_t), NULL);
			f_read(config_file, values, ch_count * sizeof(uint8_t), NULL);

			err = DMX512_engine_scene_add_preset(scene_id, fixture_id, ch_count, channels, values);

			if(err != DMX512_ENGINE_OK){break;}

		}

		if(err != DMX512_ENGINE_OK){break;}

	}

	return err;
}

/**
 * @brief Loads chasers defined within the config file into the DMX512 QLFS_manager
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see QLFS_manager.h for further information regarding error codes
 */
//TODO: check for errors here
static DMX512_engine_err_e _QLFS_manager_load_chasers(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

//	for(uint16_t i=0; i<QLFS_manager.chaser_count; i++){
//
//		uint16_t chaser_header[QLFS_CHASER_HEADER_SIZE];
//
//		f_read(config_file, chaser_header, QLFS_CHASER_HEADER_SIZE * sizeof(uint16_t), NULL);
//
//		uint16_t chaser_id     = chaser_header[QLFS_CHASER_ID_INDEX];
//		uint16_t chaser_dir    = chaser_header[QLFS_CHASER_DIR_MODE_INDEX];
//		uint16_t chaser_mode   = chaser_header[QLFS_CHASER_RUN_MODE_INDEX];
//		uint16_t step_count    = chaser_header[QLFS_CHASER_STEP_COUNT_INDEX];
//		DMX512_chaser_s chaser = DMX512_chaser_new(chaser_id, chaser_mode, chaser_dir);
//
//		for(uint16_t j=0; j<step_count; j++){
//
//			uint16_t step_buffer[4];
//
//			f_read(config_file, step_buffer, 4 * sizeof(uint16_t), NULL);
//
//			uint16_t scene_id = step_buffer[QLFS_STEP_ID_INDEX];
//			uint16_t fadein   = step_buffer[QLFS_STEP_FADEIN_INDEX];
//			uint16_t fadeout  = step_buffer[QLFS_STEP_FADEOUT_INDEX];
//			uint16_t hold  	  = step_buffer[QLFS_STEP_HOLD_INDEX];
//
////			DMX512_scene_s *scene 	  = DMX512_scene_pool_get(QLFS_manager.scenes, scene_id);
////			DMX512_chaser_step_s step = DMX512_chaser_step_init(scene, fadein, fadeout, hold);
//
//			//if((err = DMX512_chaser_add_step(&chaser, step)) != DMX512_ENGINE_OK ){ break; }
////			DMX512_chaser_add_step(&chaser, step);
//
//		}
//
////		DMX512_chaser_pool_add(QLFS_manager.chasers, chaser);
//
//	}


	return err;

}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see QLFS_manager.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Loads a show configuration from a QLSF file
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_defs.h for further information regarding error codes
 */
DMX512_engine_err_e QLFS_manager_load(TCHAR *config_file_path){

	DMX512_engine_reset();

	DMX512_engine_err_e err = 0;
	FIL config_file;

	//TODO: handle error cases. reset engine on parse error.
	if(fs_get_mount_status() != FS_MOUNTED){
	}else if(f_open(&config_file, config_file_path, FA_OPEN_ALWAYS | FA_READ ) != FR_OK){
	}else if(_QLFS_manager_load_header(&config_file)  != DMX512_ENGINE_OK){
	}else if(_QLFS_manager_load_patch(&config_file)   != DMX512_ENGINE_OK){
	}else if(_QLFS_manager_load_scenes(&config_file)  != DMX512_ENGINE_OK){
	}else if(_QLFS_manager_load_chasers(&config_file) != DMX512_ENGINE_OK){
	}else{
		err = DMX512_ENGINE_OK;
		f_close(&config_file);
	}

	return err;
}
