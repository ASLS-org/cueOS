/*
 * ASLS_DMX512_fixture.c
 *
 *  Created on: 23 Jan 2020
 *      Author: root
 */

#include <stdlib.h>
#include "ASLS_DMX512_fixture.h"

static ASLS_DMX512_err ASLS_DMX512_channel_setValue(ASLS_DMX512_channel *channel, uint8_t value);


ASLS_DMX512_err ASLS_DMX512_channel_init(ASLS_DMX512_channel *channel, uint16_t address){

	channel = malloc(sizeof(ASLS_DMX512_channel));
	ASLS_DMX512_err err = DMX512_OK;

	if(address > DMX512_CHN_MIN_ADDR && address < DMX512_CHN_MAX_ADDR){
		channel->address 	= address;
		channel->value 		= DMX512_CHN_MIN_VALUE;
		channel->setValue 	= ASLS_DMX512_channel_setValue;
	}else{
		err	= DMX512_CHN_OUT_OF_BOUNDS;
		free(channel);
	}

	return err;

}

static ASLS_DMX512_err ASLS_DMX512_channel_setValue(ASLS_DMX512_channel *channel, uint8_t value){
	ASLS_DMX512_err err = DMX512_OK;
	if( value >= 0 && value <= 255){
		channel->value = value;
	}else{
		err = DMX512_CHN_VAL_ERROR;
	}
	return err;
}


ASLS_DMX512_err ASLS_DMX512_fixture_init(ASLS_DMX512_Fixture *fixture, char *name, uint16_t chStart, uint16_t chStop){

	fixture = malloc(sizeof(ASLS_DMX512_Fixture));
	ASLS_DMX512_err err = DMX512_OK;

	fixture->name 		= name;
	fixture->chStart 	= chStart;
	fixture->chStop 	= chStop;
	fixture->chCount 	= chStop - chStart + 1;
	fixture->channels 	= malloc(sizeof(ASLS_DMX512_channel) * fixture->chCount);

	for(int i=0; i< chStart - chStop; i++){
		err = ASLS_DMX512_channel_init(fixture->channels[i], chStart + i);
		if(err != DMX512_OK){
			free(fixture);
			break;
		}
	}

	return err;

}


