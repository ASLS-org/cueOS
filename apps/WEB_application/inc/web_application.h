/**
 * @ingroup cueos_apps
 * @defgroup cueos_web_application Web Application
 * Serves web-base user interface static files over HTTP to a remote device.
 * @{
 */

#ifndef WEB_APPLICATION_H_
#define WEB_APPLICATION_H_

#include <stdlib.h>
#include <stdint.h>


#define FS_READ_EOF     -1						/**< END OF FILE*/
#define FS_READ_DELAYED -2						/**< Read is delayed*/

#define FS_FILE_FLAGS_HEADER_INCLUDED     0x01	/**< File HTTP Header is included within file daclaration*/
#define FS_FILE_FLAGS_HEADER_PERSISTENT   0x02	/**< File HTTP Header is persistant*/
#define FS_FILE_FLAGS_HEADER_HTTPVER_1_1  0x04	/**< File HTTP Header version is HTTP/1.1*/
#define FS_FILE_FLAGS_SSI                 0x08	/**< File contains server-side inclusion*/

#define WEBAPP_HTTP_SERVER_PORT 80				/**< HTTP port on which the web application will be running*/

#ifndef FS_FILE_EXTENSION_T_DEFINED
typedef void fs_file_extension;					/**< TODO: remove this*/
#endif

/**
 * @brief web-content file structure object
 */
typedef struct fs_file {
  const char *data;								/**< File data buffer*/
  int len;										/**< File data length*/
  int index;									/**< File data pointer index*/
  fs_file_extension *pextension;				/**< File extension TODO: see if this can be removed safely*/
  uint8_t flags;								/**< File flags*/
}fs_file_s;


/**
 * @brief web-content file linked list data structure object
 */
struct fsdata_file {
  const struct fsdata_file *next;				/**< Pointer to the next file object in the linked list*/
  const unsigned char *name;					/**< Name of the file*/
  const unsigned char *data;					/**< File data buffer*/
  int len;										/**< Length of data buffer*/
  uint8_t flags;								/**< FIle flags*/
};


void web_application_start(void);

#endif

/**
 * @} Grouping in web_application module ends
 **/
