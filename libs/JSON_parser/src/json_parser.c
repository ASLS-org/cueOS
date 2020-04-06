/***============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "json_parser.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Concatenates a provided string of character to a json string instance
 *
 * @param *json_string pointer to the JSON string instance to be changed
 * @param *data pointer to the string of character to be concatenated to the JSON string instance
 */
static void json_parser_json_string_cat(json_parser_json_string_s *json_string, char *data){
	uint16_t data_len = strlen(data);
	json_string->data = pvPortRealloc(json_string->data, json_string->len + data_len);
	memmove(json_string->data + json_string->len, data, data_len);
	json_string->len+=data_len;
}

/**
 * @brief Terminates json string instance by puting a NUL character into its last given index
 *
 * @param *json_string pointer to the JSON string instance to be terminated
 */
static void json_parser_json_string_terminate(json_parser_json_string_s *json_string){
	json_string->data = pvPortRealloc(json_string->data, json_string->len + 1);
	memmove(json_string->data + json_string->len, "\0", 1);
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see json_parser.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Puts a JSON key/(integer)value pair into a provided json string object;
 *
 * @param *json_string pointer to a json string instance
 * @param *key pointer to the JSON key string
 * @param val integer value to be associated to the provided JSON key
 * @param is_initial determines if a JSON string delimitor "{" should be prefixing the key/value pair. (the json string starts)
 * @param is_initial determines if a JSON string terminator "}\0" should be suffixing the key/value pair. (the json string stops)
 */
void json_parser_json_string_put_int_pair(json_parser_json_string_s *json_string, char *key, uint16_t val, uint8_t is_initial, uint8_t is_final){

	char val_str[20];
	itoa(val, val_str, 10);

	if(is_initial){
		json_parser_json_string_cat(json_string, JSON_STRING_DELIMITOR);
	}

	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, key);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_SEPARATOR);
	json_parser_json_string_cat(json_string, val_str);

	if(is_final){
		json_parser_json_string_cat(json_string, JSON_STRING_TERMINATOR);
		json_parser_json_string_terminate(json_string);
	}else{
		json_parser_json_string_cat(json_string, JSON_STRING_PAIR_SEPARATOR);
	}

}

/**
 * @brief Puts a JSON key/(string)value pair into a provided json string object;
 *
 * @param *json_string pointer to a json string instance
 * @param *key pointer to the JSON key string
 * @param *val pointer to the string value to be associated to the provided JSON key
 * @param is_initial determines if a JSON string delimitor "{" should be prefixing the key/value pair. (the json string starts)
 * @param is_initial determines if a JSON string terminator "}\0" should be suffixing the key/value pair. (the json string stops)
 */
void json_parser_json_string_put_str_pair(json_parser_json_string_s *json_string, char *key, char *val, uint8_t is_initial, uint8_t is_final){

	if(is_initial){
		json_parser_json_string_cat(json_string, JSON_STRING_DELIMITOR);
	}

	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, key);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_SEPARATOR);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, val);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);

	if(is_final){
		json_parser_json_string_cat(json_string, JSON_STRING_TERMINATOR);
		json_parser_json_string_terminate(json_string);
	}else{
		json_parser_json_string_cat(json_string, JSON_STRING_PAIR_SEPARATOR);
	}

}

/**
 * @brief Nests a child JSON string into a parent JSON string
 *
 * @param *json_string pointer to a json string instance
 * @param *key pointer to the JSON key string
 * @param *nested_json_string pointer to the json string value to be nested and associated to the provided JSON key
 * @param is_initial determines if a JSON string delimitor "{" should be prefixing the key/value pair. (the json string starts)
 * @param is_initial determines if a JSON string terminator "}\0" should be suffixing the key/value pair. (the json string stops)
 */
void json_parser_json_string_nest(json_parser_json_string_s *json_string, char *key, json_parser_json_string_s *nested_json_string, uint8_t is_initial, uint8_t is_final){

	if(is_initial){
		json_parser_json_string_cat(json_string, JSON_STRING_DELIMITOR);
	}

	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, key);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_DELIMITOR);
	json_parser_json_string_cat(json_string, JSON_STRING_ENTRY_SEPARATOR);
	json_parser_json_string_cat(json_string, nested_json_string->data);

	if(is_final){
		json_parser_json_string_cat(json_string, JSON_STRING_TERMINATOR);
		json_parser_json_string_terminate(json_string);
	}else{
		json_parser_json_string_cat(json_string, JSON_STRING_PAIR_SEPARATOR);
	}

}

/**
 * @brief Nests a child JSON string into a parent JSON array
 *
 * @param *json_string pointer to a json string instance (array)
 * @param *json_object pointer to the json string value to be nested into the json array
 * @param is_initial determines if a JSON array delimitor "[" should be prefixing the key/value pair. (the json string starts)
 * @param is_initial determines if a JSON array terminator "]\0" should be suffixing the key/value pair. (the json string stops)
 */
void json_parser_json_string_put_array_object(json_parser_json_string_s *json_string, json_parser_json_string_s *json_object, uint8_t is_initial, uint8_t is_final){

	if(is_initial){
		json_parser_json_string_cat(json_string, JSON_ARRAY_DELIMITOR);
	}

	json_parser_json_string_cat(json_string, json_object->data);

	if(is_final){
		json_parser_json_string_cat(json_string, JSON_ARRAY_TERMINATOR);
		json_parser_json_string_terminate(json_string);
	}else{
		json_parser_json_string_cat(json_string, JSON_STRING_PAIR_SEPARATOR);
	}

}

/**
 * @brief Retrieves key/value pairs from a JSON encoded string
 *
 * @param *json_string pointer to the JSON string to be parsed
 * @param json_string_len length (in bytes) of the provided JSON string (HTTP content_length header value)
 * @return json_parser_json_object_s* pointer to the parsed JSON object instance
 * @warning this is parser cannot (yet ?) parse JSON arrays. When provided with a JSON array, it will simply
 * 			chain the array values to the parser pairs as if the array did not exist
 */
json_parser_json_object_s *json_parser_parse(char *json_string, uint16_t json_string_len){

	json_parser_json_object_s *json_object = pvPortMalloc(sizeof(json_parser_json_object_s));
	json_object->params = pvPortMalloc(sizeof(json_parser_json_param_s));
	json_object->param_count = 0;

	char *key_start = NULL;
	char *key_end 	= NULL;
	char *val_start = NULL;
	char *val_end 	= NULL;

	if(json_string[0] == JSON_STRING_DELIMITOR[0] && json_string[json_string_len-1] == JSON_STRING_TERMINATOR[0] ){

		json_object->param_count = 0;
		json_string++;

		do{

			key_start = strnstr(json_string,JSON_STRING_ENTRY_DELIMITOR, json_string_len);
			key_end   = strnstr(key_start+1,JSON_STRING_ENTRY_DELIMITOR, json_string_len);
			val_start = strnstr(key_end+1 ,JSON_STRING_ENTRY_DELIMITOR, json_string_len);
			val_end   = strnstr(val_start+1,JSON_STRING_ENTRY_DELIMITOR, json_string_len);

			if(key_start != NULL && key_end != NULL && val_start !=NULL && val_end != NULL){

				uint8_t key_len = key_end - key_start;
				uint8_t val_len = val_end - val_start;

				json_object->params = pvPortRealloc(json_object->params, sizeof(json_parser_json_param_s) * ++json_object->param_count);

				json_object->params[json_object->param_count-1].key = pvPortMalloc(key_len * sizeof(char));
				json_object->params[json_object->param_count-1].val = pvPortMalloc(val_len * sizeof(char));

				memmove(json_object->params[json_object->param_count-1].key, key_start+1, key_len);
				memmove(json_object->params[json_object->param_count-1].val, val_start+1, val_len);

				json_object->params[json_object->param_count-1].key[key_len-1] = 0;
				json_object->params[json_object->param_count-1].val[val_len-1] = 0;

				json_string = val_end + 1;

			}

		}while(key_start != NULL && key_end != NULL && val_start !=NULL && val_end != NULL);
	}

	return json_object;

}


/**
 * @brief Frees a parsed json object
 *
 * @param *json_object pointer to json object instance to be freed
 */
void json_parser_free_json_object(json_parser_json_object_s *json_object){

	for(uint16_t i=0; i<json_object->param_count;i++){
		vPortFree(json_object->params[i].key);
		vPortFree(json_object->params[i].val);
		json_object->params[i].key = NULL;
		json_object->params[i].val = NULL;
	}

	vPortFree(json_object->params);
	vPortFree(json_object);

}

/**
 * @brief Creates and initialises a new json string instance
 *
 * @return json_parser_json_string_s * pointer to the created json string instance
 */
json_parser_json_string_s * json_parser_json_string_new(void){
	json_parser_json_string_s *json_string = pvPortMalloc(sizeof(json_parser_json_string_s));
	json_string->data = NULL;
	json_string->len = 0;
	return json_string;
}

/**
 * @brief Frees a json string instance
 *
 * @param *json_parser_json_string_s pointer to json string instance to be freed
 */
void json_parser_free_json_string(json_parser_json_string_s *json_string){
	vPortFree(json_string->data);
	vPortFree(json_string);
}
