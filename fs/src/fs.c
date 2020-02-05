/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include <stdio.h>
#include "fs.h"

static fs_s *this;

fs_err_e fs_mountSD(void){

	fs_err_e err = FS_OK;

	this->SDValue =  FATFS_LinkDriver(&SD_Driver, this->SDPath);

	if(this->SDMountState == FS_SD_MOUNTED){
		if(this->SDValue == 0){
			if(f_mount(&this->SDFatFs, this->SDPath, 0) != FR_OK){
				err = FS_MOUNT_ERR;
			}
		}else{
			err = FS_DRIVER_ERR;
		}
	}else{
		err = FS_MOUNT_ERR;
	}

	this->SDMountState = FS_SD_MOUNTED;

	return err;
}


DWORD get_fattime(void) {
  return 0;
}
