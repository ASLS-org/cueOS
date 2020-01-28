/*
 * ASLS_DMX512_fixture.h
 *
 *  Created on: 24 Jan 2020
 *      Author: root
 */

#ifndef ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_FIXTURE_H_
#define ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_FIXTURE_H_

#include "cmsis_os.h"

#define DMX512_CHN_MIN_ADDR  1
#define DMX512_CHN_MAX_ADDR  512
#define DMX512_CHN_MIN_VALUE 0
#define DMX512_CHN_MAX_VALUE 0

typedef enum{
	DMX512_OK,
	DMX512_CHN_OUT_OF_BOUNDS,
	DMX512_CHN_VAL_ERROR
}ASLS_DMX512_err;

typedef struct{
	uint16_t address;
	uint8_t value;
	ASLS_DMX512_err( *setValue)(void *channel, uint8_t value);
}ASLS_DMX512_channel;

typedef struct{
	char *name;
	uint16_t chStart;
	uint16_t chStop;
	uint16_t chCount;
	ASLS_DMX512_channel **channels;
}ASLS_DMX512_Fixture;

#endif /* ASLS_DRIVERS_INC_DMX512_ASLS_DMX512_FIXTURE_H_ */
