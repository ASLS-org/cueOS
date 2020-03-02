#ifndef _fs_H
#define _fs_H

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */


#define FS_DEFAULT  {FS_LINKSTATUS_UNLINKED, FS_UNMOUNTED}


typedef enum{
	FS_OK,
	FS_CANNOT_LINK_SD,
	FS_CANNOT_MOUNT_SD,
	FS_ALREADY_MOUNTED,
}fs_err_e;

typedef enum{
	FS_LINKSTATUS_UNLINKED 	= 1,
	FS_LINKSTATUS_LINKED	= 0
}fs_link_status_e;

typedef enum{
	FS_MOUNTED,
	FS_UNMOUNTED
}fs_mount_status_e;


typedef struct{
	fs_link_status_e _link_status;
	fs_mount_status_e _mount_status;
}fs_s;


fs_err_e fs_init(void);
fs_mount_status_e fs_get_mount_status(void);

#endif
