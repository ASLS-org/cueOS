#include <stdio.h>
#include "fs.h"
#include "mmc_driver.h"

/**
 * Pivate variable declarations
 *
 * These variable values may only be manipulated within this file scope
 * Singleton's parameters may be manipulated through the use of getters
 * and setters
 * @see fs.h for the list of public functions
 */
static fs_s this = FS_DEFAULT;									//Declaring fs singleton instance as "this" and defaulting its values to FS_DEFAULT
static FATFS _fatfs;											//Declaring file system object structure
static TCHAR _path[4];											//Declaring media path instance


/**
 * Initialises the file system
 *
 * @return fs_err_e returns 0 in case of succes, negative value otherwise
 * @see fs.h for fs_err_e error enumeration
 */
fs_err_e fs_init(void){

	fs_err_e err = FS_OK;										//Error return value defaulted to FS_OK

	mmc_init();													//Initialising SD handle Structure

	this._link_status = !FATFS_LinkDriver(&SD_Driver, _path);	//Linking SD card driver to fatfs

	if(this._mount_status == FS_MOUNTED){						//Checking if the file system is already initialised
		err = FS_OK;											//The file system is already initialised
	}else if(this._link_status == 0){							//Checking if the driver was succcessfully linked
		err = FS_CANNOT_LINK_SD;								//Media driver could not be linked
	}else if(f_mount(&_fatfs, _path, 0) != FR_OK){				//Try mounting the filesystem
		err = FS_CANNOT_MOUNT_SD;								//Media could not be mounted
	}else{
		this._mount_status = FS_MOUNTED;						//Inform structure that the filesystem has been initialised
	}

	return err;													//Returning final error value

}

/**
 * Returns the file system's current state
 *
 * @return fs_mount_status_e returns 0 if mounted 1 otherwise
 * @see fs.h for fs_mount_status_e error enumeration
 */
fs_mount_status_e fs_get_mount_status(void){
	return this._mount_status;									//returning mount status
}
