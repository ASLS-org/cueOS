/**
 * @ingroup webapp_utils
 * @defgroup JSON_parser
 * Module dedicated to JSON parsing and string formating.
 * @{
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#define JSON_ARRAY_DELIMITOR 		"["
#define JSON_ARRAY_TERMINATOR 		"]"
#define JSON_STRING_DELIMITOR 		"{"
#define JSON_STRING_TERMINATOR 		"}"
#define JSON_STRING_ENTRY_DELIMITOR "\""
#define JSON_STRING_ENTRY_SEPARATOR ":"
#define JSON_STRING_PAIR_SEPARATOR 	","

/**
 * @brief JSON parameter structure object
 */
typedef struct{
	char *key;							/**< JSON parameter key string*/
	char *val;							/**< JSON parameter value string*/
}jsonparser_json_param_s;

/**
 * @brief JSON Objects structure object
 */
typedef struct{
	jsonparser_json_param_s *params;	/**< List of JSON parameter objects*/
	uint16_t param_count;				/**< Count of parameters contained within the JSON parameter list*/
}jsonparser_json_object_s;

/**
 * @brief JSON String structure object
 */
typedef struct{
	char *data;							/**< JSON string data buffer*/
	uint16_t len;						/**< JSON string data buffer length in bytes*/
}jsonparser_json_string_s;


void jsonparser_json_string_put_int_pair(jsonparser_json_string_s *json_string, char *key, uint16_t val, uint8_t is_initial, uint8_t is_final);
void jsonparser_json_string_put_str_pair(jsonparser_json_string_s *json_string, char *key, char *val, uint8_t is_final, uint8_t is_initial);
void jsonparser_json_string_nest(jsonparser_json_string_s *json_string, char *key, jsonparser_json_string_s *nested_json_string, uint8_t is_initial, uint8_t is_final);
void jsonparser_json_string_put_array_object(jsonparser_json_string_s *json_string, jsonparser_json_string_s *json_object, uint8_t is_initial, uint8_t is_final);
jsonparser_json_object_s *jsonparser_parse(char *json_string, uint16_t json_string_len);
jsonparser_json_string_s * jsonparser_json_string_new(void);
void jsonparser_free_json_object(jsonparser_json_object_s *json_object);
void jsonparser_free_json_string(jsonparser_json_string_s *json_string);

#endif

/**
 * @} Grouping in jsonparser submodule ends
 * @} Grouping in webapp_utils module ends
 */
