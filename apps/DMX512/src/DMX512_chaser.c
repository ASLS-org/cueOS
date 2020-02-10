#include "DMX512_chaser.h"

static DMX512_engine_err_e DMX512_chaser_addScene(DMX512_chaser_s *this, DMX512_scene_s *scene);
static DMX512_engine_err_e DMX512_chaser_clrScene(DMX512_chaser_s *this, uint16_t id);
static int16_t DMX512_chaser_getSceneIndex(DMX512_chaser_s *this, uint16_t sceneId);
static void  DMX512_chaser_trigger(DMX512_chaser_s *this);

DMX512_chaser_s *DMX512_chaser_init(DMX512_chaser_s *this, uint16_t id){

	this = pvPortMalloc(sizeof(DMX512_chaser_s));
	this->id 			= id;
	this->sceneCount	= 0;
	this->add	 		= DMX512_chaser_addScene;
	this->clr	 		= DMX512_chaser_clrScene;
	this->trigger 		= DMX512_chaser_trigger;

	return this;

}


static DMX512_engine_err_e DMX512_chaser_addScene(DMX512_chaser_s *this, DMX512_scene_s *scene){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(DMX512_chaser_getSceneIndex(this, scene->id) == DMX512_SCENE_UNKNW){
		this->sceneCount++;
		this->scenes = pvPortRealloc(this->scenes, this->sceneCount * sizeof(DMX512_scene_s));
		this->scenes[this->sceneCount] = scene;
	}else{
		err = DMX512_SCENE_DUP;
	}

	return err;

}

static DMX512_engine_err_e DMX512_chaser_clrScene(DMX512_chaser_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	int16_t index = DMX512_chaser_getSceneIndex(this,id);

	if(id != DMX512_PRESET_NOT_FOUND){

		DMX512_preset_s **tmpScenes = pvPortMalloc((this->sceneCount - 1) * sizeof(DMX512_scene_s));

		for(uint16_t i=0; i<this->sceneCount; i++){
			if(i < index){
				tmpScenes[i] = this->scenes[i];
			}else{
				tmpScenes[i] = this->scenes[i+1];
			}
		}

		this->scenes--;
		memcpy(this->scenes, tmpScenes, this->sceneCount * sizeof(DMX512_scene_s));
		free(tmpScenes);

	}else{
		err = DMX512_PRESET_UNKNW;
	}

	return err;

}

static void  DMX512_chaser_trigger(DMX512_chaser_s *this){
	for(uint16_t i=0; i< this->sceneCount; i++){
		this->scenes[i]->trigger(this->scenes[i]);
	}
}

static int16_t DMX512_chaser_getSceneIndex(DMX512_chaser_s *this, uint16_t sceneId){

	for(uint16_t i=0; i< (sizeof(this->scenes)/sizeof(DMX512_preset_s)); i++){
		if(sceneId == this->scenes[i]->id){
			return i;
		}
	}

	return DMX512_SCENE_UNKNW;

}

