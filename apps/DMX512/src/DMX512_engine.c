/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "DMX512_driver.h"
#include "DMX512_engine.h"


/**============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static DMX512_engine_s this;


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Loads header information into the parser instance
 *
 * Data contained within a QLSF file header conveys information regarding
 * the size of each groups of data chunks, thus enabling a parser to lookup
 * for show-relative information fast.
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see qlsf_defs.h for further information regarding error codes
 */
static DMX512_engine_err_e _DMX512_engine_load_config_header(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	uint16_t buf[QLSF_HEADER_SIZE];
	UINT br;

	f_lseek(config_file, 0);

	if(f_read(config_file, buf, QLSF_HEADER_SIZE * sizeof(uint16_t), &br) == FR_OK){
		this.fixture_count	= buf[QLSF_HEADER_PATCH_CNT_INDEX];
		this.scene_count 	= buf[QLSF_HEADER_SCENE_CNT_INDEX];
		this.chaser_count 	= buf[QLSF_HEADER_CHASE_CNT_INDEX];
	}else{
		err = 1;
	}

	return err;

}

/**
 * Loads patch defined within the config file into the DMX51 engine
 *
 * Patch information is conveyed into 6 bytes data chunk:
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_qlsf_defs.h for further information regarding error codes
 */
static DMX512_engine_err_e _DMX512_engine_load_config_patch(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(int i=0; i< this.fixture_count; i++){

		uint16_t patch_buf[QLSF_PATCH_DATA_SIZE];
		UINT br;

		if(f_read(config_file, patch_buf, QLSF_PATCH_DATA_SIZE * sizeof(uint16_t), &br) != FR_OK){
			err = -1; //TODO: rename err enumeration
		}else if(br < QLSF_PATCH_DATA_SIZE){
			err = -1; //TODO: rename err enumeration
		}else{
			uint16_t id   = patch_buf[QLSF_PATCH_CHUNK_ID_INDEX];
			uint16_t addr = patch_buf[QLSF_PATCH_CHUNK_ADDR_INDEX];
			uint16_t chn  = patch_buf[QLSF_PATCH_CHUNK_CHN_INDEX];
			DMX512_fixture_s fixture = DMX512_fixture_new(id, addr, chn);
			DMX512_fixture_pool_add(this.fixtures, fixture);	//TODO: try and define universal error enumeration values for better error arsing
		}

	}

	return err;

}

/**
 * Loads scenes defined within the config file into the DMX512 engine
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see qlsf_defs.h for further information regarding error codes
 */
//TODO: clean and minify for clarity
static DMX512_engine_err_e _DMX512_engine_load_config_scenes(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(uint16_t i=0; i<this.scene_count; i++){

		uint16_t header[QLSF_SCENE_HEADER_SIZE];

		f_read(config_file, header, QLSF_SCENE_HEADER_SIZE * sizeof(uint16_t), NULL);

		uint16_t scene_id 	  = header[QLSF_SCENE_ID_INDEX];
		uint16_t preset_count = header[QLSF_SCENE_FIXTURE_PRESET_CNT_INDEX];
		DMX512_scene_s scene  = DMX512_scene_new(scene_id, 250, 250);

		for(uint16_t j=0; j<preset_count;j++){

			uint16_t preset_header[QLSF_FIXTURE_PRESET_HEADER_SIZE];

			f_read(config_file, preset_header, QLSF_FIXTURE_PRESET_HEADER_SIZE * sizeof(uint16_t), NULL);

			uint16_t fixture_id = preset_header[QLSF_FIXTURE_PRESET_ID_INDEX];
			uint16_t ch_count 	= preset_header[QLSF_FIXTURE_PRESET_CHANNEL_CNT_INDEX];
			uint16_t channels[ch_count];
			uint8_t values[ch_count];

			f_read(config_file, channels, ch_count * sizeof(uint16_t), NULL);
			f_read(config_file, values, ch_count * sizeof(uint8_t), NULL);

			DMX512_fixture_s *fixture 		= DMX512_fixture_pool_get(this.fixtures, fixture_id);
			DMX512_fixture_preset_s preset 	= DMX512_fixture_preset_new(fixture, ch_count, channels, values);

			//if((err = DMX512_scene_add_preset(&scene, preset) != DMX512_ENGINE_OK )){ break; }
			DMX512_scene_add_preset(&scene, preset);
		}

		DMX512_scene_pool_add(this.scenes, scene);

	}

	return err;
}

static DMX512_engine_err_e _DMX512_engine_load_config_chasers(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(uint16_t i=0; i<this.chaser_count; i++){

		uint16_t chaser_header[QLSF_CHASER_HEADER_SIZE];

		f_read(config_file, chaser_header, QLSF_CHASER_HEADER_SIZE * sizeof(uint16_t), NULL);

		uint16_t chaser_id     = chaser_header[QLSF_CHASER_ID_INDEX];
		uint16_t chaser_mode   = chaser_header[QLSF_CHASER_RUN_MODE_INDEX];
		uint16_t chaser_dir    = chaser_header[QLSF_CHASER_DIR_MODE_INDEX];
		uint16_t step_count    = chaser_header[QLSF_CHASER_STEP_COUNT_INDEX];
		DMX512_chaser_s chaser = DMX512_chaser_new(chaser_id, chaser_mode, chaser_dir);

		for(uint16_t j=0; j<step_count; j++){

			uint16_t step_buffer[4];

			f_read(config_file, step_buffer, 4 * sizeof(uint16_t), NULL);

			uint16_t scene_id = step_buffer[QLSF_STEP_ID_INDEX];
			uint16_t fadein   = step_buffer[QLSF_STEP_FADEIN_INDEX];
			uint16_t fadeout  = step_buffer[QLSF_STEP_FADEOUT_INDEX];
			uint16_t hold  	  = step_buffer[QLSF_STEP_HOLD_INDEX];

			DMX512_scene_s *scene 	  = DMX512_scene_pool_get(this.scenes, scene_id);
			DMX512_chaser_step_s step = DMX512_chaser_step_init(scene, fadein, fadeout, hold);

			//if((err = DMX512_chaser_add_step(&chaser, step) != DMX512_ENGINE_OK )){ break; }
			DMX512_chaser_add_step(&chaser, step);

		}

		DMX512_chaser_pool_add(this.chasers, chaser);

	}


	return err;

}

static void _DMX512_engine_manage(void *arg){
	for(;;){
		DMX512_scene_pool_manage(this.scenes);
		DMX512_chaser_pool_manage(this.chasers);
		osDelay(1);
	}
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see qlsf_parser.h for declarations
 *=============================================================================================================================*/

/**
 * Loads a show configuration from a QLSF file
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_defs.h for further information regarding error codes
 */
DMX512_engine_err_e DMX512_engine_load_config(TCHAR *config_file_path){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	FIL config_file;

	//TODO: update error codes
	if(fs_get_mount_status() != FS_MOUNTED){
		err = 2;
	}else if(f_open(&config_file, config_file_path, FA_OPEN_ALWAYS | FA_READ ) != FR_OK){
		err = 2;
	}else if(_DMX512_engine_load_config_header(&config_file)  != DMX512_ENGINE_OK){
		err = 2;
	}else if(_DMX512_engine_load_config_patch(&config_file)   != DMX512_ENGINE_OK){
		err = 2;
	}else if(_DMX512_engine_load_config_scenes(&config_file)  != DMX512_ENGINE_OK){
		err = 2;
	}else if(_DMX512_engine_load_config_chasers(&config_file) != DMX512_ENGINE_OK){
		err = 2;
	}else{
		f_close(&config_file);
	}

	return err;
}

/**
 * Initialises the DMX512 engine
 *
 * @param *config_file_name pointer to the configuration path string
 */
void DMX512_engine_init(TCHAR *config_file_name){

	fs_init();

	this.fixture_count  = 0;
	this.scene_count	= 0;
	this.chaser_count	= 0;
	this.effect_count	= 0;

	this.fixtures 	= DMX512_fixture_pool_new();
	this.scenes 	= DMX512_scene_pool_new();
	this.chasers 	= DMX512_chaser_pool_new();

	DMX512_engine_err_e err = DMX512_engine_load_config(config_file_name);

	DMX512_driver_init();
	DMX512_driver_start();

	osDelay(500);	//Mandatory delay to ensure at least e single DMX512 frame is sent by the driver

	osThreadNew(_DMX512_engine_manage, NULL, NULL);

	DMX512_chaser_start(&this.chasers->chasers[0]);


}
