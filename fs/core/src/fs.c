/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependencies declarations as possible
 *=============================================================================================================================*/

#include <stdio.h>
#include "fs.h"
#include "mmc_driver.h"


/***============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static fs_s this = FS_DEFAULT;
static FATFS _fatfs;
static TCHAR _path[4];


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_chaser.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Initialises the file system
 *
 * @return fs_err_e returns 0 in case of succes, negative value otherwise
 * @see fs.h for fs_err_e error enumeration
 */
fs_err_e fs_init(void){

	fs_err_e err = FS_OK;

	mmc_init();

	this._link_status = FATFS_LinkDriver(&SD_Driver, (char *)_path);

	if(this._mount_status == FS_MOUNTED){
		err = FS_OK;
	}else if(this._link_status == FS_LINKSTATUS_UNLINKED){
		err = FS_CANNOT_LINK_SD;
	}else if(f_mount(&_fatfs, _path, 0) != FR_OK){
		err = FS_CANNOT_MOUNT_SD;
	}else{
		this._mount_status = FS_MOUNTED;
	}

	return err;

}

/**
 * @brief Returns the file system's current state
 *
 * @return fs_mount_status_e returns 0 if mounted 1 otherwise
 * @see fs.h for fs_mount_status_e error enumeration
 */
fs_mount_status_e fs_get_mount_status(void){
	return this._mount_status;
}
