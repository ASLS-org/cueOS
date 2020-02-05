/**
  ******************************************************************************
  * @file   fatfs.h
  * @brief  Header for fatfs applications
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fs_H
#define __fs_H

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

typedef enum{
	FS_OK,
	FS_DRIVER_ERR,
	FS_MOUNT_ERR,
	FS_WRITE_ERR,
	FS_READ_ERR,
	FS_FILE_NOT_FOUND
}fs_err_e;

typedef enum{
	FS_SD_UNMOUNTED,
	FS_SD_MOUNTED
}fs_sd_state_e;

typedef struct{
	FATFS SDFatFs;
	uint8_t SDValue;
	TCHAR SDPath[4];
	fs_sd_state_e SDMountState;
}fs_s;


fs_err_e fs_mountSD(void);
fs_err_e fs_readFile(const TCHAR* path, uint8_t *data);
fs_err_e fs_writeFile(char *data, char *path, char *name);


#endif /*__fatfs_H */
