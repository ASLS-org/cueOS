/**============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include <string.h>
#include "cmsis_os.h"
#include "jsonparser.h"


/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see jsonparser.h for declarations
 *=============================================================================================================================*/

//jsonparser_json_object_s *jsonparser_parse(char *json_string, uint16_t content_length){
//
//	jsonparser_json_object_s *json_data = (jsonparser_json_object_s*)pvPortMalloc(sizeof(jsonparser_json_object_s));
//
//	return json_data;
//
//}
//
//void jsonparser_free(jsonparser_json_object_s *json_object){
//	vPortFree(json_object);
//}

//FIXME: Crashes only when receiving 32 bytes strings.  WTF is wrong with 32 bytes strings ??????
jsonparser_json_object_s *jsonparser_parse(char *json_string, uint16_t json_string_len){

	jsonparser_json_object_s *json_object = pvPortMalloc(sizeof(jsonparser_json_object_s));
	json_object->params = pvPortMalloc(sizeof(jsonparser_json_param_s));
	json_object->param_count = 0;

	char *arg_start = NULL;
	char *arg_end 	= NULL;
	char *val_start = NULL;
	char *val_end 	= NULL;

	if(json_string[0] == JSON_STRING_DELIMITOR && json_string[json_string_len-1] == JSON_STRING_TERMINATOR ){

		json_object->param_count = 0;
		json_string++;

		do{

			arg_start = strnstr(json_string,JSON_STRING_ENTRY_DELIMITOR, json_string_len);
			arg_end   = strnstr(arg_start+1,JSON_STRING_ENTRY_DELIMITOR, json_string_len);
			val_start = strnstr(arg_end+1 ,JSON_STRING_ENTRY_DELIMITOR, json_string_len);
			val_end   = strnstr(val_start+1,JSON_STRING_ENTRY_DELIMITOR, json_string_len);

			if(arg_start != NULL && arg_end != NULL && val_start !=NULL && val_end != NULL){

				uint8_t arg_len = arg_end - arg_start;
				uint8_t val_len = val_end - val_start;

				json_object->params = pvPortRealloc(json_object->params, sizeof(jsonparser_json_param_s) * ++json_object->param_count);

				json_object->params[json_object->param_count-1].arg = pvPortMalloc(arg_len * sizeof(char));
				json_object->params[json_object->param_count-1].val = pvPortMalloc(val_len * sizeof(char));

				memmove(json_object->params[json_object->param_count-1].arg, arg_start+1, arg_len);
				memmove(json_object->params[json_object->param_count-1].val, val_start+1, val_len);

				json_object->params[json_object->param_count-1].arg[arg_len-1] = 0;
				json_object->params[json_object->param_count-1].val[val_len-1] = 0;

				json_string = val_end + 1;

			}

		}while(arg_start != NULL && arg_end != NULL && val_start !=NULL && val_end != NULL);
	}

	return json_object;

}

void jsonparser_free(jsonparser_json_object_s *json_object){

	for(uint16_t i=0; i<json_object->param_count;i++){
		vPortFree(json_object->params[i].arg);
		vPortFree(json_object->params[i].val);
		json_object->params[i].arg = NULL;
		json_object->params[i].val = NULL;
	}

	vPortFree(json_object->params);
	vPortFree(json_object);

//	json_object->params 	 = NULL;
//	json_object->param_count = 0;
//	json_object 			 = NULL;

}
