/*
 * ASLS_DMX512_fxGenerator.c
 *
 *  Created on: 23 Jan 2020
 *      Author: root
 */

#include <math.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "ASLS_DMX512_effect.h"

#define DMX512_OFFSET 127
#define BPM_TO_HZ(bpm) (bpm/60)
extern uint16_t global_bpm_value;

static void ASLS_DMX512_effect_process(ASLS_DMX512_Effect *effect);
static void ASLS_DMX512_effect_sineWave(ASLS_DMX512_Effect *effect);

void ASLS_DMX512_effect_init(ASLS_DMX512_effect_type type, uint8_t amplitude, uint8_t offset, uint8_t phase){

	ASLS_DMX512_Effect *effect = malloc(sizeof(ASLS_DMX512_Effect));

	effect->type 		= type;
	effect->amplitude 	= amplitude;
	effect->offset		= offset;
	effect->phase		= phase;
	effect->frequency   = BPM_TO_HZ(global_bpm_value);

	osTimerDef(effect_timer, ASLS_DMX512_effect_process);
	effect->timerId = osTimerCreate(osTimer(effect_timer), osTimerPeriodic, effect);

}

static void ASLS_DMX512_effect_process(ASLS_DMX512_Effect *effect){
	effect->time += MS_VALUE;
	switch(effect->type){
		case SINE:
			ASLS_DMX512_effect_sineWave(effect);
			break;
		case SQUARE:
			//TODO: MAKE SQUARE WAVE GENERATOR
			break;
		case TRIANGLE:
			//TODO: MAKE TRIANGLE WAVE GENERATOR
			break;
		case SAWTOOTH:
			//TODO: MAKE SAWTOOTH WAVE GENERATOR
			break;
	}
}

static void ASLS_DMX512_effect_sineWave(ASLS_DMX512_Effect *effect){
	effect->channel->setValue(effect->channel, DMX512_OFFSET + effect->offset + effect->amplitude * sin(2 * M_PI * effect->frequency * effect->time + effect->phase));
}

void ASLS_DMX512_effect_start(ASLS_DMX512_Effect *effect){
	osTimerStart(effect->timerId, TICK);
}

