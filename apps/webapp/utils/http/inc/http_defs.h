#ifndef HTTP_DEFS_H_
#define HTTP_DEFS_H_

#define HTTP_HEADER_SEPARATOR "\r\n"
#define HTTP_HEADER_DELIMITOR "\r\n\r\n"
#define HTTP_PARAMS_SEPARATOR "&"
#define HTTP_PARAMS_DELIMITOR "?"

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

typedef enum{
	HTTP_HEADER_FIELD_STATUS_CODE,
	HTTP_HEADER_FIELD_CONTENT_TYPE,
	HTTP_HEADER_FIELD_CONTENT_LENGTH,
	HTTP_HEADER_FIELD_CONTENT_ENCODING,
	HTTP_HEADER_FIELD_CONTENT_LANGUAGE,
	HTTP_HEADER_FIELD_CONTENT_LOCATION
}http_header_field_e;

typedef enum{
	HTTP_CONTENT_TYPE_PLAIN,
	HTTP_CONTENT_TYPE_HTML,
	HTTP_CONTENT_TYPE_JSON,
}http_content_types_e;

typedef enum{
	HTTP_GET,
	HTTP_POST,
	HTTP_PUT,
	HTTP_PATCH,
	HTTP_DELETE
}http_method_e;


static char *const http_status_codes_str[] = {
	"200 OK"
	"201 Created"
	"Accepted"
	"No Content"
	"300 Multiple Choices"
	"301 Moved Permanently"
	"400 Bad Request"
	"401 Unauthorized"
	"403 Forbidden"
	"404 Not Found"
	"501 Not Implemented"
	"502 Bad Gateway"
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
	"GET ",
	"POST ",
	"PUT ",
	"PATCH ",
	"DELETE ",
};

#endif