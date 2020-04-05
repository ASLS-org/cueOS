#ifndef _fs_H
#define _fs_H

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

/**
 * @ingroup filesystem
 * @def FS_DEFAULT
 * @brief file system instance default values
 *
 * Used during initialisation of file system, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define FS_DEFAULT  {FS_LINKSTATUS_UNLINKED, FS_UNMOUNTED}

/**
 * @ingroup filesystem
 * @enum fs_err_e
 * @brief file system error constants definition
 *
 * Series of pre-defined constant error values to be used
 * throughout the whole file system lifecycle.
 */
typedef enum{
	FS_OK,								/**< No error*/
	FS_CANNOT_LINK_SD,					/**< file system link driver could not link SD card media*/
	FS_CANNOT_MOUNT_SD,					/**< file system could not mount SD card media*/
	FS_ALREADY_MOUNTED,					/**< The file system has already been initialised*/
}fs_err_e;

/**
 * @ingroup filesystem
 * @enum fs_link_status_e
 * @brief Conveys information relative to whether the filesystem driver has been linked or not
 */
typedef enum{
	FS_LINKSTATUS_UNLINKED 	= 1,		/**< File system driver linking failed*/
	FS_LINKSTATUS_LINKED	= 0			/**< File system driver was successfully linked*/
}fs_link_status_e;

/**
 * @ingroup filesystem
 * @enum fs_mount_status_e
 * @brief Actual initialisation state of the file system
 */
typedef enum{
	FS_MOUNTED,							/**< File system is initialised and media has been correctly mounted*/
	FS_UNMOUNTED						/**< File system is uninitialised and media hasn't been mounted yet*/
}fs_mount_status_e;

/**
 * @ingroup filesystem
 * @struct fs_s
 * @brief filesystem object
 */
typedef struct{
	fs_link_status_e _link_status;		/**< the current file system driver link status @see fs_link_status_e*/
	fs_mount_status_e _mount_status;	/**< the current file system mount status @see fs_mount_status_e*/
}fs_s;


fs_err_e fs_init(void);
fs_mount_status_e fs_get_mount_status(void);

#endif
