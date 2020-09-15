/**
 * @ingroup HTTP_server
 * @defgroup HTTP_defs Definitions
 * Definition of fundamental constant values to be used within the HTTP module
 * @{
 */

#ifndef HTTP_DEFS_H_
#define HTTP_DEFS_H_

#define STR_COMMON_SP 	" "					/**< common string separator value - Space */
#define STR_COMMON_CRLF "\r\n"				/**< common string separator value - CRLF */
#define STR_COMMON_QMRK "?"					/**< common string separator value - Question mark */
#define STR_COMMON_EQUL "="					/**< common string separator value - Equal sign */
#define STR_COMMON_AMPR	"&"					/**< common string separator value - Ampersand */


/**
 * @brief Enumeration of HTTP status codes string indexes.
 */
typedef enum{
	HTTP_STATUS_CODE_200,					/**< HTTP 200 OK status code*/
	HTTP_STATUS_CODE_201,					/**< HTTP 201 status code*/
	HTTP_STATUS_CODE_202,					/**< HTTP 202 status code*/
	HTTP_STATUS_CODE_204,					/**< HTTP 204 status code*/
	HTTP_STATUS_CODE_300,					/**< HTTP 300 status code*/
	HTTP_STATUS_CODE_301,					/**< HTTP 301 status code*/
	HTTP_STATUS_CODE_400,					/**< HTTP 400 status code*/
	HTTP_STATUS_CODE_401,					/**< HTTP 401 status code*/
	HTTP_STATUS_CODE_403,					/**< HTTP 403 status code*/
	HTTP_STATUS_CODE_404,					/**< HTTP 404 Not Found status code*/
	HTTP_STATUS_CODE_405,					/**< HTTP 405 Method Not Allowed*/
	HTTP_STATUS_CODE_501,					/**< HTTP 501 status code*/
	HTTP_STATUS_CODE_502,					/**< HTTP 502 status code*/
	HTTP_STATUS_CODE_503,					/**< HTTP 503 status code*/
}http_status_code_e;

/**
 * @brief Enumeration of HTTP header fields string indexes.
 */
typedef enum{
	HTTP_HEADER_FIELD_STATUS_CODE,			/**< HTTP status code header field string index*/
	HTTP_HEADER_FIELD_CONTENT_TYPE,			/**< HTTP content type header field string index*/
	HTTP_HEADER_FIELD_CONTENT_LENGTH,		/**< HTTP content length header field string index*/
	HTTP_HEADER_FIELD_CONTENT_ENCODING,		/**< HTTP content encoding header field string index*/
	HTTP_HEADER_FIELD_CONTENT_LANGUAGE,		/**< HTTP content manguage header field string index*/
	HTTP_HEADER_FIELD_CONTENT_LOCATION		/**< HTTP content location header field string index*/
}http_header_field_e;

/**
 * @brief Enumeration of HTTP content-types header values string indexes.
 */
typedef enum{
	HTTP_CONTENT_TYPE_PLAIN,				/**< HTTP Plain content type header value string index*/
	HTTP_CONTENT_TYPE_HTML,					/**< HTTP HTML content type header value string index*/
	HTTP_CONTENT_TYPE_JSON,					/**< HTTP JSON content type header value string index*/
}http_content_types_e;

/**
 * @brief Enumeration of HTTP method string indexes.
 */
typedef enum{
	HTTP_GET,								/**< HTTP GET method string index*/
	HTTP_POST,								/**< HTTP POST method string index*/
	HTTP_PUT,								/**< HTTP PUT method string index*/
	HTTP_PATCH,								/**< HTTP PATCH method string index*/
	HTTP_DELETE								/**< HTTP DELETE method string index*/
}http_method_e;

/**
 * @brief Enumeration of HTTP versions string indexes.
 */
typedef enum{
	HTTP_VERSION_0_9,						/**< HTTP/0.9 version string index*/
	HTTP_VERSION_1_1,						/**< HTTP/1.1 version string index*/
	HTTP_VERSION_2_0						/**< HTTP/2.0 version string index*/
}http_version_e;


/**
 * @brief list of HTTP status codes strings.
 */
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
	"405 Method Not Allowed",
	"501 Not Implemented",
	"502 Bad Gateway",
	"503 Service Unavailable"
};

/**
 * @brief list of HTTP header fields strings.
 */
static char *const http_header_field_str[] = {
  "HTTP/1.1 ",
  "Content-Type: ",
  "Content-Length: ",
  "Content-Encoding: ",
  "Content-Language: ",
  "Content-Location: ",
};

/**
 * @brief list of HTTP content-types header values strings.
 */
static char *const http_content_types_str[] = {
  "text/plain",
  "text/html",
  "application/json; charset=UTF8",
};

/**
 * @brief list of HTTP method strings.
 */
static const char *const http_methods[] = {
	"GET",
	"POST",
	"PUT",
	"PATCH",
	"DELETE",
};

/**
 * @brief list of HTTP versions strings.
 */
static const char *const http_versions[] = {
	"HTTP/0.9",
	"HTTP/1.1",
	"HTTP/2.0"
};

#endif

/**
 * @} Grouping in HTTP_defs submodule ends
 **/
