#include "DMX512_engine.h"
#include "DMX512_fixture.h"
#include "DMX512_defs.h"
#include "cmsis_os.h"

static DMX512_engine_s *this;
static DMX512_engine_err_e DMX512_engine_patch(uint16_t id, uint16_t chStart, uint16_t chStop);
static DMX512_engine_err_e DMX512_engine_unpatch(uint16_t id);
static DMX512_engine_err_e DMX512_engine_checkPatch(uint16_t chStart, uint16_t chStop);
static uint16_t DMX512_engine_getFixtureIndex(uint16_t fixtureId);

DMX512_engine_s *DMX512_engine_init(void){

	this = pvPortMalloc(sizeof(DMX512_engine_s));

	this->fixtureCount 	= 0;
	this->patch 		= DMX512_engine_patch;
	this->unpatch 		= DMX512_engine_unpatch;

	return this;
}

static DMX512_engine_err_e DMX512_engine_patch(uint16_t id, uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(err == DMX512_ENGINE_OK){
		err = DMX512_engine_checkPatch(chStart, chStop);
		if(err == DMX512_ENGINE_OK){
			DMX512_fixture_s *fixture = DMX512_fixture_init(id , chStart, chStop);
			this->fixtures = pvPortRealloc(this->fixtures, sizeof(DMX512_fixture_s) * (this->fixtureCount + 1));
			this->fixtures[this->fixtureCount] = fixture;
			this->fixtureCount++;
		}
	}

	return err;

}

DMX512_engine_err_e DMX512_engine_unpatch(uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	int16_t index = DMX512_engine_getFixtureIndex(id);

	if(index != DMX512_FIXTURE_UNKNW){

		DMX512_fixture_s **tmpFixtures = pvPortMalloc((this->fixtureCount) * sizeof(DMX512_fixture_s));

		for(uint16_t i=0; i< this->fixtureCount; i++){
			if(i < index){
				tmpFixtures[i] = this->fixtures[i];
			}else if(i > index){
				tmpFixtures[i] = this->fixtures[i+1];
			}
		}

		this->fixtureCount--;
		vPortFree(this->fixtures[index]);
		this->fixtures = pvPortRealloc(this->fixtures, sizeof(DMX512_fixture_s) * (this->fixtureCount));
		memcpy(this->fixtures, tmpFixtures, this->fixtureCount * sizeof(DMX512_fixture_s));
		this->fixtures[index]->free(this->fixtures[index]);
		vPortFree(tmpFixtures);

	}else{
		err = index;
	}

	return err;

}

static DMX512_engine_err_e DMX512_engine_checkPatch(uint16_t chStart, uint16_t chStop){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(chStart >= DMX512_CHANNEL_ADDRESS_MIN && chStop <= DMX512_CHANNEL_ADDRESS_MAX){
		for(uint16_t i=0; i<this->fixtureCount; i++){
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

	for(uint16_t i=0; i<this->fixtureCount; i++){
		if(fixtureId == this->fixtures[i]->id){
			return i;
		}
	}

	return DMX512_FIXTURE_UNKNW;

}
