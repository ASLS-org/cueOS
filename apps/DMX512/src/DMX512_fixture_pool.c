//#include "DMX512_fixture_pool.h"
//#include "cmsis_os.h"
//
//DMX512_fixture_s *_hash_table[DMX512_FIXTURE_POOL_HASHTABLE_SIZE];
//static uint16_t _DMX512_fixture_pool_hash(uint16_t index);
//
//
//DMX512_fixture_s *DMX512_fixture_pool_get(uint16_t index){
//
//	uint16_t hash_index 		= _DMX512_fixture_pool_hash(index);
//	DMX512_fixture_s *current	= _hash_table[hash_index];
//
//	while(current != NULL){
//		if(current->id == index){
//			return current;
//		}
//		current = current->_next;
//	}
//
//	return NULL;
//
//}
//
//DMX512_engine_err_e DMX512_fixture_pool_add(uint16_t id, uint16_t chStart, uint16_t chStop){
//
//	DMX512_engine_err_e err 	= DMX512_ENGINE_OK;
//	DMX512_fixture_s *current 	= NULL;
//	uint16_t hash_index 		= _DMX512_fixture_pool_hash(id);
//	uint8_t i = 0;
//
////	while(err == DMX512_ENGINE_OK && i < DMX512_FIXTURE_POOL_HASHTABLE_SIZE){
////		current = _hash_table[hash_index];
////		while(current != NULL){
////			if(chStart <= current->chStop && current->chStart <= chStop){
////				err = DMX512_CHANNEL_ADDRESS_DUP;
////				break;
////			}
////			current = current->_next;
////		}
////		i++;
////	}
//
//	if(err == DMX512_ENGINE_OK){
//		if(chStart < DMX512_CHANNEL_ADDRESS_MIN || chStop > DMX512_CHANNEL_ADDRESS_MAX){
//			err = DMX512_CHANNEL_ADDRESS_OUT_OF_BOUNDS;
//		}else if(DMX512_fixture_pool_get(id) != NULL){
//			err = DMX512_FIXTURE_DUP;
//		}else{
//			DMX512_fixture_s *fixture = DMX512_fixture_init(id, chStart, chStop);
//			if(_hash_table[hash_index] == NULL){
//				_hash_table[hash_index] = fixture;
//			}else{
//				current = _hash_table[hash_index];
//				while(current->_next != NULL){
//					current = current->_next;
//				}
//				current->_next = fixture;
//			}
//		}
//	}
//
//	return err;
//
//}
//
//
//DMX512_engine_err_e DMX512_fixture_pool_clr(uint16_t index){
//
//	DMX512_engine_err_e err 	= DMX512_FIXTURE_UNKNW;
//	uint16_t hash_index 		= _DMX512_fixture_pool_hash(index);
//	DMX512_fixture_s *prev 		= NULL;
//	DMX512_fixture_s *current 	= _hash_table[hash_index];
//
//	while(current != NULL){
//		if(current->id == index){
//			err = DMX512_ENGINE_OK;
//			prev->_next = current->_next;
//			vPortFree(current);
//			break;
//		}
//		prev = current;
//		current = current->_next;
//	}
//
//	return err;
//
//}
//
//static uint16_t _DMX512_fixture_pool_hash(uint16_t index){
//	return index % (DMX512_FIXTURE_POOL_HASHTABLE_SIZE);
//}
