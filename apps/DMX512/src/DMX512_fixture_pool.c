#include"DMX512_fixture_pool.h"

/* Private variable declarations */
static DMX512_fixture_s *_instances;	//Dynamic array containing fixture instances
static size_t _count;					//Current size of the dynamic array

/* Private functions declarations */
static DMX512_engine_err_e _DMX512_fixture_pool_check(uint16_t id, uint16_t addr, uint16_t ch_stop);
static int16_t _DMX512_fixture_pool_search(uint16_t id);


/**
 * Adds a fixture instance into the pool
 *
 * @param id the fixture's idendifier
 * @param addr fixture's first channel address
 * @param ch_stop fixture's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_fixture_pool_add(uint16_t id, uint16_t addr, uint16_t ch_count){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(err == DMX512_ENGINE_OK){
		err = _DMX512_fixture_pool_check(id, addr, ch_count);
		if(err == DMX512_ENGINE_OK){
			DMX512_fixture_s fixture = DMX512_fixture_init(id , addr, ch_count);
			_instances = (DMX512_fixture_s*) pvPortRealloc(_instances, sizeof(DMX512_fixture_s) * (_count + 1));
			_instances[_count] = fixture;
			_count++;
		}
	}

	return err;

}

/**
 * Deletes a fixture instance from the pool
 *
 * @param id the fixture's idendifier
 * @return DMX512_engine_err_e error code following the function call
 */
DMX512_engine_err_e DMX512_fixture_pool_del(uint16_t id){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;
	int16_t index = _DMX512_fixture_pool_search(id);

	if(index >= 0){
		for(uint16_t i=0; i< _count; i++){
			if(i > index){
				_instances[i-1] = _instances[i];
			}
		}
		_count--;
		_instances = pvPortRealloc(_instances, sizeof(DMX512_fixture_s) * (_count));
	}else{
		err = DMX512_FIXTURE_UNKNW;
	}

	return err;

}

/**
 * Gets a fixture instance from the pool
 *
 * @param id the fixture's identifier
 * @return *DMX512_fixture_s pointer to the fixture instance
 */
DMX512_fixture_s *DMX512_fixture_pool_get(uint16_t id){
	int16_t index = _DMX512_fixture_pool_search(id);
	if(index >= 0){
		return &_instances[index];
	}else{
		return NULL;
	}

}

/**
 * Returns the whole instance array
 *
 */
DMX512_fixture_s *DMX512_fixture_pool_get_all(void){
	return _instances;
}

/**
 * Checks if a fixture can be added into the pool
 *
 * @param id the fixture's idendifier
 * @param addr fixture's first channel address
 * @param ch_stop fixture's last channel address
 * @return DMX512_engine_err_e error code following the function call
 */
static DMX512_engine_err_e _DMX512_fixture_pool_check(uint16_t id, uint16_t addr, uint16_t ch_count){

	DMX512_engine_err_e err = DMX512_ENGINE_OK;

	if(_DMX512_fixture_pool_search(id) >= 0){
		err  = DMX512_FIXTURE_DUP;
	}else if(addr < DMX512_CHANNEL_ADDRESS_MIN || ch_count + addr - 1 > DMX512_CHANNEL_ADDRESS_MAX){
		err  = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
	}else{
		for(uint16_t i=0; i<_count; i++){
			if(addr <= _instances[i].ch_stop && _instances[i].addr <=  ch_count + addr - 1){
				err = DMX512_CHANNEL_ADDRESS_DUP;
				break;
			}
		}
	}

	return err;

}

/**
 * Finds the array index of a fixture
 *
 * @param id the fixture's identifier
 * @return int16_t the array index of the fixture.
 * -1 is returned if the fixture couldn't be found
 */
static int16_t _DMX512_fixture_pool_search(uint16_t id){
	for(uint16_t i=0; i<_count; i++){
		if(id == _instances[i].id){
			return i;
		}
	}
	return -1;
}
