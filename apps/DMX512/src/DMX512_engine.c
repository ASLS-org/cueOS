#include "DMX512_engine.h"
#include "DMX512_fixture.h"
#include "DMX512_defs.h"

static DMX512_engine_s *this;
static DMX512_engine_err_e DMX512_engine_patch(uint16_t chStart, uint16_t chStop);
static DMX512_engine_err_e DMX512_engine_unpatch(uint16_t fixtureId);
static DMX512_engine_err_e DMX512_engine_checkPatch(uint16_t chStart, uint16_t chStop);
static uint16_t DMX512_engine_getFixtureIndex(uint16_t fixtureId);


DMX512_engine_s *DMX512_engine_init(void){

	this->fixtureCount 	= 0;
	this->fixtures 		= NULL;
	this->patch 		= DMX512_engine_patch;
	this->unpatch 		= DMX512_engine_unpatch;

	return this;
}

static DMX512_engine_err_e DMX512_engine_patch(uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(err == DMX512_ENGINE_OK){
		err = DMX512_engine_checkPatch(chStart, chStop);
		if(err == DMX512_ENGINE_OK){
			DMX512_fixture_s *fixture = NULL;
			this->fixtureCount++;
			err = DMX512_fixture_init(fixture, this->fixtureCount , chStart, chStop);
			if(err == DMX512_ENGINE_OK){
				this->fixtures = realloc(this->fixtures, this->fixtureCount * sizeof(DMX512_fixture_s));
				this->fixtures[this->fixtureCount] = fixture;
			}else{
				err = DMX512_engine_unpatch(this->fixtureCount);
			}
		}
	}

	return err;

}

DMX512_engine_err_e DMX512_engine_unpatch(uint16_t fixtureId){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	int16_t index = DMX512_engine_getFixtureIndex(fixtureId);

	if(fixtureId != DMX512_FIXTURE_UNKNW){

		DMX512_fixture_s **tmpFixtures = malloc((this->fixtureCount - 1) * sizeof(DMX512_fixture_s));

		for(uint16_t i=0; i<this->fixtureCount; i++){
			if(i < index){
				tmpFixtures[i] = this->fixtures[i];
			}else{
				tmpFixtures[i] = this->fixtures[i+1];
			}
		}

		this->fixtureCount--;
		memcpy(this->fixtures, tmpFixtures, this->fixtureCount * sizeof(DMX512_fixture_s));
		this->fixtures[index]->free(this->fixtures[index]);
		free(tmpFixtures);

	}else{
		err = DMX512_PRESET_UNKNW;
	}

	return err;

}

static DMX512_engine_err_e DMX512_engine_checkPatch(uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(chStart > DMX512_CHANNEL_ADDRESS_MIN && chStop <= DMX512_CHANNEL_ADDRESS_MAX){
		for(uint16_t i=0; i<sizeof(this->fixtures); i++){
			if(chStart <= this->fixtures[i]->chStop && this->fixtures[i]->chStart <= chStop){
				err = DMX512_CHANNEL_ADDRESS_DUP;
				break;
			}
		}
	}else{
		err  = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
	}

	return err;

}

uint16_t DMX512_engine_getFixtureIndex(uint16_t fixtureId){

	for(uint16_t i=0; i< (sizeof(this->fixtures)/sizeof(DMX512_fixture_s)); i++){
		if(fixtureId == this->fixtures[i]->id){
			return i;
		}
	}

	return DMX512_FIXTURE_UNKNW;

}
