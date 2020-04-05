/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#if cueOS_CONFIG_NODETYPE == cueOS_NODETYPE_SLAVE_DMX

#include "cueos_config.h"
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
static osThreadId_t DMX512engineThread = NULL;

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
 * @see DMX512_engine.h for further information regarding error codes
 */
static DMX512_engine_err_e _DMX512_engine_load_config_header(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	uint16_t buf[DMX512_ENGINE_CONFIG_HEADER_SIZE];
	UINT br;

	f_lseek(config_file, 0);

	if(f_read(config_file, buf, DMX512_ENGINE_CONFIG_HEADER_SIZE * sizeof(uint16_t), &br) == FR_OK){
		this.fixture_count	= buf[DMX512_ENGINE_CONFIG_HEADER_PATCH_CNT_INDEX];
		this.scene_count 	= buf[DMX512_ENGINE_CONFIG_HEADER_SCENE_CNT_INDEX];
		this.chaser_count 	= buf[DMX512_ENGINE_CONFIG_HEADER_CHASE_CNT_INDEX];
	}else{
		err = DMX512_ENGINE_CONFIG_LOAD_EXCEPTION;
	}

	return err;

}

/**
 * Loads patch defined within the config file into the DMX51 engine
 *
 * Patch information is conveyed into 6 bytes data chunk:
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_engine.h for further information regarding error codes
 */
static DMX512_engine_err_e _DMX512_engine_load_config_patch(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_CONFIG_LOAD_EXCEPTION;

	for(int i=0; i< this.fixture_count; i++){

		uint16_t patch_buf[DMX512_ENGINE_CONFIG_PATCH_DATA_SIZE];
		UINT br;

		if(f_read(config_file, patch_buf, DMX512_ENGINE_CONFIG_PATCH_DATA_SIZE * sizeof(uint16_t), &br) != FR_OK){
		}else if(br < DMX512_ENGINE_CONFIG_PATCH_DATA_SIZE){
		}else{
			uint16_t id   = patch_buf[DMX512_ENGINE_CONFIG_PATCH_CHUNK_ID_INDEX];
			uint16_t addr = patch_buf[DMX512_ENGINE_CONFIG_PATCH_CHUNK_ADDR_INDEX];
			uint16_t chn  = patch_buf[DMX512_ENGINE_CONFIG_PATCH_CHUNK_CHN_INDEX];
			DMX512_fixture_s fixture = DMX512_fixture_new(id, addr, chn);
			if(DMX512_fixture_pool_add(this.fixtures, fixture) != DMX512_ENGINE_OK){
				err = DMX512_ENGINE_CONFIG_LOAD_EXCEPTION;
				break;
			}else{
				err = DMX512_ENGINE_OK;
			}
		}

	}

	return err;

}

/**
 * Loads scenes defined within the config file into the DMX512 engine
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_engine.h for further information regarding error codes
 */
//TODO: clean and minify for clarity
//TODO: check for errors here
static DMX512_engine_err_e _DMX512_engine_load_config_scenes(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(uint16_t i=0; i<this.scene_count; i++){

		uint16_t header[DMX512_ENGINE_CONFIG_SCENE_HEADER_SIZE];
		f_read(config_file, header, DMX512_ENGINE_CONFIG_SCENE_HEADER_SIZE * sizeof(uint16_t), NULL);
		uint16_t scene_id 	  = header[DMX512_ENGINE_CONFIG_SCENE_ID_INDEX];
		uint16_t preset_count = header[DMX512_ENGINE_CONFIG_SCENE_FIXTURE_PRESET_CNT_INDEX];
		DMX512_scene_s scene  = DMX512_scene_new(scene_id, 250, 250);
		for(uint16_t j=0; j<preset_count;j++){
			uint16_t preset_header[DMX512_ENGINE_CONFIG_FIXTURE_PRESET_HEADER_SIZE];
			f_read(config_file, preset_header, DMX512_ENGINE_CONFIG_FIXTURE_PRESET_HEADER_SIZE * sizeof(uint16_t), NULL);
			uint16_t fixture_id = preset_header[DMX512_ENGINE_CONFIG_FIXTURE_PRESET_ID_INDEX];
			uint16_t ch_count 	= preset_header[DMX512_ENGINE_CONFIG_FIXTURE_PRESET_CHANNEL_CNT_INDEX];
			uint16_t channels[ch_count];
			uint8_t values[ch_count];
			f_read(config_file, channels, ch_count * sizeof(uint16_t), NULL);
			f_read(config_file, values, ch_count * sizeof(uint8_t), NULL);

//			FIXME: update following changes made to  fixture pool "get" function
//			DMX512_fixture_s *fixture 		= DMX512_fixture_pool_get(this.fixtures, fixture_id);
//			DMX512_fixture_preset_s preset 	= DMX512_fixture_preset_new(fixture, ch_count, channels, values);
//			DMX512_scene_add_preset(&scene, preset);
		}

		DMX512_scene_pool_add(this.scenes, scene);

	}

	return err;
}

/**
 * Loads chasers defined within the config file into the DMX512 engine
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_engine.h for further information regarding error codes
 */
//TODO: check for errors here
static DMX512_engine_err_e _DMX512_engine_load_config_chasers(FIL *config_file){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	for(uint16_t i=0; i<this.chaser_count; i++){

		uint16_t chaser_header[DMX512_ENGINE_CONFIG_CHASER_HEADER_SIZE];

		f_read(config_file, chaser_header, DMX512_ENGINE_CONFIG_CHASER_HEADER_SIZE * sizeof(uint16_t), NULL);

		uint16_t chaser_id     = chaser_header[DMX512_ENGINE_CONFIG_CHASER_ID_INDEX];
		uint16_t chaser_dir    = chaser_header[DMX512_ENGINE_CONFIG_CHASER_DIR_MODE_INDEX];
		uint16_t chaser_mode   = chaser_header[DMX512_ENGINE_CONFIG_CHASER_RUN_MODE_INDEX];
		uint16_t step_count    = chaser_header[DMX512_ENGINE_CONFIG_CHASER_STEP_COUNT_INDEX];
		DMX512_chaser_s chaser = DMX512_chaser_new(chaser_id, chaser_mode, chaser_dir);

		for(uint16_t j=0; j<step_count; j++){

			uint16_t step_buffer[4];

			f_read(config_file, step_buffer, 4 * sizeof(uint16_t), NULL);

			uint16_t scene_id = step_buffer[DMX512_ENGINE_CONFIG_STEP_ID_INDEX];
			uint16_t fadein   = step_buffer[DMX512_ENGINE_CONFIG_STEP_FADEIN_INDEX];
			uint16_t fadeout  = step_buffer[DMX512_ENGINE_CONFIG_STEP_FADEOUT_INDEX];
			uint16_t hold  	  = step_buffer[DMX512_ENGINE_CONFIG_STEP_HOLD_INDEX];

			DMX512_scene_s *scene 	  = DMX512_scene_pool_get(this.scenes, scene_id);
			DMX512_chaser_step_s step = DMX512_chaser_step_init(scene, fadein, fadeout, hold);

			//if((err = DMX512_chaser_add_step(&chaser, step)) != DMX512_ENGINE_OK ){ break; }
			DMX512_chaser_add_step(&chaser, step);

		}

		DMX512_chaser_pool_add(this.chasers, chaser);

	}


	return err;

}

/**
 * Thread managing engine functions execution
 *
 * Currently available DMX512 functions include:
 * Scene: a set of predefined values for one or many fixtures
 * Chaser: a chained set of scenes defined as "steps" executed sequentially
 * Effect: a set of mathematical function used as individual channel actuator for one or many fixtures
 * @param *arg pointer to argument passed during thread creation. Not used here.
 */
static void _DMX512_engine_manage(void *arg){
	for(;;){
		DMX512_scene_pool_manage(this.scenes);
		DMX512_chaser_pool_manage(this.chasers);
		osDelay(DMX512_ENGINE_THREAD_DELAY);
	}
}


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_engine.h for declarations
 *=============================================================================================================================*/

/**
 * Loads a show configuration from a QLSF file
 *
 * @return DMX512_engine_err_e DMX512_ENGINE_OK on success, specific error code otherwise
 * @see DMX512_defs.h for further information regarding error codes
 */
DMX512_engine_err_e DMX512_engine_load_config(TCHAR *config_file_path){

	DMX512_engine_err_e err = DMX512_ENGINE_CONFIG_LOAD_EXCEPTION;
	FIL config_file;

	if(fs_get_mount_status() != FS_MOUNTED){ fs_init(); }

	if(f_open(&config_file, config_file_path, FA_OPEN_ALWAYS | FA_READ ) != FR_OK){
	}else if(_DMX512_engine_load_config_header(&config_file)  != DMX512_ENGINE_OK){
	}else if(_DMX512_engine_load_config_patch(&config_file)   != DMX512_ENGINE_OK){
	}else if(_DMX512_engine_load_config_scenes(&config_file)  != DMX512_ENGINE_OK){
	}else if(_DMX512_engine_load_config_chasers(&config_file) != DMX512_ENGINE_OK){
	}else{
		err = DMX512_ENGINE_OK;
		f_close(&config_file);
	}

	return err;
}

/**
 * Initialises the DMX512 engine singleton.
 */
void DMX512_engine_init(void){

	this.fixture_count  = 0;
	this.scene_count	= 0;
	this.chaser_count	= 0;
	this.effect_count	= 0;
	this.fixtures 		= DMX512_fixture_pool_new();
	this.scenes 		= DMX512_scene_pool_new();
	this.chasers 		= DMX512_chaser_pool_new();

	if(DMX512_driver_get_status() != DMX512_DRIVER_INITIALISED){
		DMX512_driver_init();
	}

	DMX512_engine_start();

}

//TODO: maybe add a "started state" to prevent recursive calls to start/stop function

/**
 * Starts the DMX512 driver and launches the DMX512 engine management thread
 */
void DMX512_engine_start(void){
	DMX512_driver_start();
	DMX512engineThread = osThreadNew(_DMX512_engine_manage, NULL, NULL);
}

/**
 * Stops the DMX512 driver and terminates the DMX512 engine management thread
 */
void DMX512_engine_stop(void){
	DMX512_driver_stop();
	osThreadTerminate(DMX512engineThread);
}

/**
 * Wrapper for "DMX512_fixture_pool_add" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param id the fixture's identifier
 * @param addr fixture's first channel address
 * @param ch_stop fixture's last channel address
 * @return DMX512_engine_err_e error code following the function call
 * @see DMX512_defs.h for further information regarding DMX512 engin error codes
 */
DMX512_engine_err_e DMX512_engine_patch_add(uint16_t fixture_id, uint16_t address, uint16_t ch_count){
	DMX512_fixture_s fixture = DMX512_fixture_new(fixture_id, address, ch_count);
	return DMX512_fixture_pool_add(this.fixtures, fixture);
}

/**
 * Wrapper for "DMX512_fixture_pool_get" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param id the fixture's identifier
 * @param **fixture pointer to the fixture
 * @return DMX512_engine_err_e ERR_OK if fixture was found, DMX512_ENGINE_INSTANCE_UNDEFINED otherwise
 */
DMX512_engine_err_e DMX512_engine_patch_get(uint16_t fixture_id, DMX512_fixture_s **fixture){
	return DMX512_fixture_pool_get(this.fixtures, fixture_id, fixture);
}

/**
 * Returns the current engine fixture patch
 *
 * @return *DMX512_fixture_pool_s pointer to the engine's fixture pool
 */
DMX512_fixture_pool_s *DMX512_engine_patch_get_all(void){
	return this.fixtures;
}

/**
 * Wrapper for "DMX512_fixture_pool_del" function. Provides context to the specified function using
 * DMX512 engine's singleton parameter "fixtures" as argument.
 *
 * @param id the fixture's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_engine_patch_delete(uint16_t fixture_id){
	return DMX512_fixture_pool_del(this.fixtures, fixture_id);
}

#endif
