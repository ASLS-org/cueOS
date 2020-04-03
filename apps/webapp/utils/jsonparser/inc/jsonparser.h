#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#define JSON_STRING_DELIMITOR '{'
#define JSON_STRING_TERMINATOR '}'
#define JSON_STRING_ENTRY_DELIMITOR "\""

typedef struct{
	char *arg;
	char *val;
}jsonparser_json_param_s;

typedef struct{
	jsonparser_json_param_s *params;
	uint16_t param_count;
}jsonparser_json_object_s;

jsonparser_json_object_s *jsonparser_parse(char *json_string, uint16_t json_string_len);
void jsonparser_free(jsonparser_json_object_s *json_object);

#endif
