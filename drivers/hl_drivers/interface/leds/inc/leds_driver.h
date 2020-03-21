#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#include "stm32f4xx_hal.h"

#define LED_DRIVER_COUNT 3
#define LED_DRIVER_PORT GPIOD
#define LED_DRIVER_BLINK_RATE 250

#define LED_DRIVER_DEFAULT {{}, LED_DRIVER_UNINITIALISED}

typedef enum{
	LED_DRIVER_UNINITIALISED,
	LED_DRIVER_INITIALISED
}led_driver_status_e;

typedef enum{
	LED_OFF,
	LED_ON,
	LED_BLINK
}led_driver_led_state_e;

typedef enum{
	LED_STATE_PIN 	 = GPIO_PIN_12,
	LED_ERROR_PIN	 = GPIO_PIN_14,
	LED_NETWORK_PIN	 = GPIO_PIN_15
}led_driver_pin_e;

typedef enum{
	LED_STATE,
	LED_ERROR,
	LED_NETWORK
}led_driver_led_e;


typedef struct{
	led_driver_pin_e pin;
	led_driver_led_state_e state;
}led_driver_led_s;

typedef struct{
	led_driver_led_s leds[LED_DRIVER_COUNT];
	led_driver_status_e status;
}led_driver_s;


void leds_driver_init(void);
void leds_driver_set(led_driver_led_e led, led_driver_led_state_e state);


#endif
