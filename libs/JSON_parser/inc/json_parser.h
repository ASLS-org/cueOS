/**
 * @ingroup libs
 * @defgroup JSON_parser JSON Parser
 * Module dedicated to JSON parsing and JSON string formating.
 * @{
 */

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#define JSON_ARRAY_DELIMITOR 		"["		/**< JSON string array delimitor*/
#define JSON_ARRAY_TERMINATOR 		"]"		/**< JSON string array terminator*/
#define JSON_STRING_DELIMITOR 		"{"		/**< JSON string object delimitor*/
#define JSON_STRING_TERMINATOR 		"}"		/**< JSON string object delimitor*/
#define JSON_STRING_ENTRY_DELIMITOR "\""	/**< JSON string entry delimitor*/
#define JSON_STRING_ENTRY_SEPARATOR ":"		/**< JSON string entry delimitor*/
#define JSON_STRING_PAIR_SEPARATOR 	","		/**< JSON string pair delimitor*/

/**
 * @brief JSON parameter structure object
 */
typedef struct{
	char *key;								/**< JSON parameter key string*/
	char *val;								/**< JSON parameter value string*/
}json_parser_json_param_s;

/**
 * @brief JSON Objects structure object
 */
typedef struct{
	json_parser_json_param_s *params;		/**< List of JSON parameter objects*/
	uint16_t param_count;					/**< Count of parameters contained within the JSON parameter list*/
}json_parser_json_object_s;

/**
 * @brief JSON String structure object
 */
typedef struct{
	char *data;								/**< JSON string data buffer*/
	uint16_t len;							/**< JSON string data buffer length in bytes*/
}json_parser_json_string_s;


void json_parser_json_string_put_int_pair(json_parser_json_string_s *json_string, char *key, uint16_t val, uint8_t is_initial, uint8_t is_final);
void json_parser_json_string_put_str_pair(json_parser_json_string_s *json_string, char *key, char *val, uint8_t is_final, uint8_t is_initial);
void json_parser_json_string_nest(json_parser_json_string_s *json_string, char *key, json_parser_json_string_s *nested_json_string, uint8_t is_initial, uint8_t is_final);
void json_parser_json_string_put_array_object(json_parser_json_string_s *json_string, json_parser_json_string_s *json_object, uint8_t is_initial, uint8_t is_final);
json_parser_json_object_s *json_parser_parse(char *json_string, uint16_t json_string_len);
json_parser_json_string_s * json_parser_json_string_new(void);
void json_parser_free_json_object(json_parser_json_object_s *json_object);
void json_parser_free_json_string(json_parser_json_string_s *json_string);

#endif

/**
 * @} Grouping in json_parser submodule ends
 */
