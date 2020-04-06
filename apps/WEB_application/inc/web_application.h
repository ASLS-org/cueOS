/**
 * @ingroup apps
 * @defgroup web_application
 * Serves web-base user interface static files over HTTP to a remote device.
 * @{
 */

#ifndef WEBAPP_STATIC_H_
#define WEBAPP_STATIC_H_

#include <stdlib.h>
#include <stdint.h>


#define FS_READ_EOF     -1
#define FS_READ_DELAYED -2

#define FS_FILE_FLAGS_HEADER_INCLUDED     0x01
#define FS_FILE_FLAGS_HEADER_PERSISTENT   0x02
#define FS_FILE_FLAGS_HEADER_HTTPVER_1_1  0x04
#define FS_FILE_FLAGS_SSI                 0x08

#define WEBAPP_HTTP_SERVER_PORT 80

#ifndef FS_FILE_EXTENSION_T_DEFINED
typedef void fs_file_extension;
#endif

typedef struct fs_file {
  const char *data;
  int len;
  int index;
  fs_file_extension *pextension;
  uint8_t flags;
}fs_file_s;


struct fsdata_file {
  const struct fsdata_file *next;
  const unsigned char *name;
  const unsigned char *data;
  int len;
  uint8_t flags;
};


void webapp_start(void);

#endif

/**
 * @} Grouping in web_application module ends
 **/
