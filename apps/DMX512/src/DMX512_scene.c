#include "DMX512_scene.h"
#include "DMX512_fixture_pool.h"
#include "DMX512_driver.h"
#include "cmsis_os.h"

/**============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * Finds the array index of a fixture
 *
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_scene_search(DMX512_scene_s *this, uint16_t fixture_id){
	for(uint16_t i=0; i<this->fp_count; i++){
		if(fixture_id == this->fp_instances[i].fixture->id){
			return i;
		}
	}
	return -1;
}

//TODO: enable RTC for fade time calculations ?

static void _DMX512_scene_fadein(void *arg){
	uint16_t time = *((uint16_t*)arg);
	float val;
//	for(uint16_t i=0; i<this->fp_count; i++){
//		for(uint16_t j=0; j<this->fp_instances[i].ch_count; j++){
//			uint16_t addr = this->fp_instances[i].fixture->addr + this->fp_instances[i].channels[j];
//			uint16_t value = this->fp_instances[i].values[j];
//			if(this->fadein_time > 0){
//				val = ((float)value/(float)this->fadein_time)*((float)this->tick_count);
//				DMX512_driver_set_single(addr, (uint16_t)(val));
//			}else{
//				DMX512_driver_set_single(addr, value);
//			}
//		}
//	}
}

static void _DMX512_scene_fadeout(DMX512_scene_s *this){
	for(uint16_t i=0; i<this->fp_count; i++){
		for(uint16_t j=0; j<this->fp_instances[i].ch_count; j++){
			uint16_t addr = this->fp_instances[i].fixture->addr + this->fp_instances[i].channels[j];
			uint16_t value = this->fp_instances[i].values[j];
			float val = ((float)value/(float)this->fadeout_time)*(float)this->tick_count + value;
			DMX512_driver_set_single(addr, (uint16_t)(val));
		}
	}
	this->tick_count++;
}

void DMX512_scene_start(DMX512_scene_s *this){
	osTimerDef(TestTimer, _DMX512_scene_fadein);
	osTimerId test = osTimerCreate(osTimer(TestTimer), osTimerPeriodic, &this->fadein_time);
	osTimerStart(test, 1);
}

void DMX512_scene_manage(DMX512_scene_s *this){
	switch(this->state){
		case DMX512_SCENE_FADEIN:
			if(this->tick_count > this->fadein_time){
				this->tick_count = 0; this->state = DMX512_SCENE_IDLE;
			}
			else{ _DMX512_scene_fadein(this); this->tick_count++; }
			break;
		case DMX512_SCENE_FADEOUT:
			if(this->tick_count > this->fadeout_time){ this->tick_count = 0; this->state = DMX512_SCENE_IDLE; }
			else{ _DMX512_scene_fadeout(this); this->tick_count++; }
			break;
		case DMX512_SCENE_IDLE:
			this->tick_count = 0;
			break;
	}
}


//TODO: Implement scene fade-in fade-out

/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see DMX512_scene.h for declarations
 *=============================================================================================================================*/

/**
 * Creates and initialises a new scene instance
 *
 * @param id the scene identifier (@see DMX512_scene_pool.h)
 * @param fadein_time the scene fade-in time in ms
 * @param fadeout_time the scene fade-out time in ms
 * @return DMX512_scene_s the created scene
 */
DMX512_scene_s DMX512_scene_init(uint16_t id, uint16_t fadein_time, uint16_t fadeout_time){
	DMX512_scene_s scene;
	scene.id = id;
	scene.fadein_time = fadein_time;
	scene.fadeout_time = fadeout_time;
	scene.fp_instances = NULL;
	scene.fp_count = 0;
	scene.state = DMX512_SCENE_IDLE;
	scene.tick_count = 0;
	return scene;
}

/**
 * Adds a fixture preset instance into the scene
 *
 * @param this pointer to the scene instance
 * @param id the fixture's idendifier
 * @param values the list of values to be stored within the scene
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_add(DMX512_scene_s *this, DMX512_fixture_preset_s fp){

	DMX512_engine_err_e err   = DMX512_ENGINE_OK;

	if(fp.status == DMX512_FIXTURE_PRESET_UNINITIALISED){
		err = DMX512_INVALID_FIXTURE_PRESET;
	}else if(_DMX512_scene_search(this, fp.fixture->id) >= 0){
		err = DMX512_DUPLICATE_FIXTURE_PRESET;
	}else{
		this->fp_instances = (DMX512_fixture_preset_s*) pvPortRealloc(this->fp_instances, sizeof(DMX512_fixture_preset_s) * (this->fp_count + 1));
		this->fp_instances[this->fp_count] = fp;
		this->fp_count++;
	}

	return err;

}

/**
 * Deletes a fixture preset instance from the scene
 *
 * @param id the fixture preset's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_scene_del(DMX512_scene_s *this, uint16_t id){

	DMX512_engine_err_e err = DMX512_INVALID_FIXTURE_PRESET;
	int16_t index = _DMX512_scene_search(this, id);

	if(index >= 0){
		for(uint16_t i=index+1; i< this->fp_count; i++){
			this->fp_instances[i-1] = this->fp_instances[i];
		}
		this->fp_count--;
		vPortFree(this->fp_instances[index].values);
		this->fp_instances = pvPortRealloc(this->fp_instances, sizeof(DMX512_fixture_s) * (this->fp_count));
		err = DMX512_ENGINE_OK;
	}

	return err;

}

/**
 * Gets a fixture instance from the pool
 *
 * @param id the fixture preset identifier
 * @return *DMX512_fixture_preset_s pointer to the fixture preset instance
 */
DMX512_fixture_preset_s *DMX512_scene_get(DMX512_scene_s *this, uint16_t id){
	int16_t index = _DMX512_scene_search(this, id);
	if(index >= 0){
		return &this->fp_instances[index];
	}else{
		return NULL;
	}
}

