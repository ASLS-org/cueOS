#include "DMX512_patcher.h"
#include "DMX512_fixture.h"
#include "DMX512_defs.h"

static DMX512_patcher_s *this;
static DMX512_engine_err_e DMX512_patcher_checkPatch(DMX512_fixture_s *fixture);


DMX512_engine_err_e DMX512_patcher_patch(DMX512_fixture_s *fixture){

	DMX512_engine_err_e err = DMX512_patcher_checkPatch(fixture) ;

	if(err == DMX512_ENGINE_OK){
		this->fixtureCount += 1;
		this->fixtures = realloc(this->fixtures, this->fixtureCount * sizeof(DMX512_fixture_s));
		this->fixtures[this->fixtureCount] = fixture;
	}

	return err;

}

DMX512_engine_err_e DMX512_patcher_unpatch(uint16_t fixtureId){

	DMX512_engine_err_e err = DMX512_FIXTURE_NOT_FOUND;

	for(uint16_t i=0; i< sizeof(this->fixtures); i++){
		if(this->fixtures[i]->id == fixtureId){

			err = DMX512_ENGINE_OK;
			break;
		}
	}

	return err;

}

DMX512_engine_err_e DMX512_patcher_getFixtureInstance(DMX512_fixture_s *fixture, uint16_t fixtureId){

	DMX512_engine_err_e err = DMX512_FIXTURE_NOT_FOUND;

	for(uint16_t i=0; i< sizeof(this->fixtures); i++){
		if(this->fixtures[i]->id == fixtureId){
			err = DMX512_ENGINE_OK;
			break;
		}
	}

	return err;

}

static DMX512_engine_err_e DMX512_patcher_checkPatch(DMX512_fixture_s *fixture){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(fixture->chStart > DMX512_MIN_CHANNEL && fixture->chStop <= DMX512_MAX_CHANNEL){
		for(uint16_t i=0; i<sizeof(this->fixtures); i++){
			if(fixture->chStart <= this->fixtures[i]->chStop && this->fixtures[i]->chStart <= fixture->chStop){
				err = DMX512_CHN_IN_USE;
				break;
			}
		}
	}else{
		err  = DMX512_CHN_OUT_OF_BOUNDS;
	}

	return err;

}
