/***============================================================================================================================
 * Depedencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "DMX512_engine.h"
#include "http_server.h"
#include "http_defs.h"
#include "http_response.h"
#include "http_request.h"
#include "json_parser.h"
#include "DMX512_rest_api.h"


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Retrieves one or many fixture patched within the DMX512 engine's
 *
 * @param *req pointer to the http request instance for retrieving uri parameters
 * @see http_server.h for further information relative to http requests parameter
 * 		parsing and retrieval.
 */
static void _DMX512_rest_api_get_fixtures(http_request_s *req){

	DMX512_engine_err_e err;

	uint8_t endpoint_str_len = strlen(DMX512_rest_api_endpoints_str[DMX512_API_ENDPOINT_PATCH]);

	char *id_str = strnstr(req->uri + endpoint_str_len, "/", strlen(req->uri) - endpoint_str_len);

	json_parser_json_string_s *res_body = json_parser_json_string_new();

	if(id_str != NULL){

		char *garbage = NULL;

		uint16_t id = strtoul(id_str + 1, &garbage, 10);

		DMX512_fixture_s *fixture;

		err = garbage[0] == 0 ? DMX512_engine_patch_get(id, &fixture) : DMX512_ENGINE_INSTANCE_INVALID;

		if(err == DMX512_ENGINE_OK){
			json_parser_json_string_s *json_fixture = json_parser_json_string_new();
			json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ID], fixture->id, 1, 0);
			json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ADDRESS], fixture->addr, 0, 0);
			json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_CHANNEL_COUNT], fixture->ch_count, 0, 1);
			json_parser_json_string_nest(res_body, "fixture", json_fixture, 1, 0);
			json_parser_json_string_put_int_pair(res_body, "errcode", err, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(json_fixture);
			json_parser_free_json_string(res_body);
		}else{
			json_parser_json_string_put_str_pair(res_body, "msg", DMX512_engine_errs_str[-err], 1, 0);
			json_parser_json_string_put_int_pair(res_body, "errcode", -err, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);
		}


	}else{

		uint16_t start = 0;
		uint16_t stop = 0;

		for(uint8_t i=0; i<req->param_count; i++){
			if(!strncmp(req->params[i].arg, DMX512_rest_api_args_str[DMX512_API_PARAM_START],
				strlen(DMX512_rest_api_args_str[DMX512_API_PARAM_START]))){
				start = atoi(req->params[i].val);
			}else if(!strncmp(req->params[i].arg, DMX512_rest_api_args_str[DMX512_API_PARAM_STOP],
				strlen(DMX512_rest_api_args_str[DMX512_API_PARAM_STOP]))){
				stop = atoi(req->params[i].val);
			}
		}

		DMX512_fixture_pool_s *fixture_pool = DMX512_engine_patch_get_all();

		if(fixture_pool->fixture_count > 0 && start < stop){

			json_parser_json_string_s *json_fixture_array = json_parser_json_string_new();

			for(uint i=start; i<stop; i++){
				if(i < fixture_pool->fixture_count){
					json_parser_json_string_s *json_fixture = json_parser_json_string_new();
					json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ID], fixture_pool->fixtures[i].id, 1, 0);
					json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ADDRESS], fixture_pool->fixtures[i].addr, 0, 0);
					json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_CHANNEL_COUNT], fixture_pool->fixtures[i].ch_count, 0, 1);
					json_parser_json_string_put_array_object(
							json_fixture_array, json_fixture,
							i == start ? 1 : 0, i == stop-1 ||
							i == fixture_pool->fixture_count-1 ?
							1 : 0
					);
					json_parser_free_json_string(json_fixture);
				}
			}

			json_parser_json_string_nest(res_body, "fixtures", json_fixture_array, 1, 0);
			json_parser_free_json_string(json_fixture_array);
			json_parser_json_string_put_int_pair(res_body, "errcode", -DMX512_ENGINE_OK, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_200, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);

		}else{

			json_parser_json_string_put_str_pair(res_body, "msg", DMX512_engine_errs_str[-DMX512_ENGINE_INSTANCE_INVALID], 1, 0);
			json_parser_json_string_put_int_pair(res_body, "errcode", -DMX512_ENGINE_INSTANCE_INVALID, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_400, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);

		}

	}

}

/**
 * @brief Tries patching a fixture into the DMX512 engine fixture pool.
 *
 * @param *req pointer to the http request instance for retrieving uri parameters
 * @see http_server.h for further information relative to http requests parameter
 * 		parsing and retrieval.
 */
static void _DMX512_rest_api_add_fixture(http_request_s *req){

	DMX512_engine_err_e err;

	uint16_t fixture_id   = DMX512_CHANNEL_ADDRESS_MAX + 1;
	uint16_t fixture_addr = DMX512_CHANNEL_ADDRESS_MAX + 1;
	uint16_t fixture_ch	  = DMX512_CHANNEL_ADDRESS_MAX + 1;

	json_parser_json_object_s *json_data = json_parser_parse(req->content, req->content_length);
	json_parser_json_string_s *res_body  = json_parser_json_string_new();

	for(uint8_t i=0; i<json_data->param_count;i++){
		if(strcmp(json_data->params[i].key, DMX512_rest_api_args_str[DMX512_API_PARAM_ID]) == 0){
			fixture_id = atoi(json_data->params[i].val);
		}else if(strcmp(json_data->params[i].key, DMX512_rest_api_args_str[DMX512_API_PARAM_ADDRESS]) == 0){
			fixture_addr = atoi(json_data->params[i].val);
		}else if(strcmp(json_data->params[i].key, DMX512_rest_api_args_str[DMX512_API_PARAM_CHANNEL_COUNT]) == 0){
			fixture_ch = atoi(json_data->params[i].val);
		}
	}

	err = DMX512_engine_patch_add(fixture_id, fixture_addr, fixture_ch);

	if(err == DMX512_ENGINE_OK){
		json_parser_json_string_s *json_fixture = json_parser_json_string_new();
		json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ID], fixture_id, 1, 0);
		json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ADDRESS], fixture_addr, 0, 0);
		json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_CHANNEL_COUNT], fixture_ch, 0, 1);
		json_parser_json_string_nest(res_body, "fixture", json_fixture, 1, 0);
		json_parser_json_string_put_int_pair(res_body, "errcode", err, 0, 1);
		http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
		json_parser_free_json_string(json_fixture);
		json_parser_free_json_string(res_body);
	}else{
		json_parser_json_string_put_str_pair(res_body, "msg", DMX512_engine_errs_str[-err], 1, 0);
		json_parser_json_string_put_int_pair(res_body, "errcode", -err, 0, 1);
		http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
		json_parser_free_json_string(res_body);
	}

	json_parser_free_json_object(json_data);

}

/**
 * @brief Retrieves one or many scenes added to the DMX512 engine's
 *
 * @param *req pointer to the http request instance for retrieving uri parameters
 * @see http_server.h for further information relative to http requests parameter
 * 		parsing and retrieval.
 */
static void _DMX512_rest_api_get_scenes(http_request_s *req){

	DMX512_engine_err_e err;

	uint8_t endpoint_str_len = strlen(DMX512_rest_api_endpoints_str[DMX512_API_ENDPOINT_SCENES]);

	char *id_str = strnstr(req->uri + endpoint_str_len, "/", strlen(req->uri) - endpoint_str_len);

	json_parser_json_string_s *res_body = json_parser_json_string_new();

	if(id_str != NULL){

		char *garbage = NULL;

		uint16_t id = strtoul(id_str + 1, &garbage, 10);

		DMX512_scene_s *scene;

		err = garbage[0] == 0 ? DMX512_engine_scene_get(id, &scene) : DMX512_ENGINE_INSTANCE_INVALID;

		if(err == DMX512_ENGINE_OK){

			json_parser_json_string_s *json_scene = json_parser_json_string_new();

			json_parser_json_string_put_int_pair(json_scene, DMX512_rest_api_args_str[DMX512_API_PARAM_ID], scene->id, 1, 0);
			json_parser_json_string_put_int_pair(json_scene, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEIN_TIME], scene->fadein_time, 0, 0);
			json_parser_json_string_put_int_pair(json_scene, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEOUT_TIME], scene->fadeout_time, 0, 0);

			json_parser_json_string_s *json_fixture_preset_array = json_parser_json_string_new();


			if(scene->fixture_preset_count>0){
				for(uint16_t i=0; i<scene->fixture_preset_count; i++){
					json_parser_json_string_put_array_int(
							json_fixture_preset_array,
							scene->fixture_presets[i].fixture->id,
							i==0 ? 1:0,
							i== scene->fixture_preset_count - 1 ? 1 : 0
					);
				}
			}else{
				json_parser_json_string_put_array_object(json_fixture_preset_array,NULL,1,1);
			}

			json_parser_json_string_nest(json_scene, "fixture_presets", json_fixture_preset_array, 0, 1);
			json_parser_free_json_string(json_fixture_preset_array);
			json_parser_json_string_nest(res_body, "scene", json_scene, 1, 0);
			json_parser_free_json_string(json_scene);
			json_parser_json_string_put_int_pair(res_body, "errcode", err, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);
		}else{
			json_parser_json_string_put_str_pair(res_body, "msg", DMX512_engine_errs_str[-err], 1, 0);
			json_parser_json_string_put_int_pair(res_body, "errcode", -err, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);
		}


	}else{

		uint16_t start = 0;
		uint16_t stop = 0;

		for(uint8_t i=0; i<req->param_count; i++){
			if(!strncmp(req->params[i].arg, DMX512_rest_api_args_str[DMX512_API_PARAM_START],
				strlen(DMX512_rest_api_args_str[DMX512_API_PARAM_START]))){
				start = atoi(req->params[i].val);
			}else if(!strncmp(req->params[i].arg, DMX512_rest_api_args_str[DMX512_API_PARAM_STOP],
				strlen(DMX512_rest_api_args_str[DMX512_API_PARAM_STOP]))){
				stop = atoi(req->params[i].val);
			}
		}

		DMX512_scene_pool_s *scene_pool = DMX512_engine_scene_get_all();

		if(scene_pool->scene_count > 0 && start < stop){

			json_parser_json_string_s *json_scenes_array = json_parser_json_string_new();

			for(uint i=start; i<stop; i++){
				if(i < scene_pool->scene_count){
					json_parser_json_string_s *json_scene = json_parser_json_string_new();
					json_parser_json_string_put_int_pair(json_scene, DMX512_rest_api_args_str[DMX512_API_PARAM_ID], scene_pool->scenes[i].id, 1, 0);
					json_parser_json_string_put_int_pair(json_scene, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEIN_TIME], scene_pool->scenes[i].fadein_time, 0, 0);
					json_parser_json_string_put_int_pair(json_scene, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEOUT_TIME], scene_pool->scenes[i].fadeout_time, 0, 1);
					json_parser_json_string_put_array_object(
							json_scenes_array, json_scene,
							i == start ? 1 : 0, i == stop-1 ||
							i == scene_pool->scene_count-1 ?
							1 : 0
					);
					json_parser_free_json_string(json_scene);
				}
			}

			json_parser_json_string_nest(res_body, "scenes", json_scenes_array, 1, 0);
			json_parser_free_json_string(json_scenes_array);
			json_parser_json_string_put_int_pair(res_body, "errcode", -DMX512_ENGINE_OK, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_200, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);

		}else{

			json_parser_json_string_put_str_pair(res_body, "msg", DMX512_engine_errs_str[-DMX512_ENGINE_INSTANCE_INVALID], 1, 0);
			json_parser_json_string_put_int_pair(res_body, "errcode", -DMX512_ENGINE_INSTANCE_INVALID, 0, 1);
			http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_400, HTTP_CONTENT_TYPE_JSON, res_body->data);
			json_parser_free_json_string(res_body);

		}

	}

}

/**
 * @brief Tries patching a fixture into the DMX512 engine fixture pool.
 *
 * @param *req pointer to the http request instance for retrieving uri parameters
 * @see http_server.h for further information relative to http requests parameter
 * 		parsing and retrieval.
 */
static void _DMX512_rest_api_add_scene(http_request_s *req){

	DMX512_engine_err_e err;

	uint16_t scene_id   	= DMX512_CHANNEL_ADDRESS_MAX + 1;
	uint16_t scene_fadein	= DMX512_CHANNEL_ADDRESS_MAX + 1;
	uint16_t scene_fadeout	= DMX512_CHANNEL_ADDRESS_MAX + 1;

	json_parser_json_object_s *json_data = json_parser_parse(req->content, req->content_length);
	json_parser_json_string_s *res_body  = json_parser_json_string_new();

	for(uint8_t i=0; i<json_data->param_count;i++){
		if(strcmp(json_data->params[i].key, DMX512_rest_api_args_str[DMX512_API_PARAM_ID]) == 0){
			scene_id = atoi(json_data->params[i].val);
		}else if(strcmp(json_data->params[i].key, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEIN_TIME]) == 0){
			scene_fadein = atoi(json_data->params[i].val);
		}else if(strcmp(json_data->params[i].key, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEOUT_TIME]) == 0){
			scene_fadeout = atoi(json_data->params[i].val);
		}
	}

	err = DMX512_engine_scene_add(scene_id, scene_fadein, scene_fadeout);

	if(err == DMX512_ENGINE_OK){
		json_parser_json_string_s *json_fixture = json_parser_json_string_new();
		json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_ID], scene_id, 1, 0);
		json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEIN_TIME], scene_fadein, 0, 0);
		json_parser_json_string_put_int_pair(json_fixture, DMX512_rest_api_args_str[DMX512_API_PARAM_FADEOUT_TIME], scene_fadeout, 0, 1);
		json_parser_json_string_nest(res_body, "fixture", json_fixture, 1, 0);
		json_parser_json_string_put_int_pair(res_body, "errcode", err, 0, 1);
		http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
		json_parser_free_json_string(json_fixture);
		json_parser_free_json_string(res_body);
	}else{
		json_parser_json_string_put_str_pair(res_body, "msg", DMX512_engine_errs_str[-err], 1, 0);
		json_parser_json_string_put_int_pair(res_body, "errcode", -err, 0, 1);
		http_response_prepare_dynamic(req->res, HTTP_STATUS_CODE_404, HTTP_CONTENT_TYPE_JSON, res_body->data);
		json_parser_free_json_string(res_body);
	}

	json_parser_free_json_object(json_data);

}

static void _DMX512_rest_api_configure(http_request_s *req){
	char *content = req->content;
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_rest_api.h for declarations
 *=============================================================================================================================*/

/**
 * @brief Parses the current HTTP request method and URI in order to route requests to a
 * specific api function.
 *
 * @param *req pointer to the http request instance for retrieving uri parameters
 * @see http_server.h for further information relative to http requests parameter
 * 		parsing and retrieval.
 */
void DMX512_rest_api_router(http_request_s *req){

	DMX512_rest_api_endpoint_e endpoint;

	for(endpoint=DMX512_API_ENDPOINT_PATCH; endpoint<DMX512_API_ENDPOINT_DEFAULT; endpoint++){
		if(!strncmp(req->uri, DMX512_rest_api_endpoints_str[endpoint], strlen(DMX512_rest_api_endpoints_str[endpoint]))){
			break;
		}
	}

	switch(endpoint){
		case DMX512_API_ENDPOINT_PATCH:
			switch(req->method){
				case HTTP_GET: 	_DMX512_rest_api_get_fixtures(req); break;
				case HTTP_POST: _DMX512_rest_api_add_fixture(req);	break;
				default:											break;
			}
			break;
		case DMX512_API_ENDPOINT_SCENES:
			switch(req->method){
			case HTTP_GET: 	_DMX512_rest_api_get_scenes(req); 		break;
			case HTTP_POST: _DMX512_rest_api_add_scene(req);		break;
				default:											break;
			}
			break;
		case DMX512_API_ENDPOINT_CONFIGURE:
			switch(req->method){
				case HTTP_POST: _DMX512_rest_api_configure(req);	break;
				default:											break;
			}
			break;
		default:
			break;
	}
}
