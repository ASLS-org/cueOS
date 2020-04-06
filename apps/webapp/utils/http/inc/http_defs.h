/**
 * @ingroup HTTP
 * @defgroup HTTP_defs
 * Definition of fundamental constant values to be used within the HTTP module
 * @{
 */

#ifndef HTTP_DEFS_H_
#define HTTP_DEFS_H_

#define STR_COMMON_SP 	" "
#define STR_COMMON_CRLF "\r\n"
#define STR_COMMON_QMRK "?"
#define STR_COMMON_EQUL "="
#define STR_COMMON_AMPR	"&"

#define HTTP_HEADER_SEPARATOR 	"\r\n"
#define HTTP_HEADER_DELIMITOR 	"\r\n\r\n"
#define HTTP_PARAMS_DELIMITOR 	"?"
#define HTTP_PARAMS_TERMINATOR 	" "
#define HTTP_VALUE_SEPARATOR 	"="
#define HTTP_PARAMS_SEPARATOR 	"&"


/**
 * @brief Enumeration index of HTTP status codes.
 */
typedef enum{
	HTTP_STATUS_CODE_200,
	HTTP_STATUS_CODE_201,
	HTTP_STATUS_CODE_202,
	HTTP_STATUS_CODE_204,
	HTTP_STATUS_CODE_300,
	HTTP_STATUS_CODE_301,
	HTTP_STATUS_CODE_400,
	HTTP_STATUS_CODE_401,
	HTTP_STATUS_CODE_403,
	HTTP_STATUS_CODE_404,
	HTTP_STATUS_CODE_501,
	HTTP_STATUS_CODE_502,
	HTTP_STATUS_CODE_503,
}http_status_code_e;

/**
 * @brief Enumeration index of HTTP header fields.
 */
typedef enum{
	HTTP_HEADER_FIELD_STATUS_CODE,
	HTTP_HEADER_FIELD_CONTENT_TYPE,
	HTTP_HEADER_FIELD_CONTENT_LENGTH,
	HTTP_HEADER_FIELD_CONTENT_ENCODING,
	HTTP_HEADER_FIELD_CONTENT_LANGUAGE,
	HTTP_HEADER_FIELD_CONTENT_LOCATION
}http_header_field_e;

/**
 * @brief Enumeration index of HTTP content-types header values.
 */
typedef enum{
	HTTP_CONTENT_TYPE_PLAIN,
	HTTP_CONTENT_TYPE_HTML,
	HTTP_CONTENT_TYPE_JSON,
}http_content_types_e;

/**
 * @brief Enumeration index of HTTP method.
 */
typedef enum{
	HTTP_GET,
	HTTP_POST,
	HTTP_PUT,
	HTTP_PATCH,
	HTTP_DELETE
}http_method_e;

/**
 * @brief Enumeration index of current HTTP versions
 */
typedef enum{
	HTTP_VERSION_0_9,
	HTTP_VERSION_1_1,
	HTTP_VERSION_2_0
}http_version_e;

static char *const http_status_codes_str[] = {
	"200 OK",
	"201 Created",
	"Accepted",
	"No Content",
	"300 Multiple Choices",
	"301 Moved Permanently",
	"400 Bad Request",
	"401 Unauthorized",
	"403 Forbidden",
	"404 Not Found",
	"501 Not Implemented",
	"502 Bad Gateway",
	"503 Service Unavailable"
};

static char *const http_header_field_str[] = {
  "HTTP/1.1 ",
  "Content-Type: ",
  "Content-Length: ",
  "Content-Encoding: ",
  "Content-Language: ",
  "Content-Location: ",
};

static char *const http_content_types_str[] = {
  "text/plain",
  "text/html",
  "text/json",
};

static const char *const http_methods[] = {
	"GET",
	"POST",
	"PUT",
	"PATCH",
	"DELETE",
};

static const char *const http_versions[] = {
	"HTTP/0.9",
	"HTTP/1.1",
	"HTTP/2.0"
};

#endif

/**
 * @} Grouping in HTTP_defs submodule ends
 * @} Grouping in HTTP module ends
 **/
