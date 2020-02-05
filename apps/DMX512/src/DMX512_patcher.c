#include "DMX512_patcher.h"
#include "DMX512_fixture.h"
#include "DMX512_defs.h"

DMX512_patcher_s *patcher;

static DMX512_engine_err_e DMX512_patcher_checkPatch(DMX512_fixture_s *fixture);
DMX512_engine_err_e DMX512_patcher_patch(DMX512_fixture_s *fixture);
DMX512_engine_err_e DMX512_patcher_unpatch(uint16_t fixtureId);
DMX512_engine_err_e DMX512_patcher_getFixtureInstance(DMX512_fixture_s *fixture, uint16_t fixtureId);


DMX512_engine_err_e DMX512_patcher_patch(DMX512_fixture_s *fixture){

	DMX512_engine_err_e err = DMX512_patcher_checkPatch(fixture) ;

	if(err == DMX512_ENGINE_OK){
		patcher->fixtureCount += 1;
		patcher->fixtures = realloc(patcher->fixtures, patcher->fixtureCount * sizeof(DMX512_fixture_s));
		patcher->fixtures[patcher->fixtureCount] = fixture;
	}

	return err;

}

DMX512_engine_err_e DMX512_patcher_unpatch(uint16_t fixtureId){

	DMX512_engine_err_e err = DMX512_FIXTURE_NOT_FOUND;

	for(uint16_t i=0; i< sizeof(patcher->fixtures); i++){
		if(patcher->fixtures[i]->id == fixtureId){

			err = DMX512_ENGINE_OK;
			break;
		}
	}

	return err;

}

DMX512_engine_err_e DMX512_patcher_getFixtureInstance(DMX512_fixture_s *fixture, uint16_t fixtureId){

	DMX512_engine_err_e err = DMX512_FIXTURE_NOT_FOUND;

	for(uint16_t i=0; i< sizeof(patcher->fixtures); i++){
		if(patcher->fixtures[i]->id == fixtureId){
			err = DMX512_ENGINE_OK;
			break;
		}
	}

	return err;

}

static DMX512_engine_err_e DMX512_patcher_checkPatch(DMX512_fixture_s *fixture){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(fixture->chStart > DMX512_MIN_CHANNEL && fixture->chStop <= DMX512_MAX_CHANNEL){
		for(uint16_t i=0; i<sizeof(patcher->fixtures); i++){
			if(fixture->chStart <= patcher->fixtures[i]->chStop && patcher->fixtures[i]->chStart <= fixture->chStop){
				err = DMX512_CHN_IN_USE;
				break;
			}
		}
	}else{
		err  = DMX512_CHN_OUT_OF_BOUNDS;
	}

	return err;

}
