#include "DMX512_engine.h"
#include "DMX512_fixture.h"
#include "DMX512_defs.h"
#include "cmsis_os.h"

/** Private functions declaration */
static DMX512_engine_s *this;
static DMX512_engine_err_e DMX512_engine_patch(uint16_t id, uint16_t chStart, uint16_t chStop);
static DMX512_engine_err_e DMX512_engine_unpatch(uint16_t id);
static DMX512_engine_err_e DMX512_engine_checkPatch(uint16_t chStart, uint16_t chStop);
static uint16_t DMX512_engine_getFixtureIndex(uint16_t id);


DMX512_engine_s *DMX512_engine_init(void){

	this = pvPortMalloc(sizeof(DMX512_engine_s));

	this->_fixtureCount = 0;
//	this->_sceneCount 	= 0;
//	this->_effectCount	= 0;
	this->patch 		= DMX512_engine_patch;
	this->unpatch 		= DMX512_engine_unpatch;

	return this;
}

static DMX512_engine_err_e DMX512_engine_patch(uint16_t id, uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(err == DMX512_ENGINE_OK){
		err = DMX512_engine_checkPatch(chStart, chStop);
		if(err == DMX512_ENGINE_OK){
			DMX512_fixture_s fixture = DMX512_fixture_init(id , chStart, chStop);
			this->_fixtures = pvPortRealloc(this->_fixtures, sizeof(DMX512_fixture_s) * (this->_fixtureCount + 1));
			this->_fixtures[this->_fixtureCount] = fixture;
			this->_fixtureCount++;
		}
	}

	return err;

}

DMX512_engine_err_e DMX512_engine_unpatch(uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = DMX512_engine_getFixtureIndex(id);

	if(index >= 0){
		for(uint16_t i=0; i< this->_fixtureCount; i++){
			if(i >= index){
				this->_fixtures[i] = this->_fixtures[i+1];
			}
		}
		this->_fixtureCount--;
		this->_fixtures = pvPortRealloc(this->_fixtures, sizeof(DMX512_fixture_s) * (this->_fixtureCount));
	}else{
		err = DMX512_FIXTURE_UNKNW;
	}

	return err;

}

static DMX512_engine_err_e DMX512_engine_checkPatch(uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(chStop < chStart){
		err  = DMX512_CHANNEL_ADDRESS_INTERVAL_NEG;
	}else if(chStart >= DMX512_CHANNEL_ADDRESS_MIN && chStop <= DMX512_CHANNEL_ADDRESS_MAX){
		for(uint16_t i=0; i<this->_fixtureCount; i++){
			if(chStart <= this->_fixtures[i].chStop && this->_fixtures[i].chStart <= chStop){
				err = DMX512_CHANNEL_ADDRESS_DUP;
				break;
			}
		}
	}else{
		err  = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
	}

	return err;

}

uint16_t DMX512_engine_getFixtureIndex(uint16_t id){
	for(uint16_t i=0; i<this->_fixtureCount; i++){
		if(id == this->_fixtures[i].id){
			return id;
		}
	}
	return -1;
}

DMX512_fixture_s *DMX512_engine_getFixture(uint16_t id){
	for(uint16_t i=0; i<this->_fixtureCount; i++){
		if(id == this->_fixtures[i].id){
			return &this->_fixtures[i];
		}
	}
	return NULL;
}
