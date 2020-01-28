/*
 * ASLS_DMX512_effect.h
 *
 *  Created on: 24 Jan 2020
 *      Author: root
 */

#ifndef ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_EFFECT_H_
#define ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_EFFECT_H_

#include "ASLS_DMX512_fixture.h"

#define MS_VALUE  	0.001
#define TICK		1

typedef enum{
	SINE,
	SQUARE,
	TRIANGLE,
	SAWTOOTH
}ASLS_DMX512_effect_type;

typedef struct{
	ASLS_DMX512_effect_type type;
	ASLS_DMX512_channel *channel;
	float_t time;
	float_t frequency;
	uint8_t amplitude;
	uint8_t offset;
	uint8_t phase;
	osTimerId timerId;
	void (* function)();
}ASLS_DMX512_Effect;

#endif /* ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_EFFECT_H_ */
