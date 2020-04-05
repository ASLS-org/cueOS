#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#include "stm32f4xx_hal.h"

#define LED_DRIVER_COUNT 3				/** @ingroup leds_driver @def LED_DRIVER_COUNT @brief the amount of leds supported by the driver*/
#define LED_DRIVER_PORT GPIOD			/** @ingroup leds_driver @def LED_DRIVER_PORT @brief GPIO port over which these LEDs should be controlled*/
#define LED_DRIVER_BLINK_RATE 250		/** @ingroup leds_driver @def LED_DRIVER_BLINK_RATE @brief Blink rate of a driver LED in ms */

/**
 * @ingroup leds_driver
 * @def LED_DRIVER_DEFAULT
 * @brief leds driver instance default values
 *
 * Used during initialisation of a leds driver instance, it lowers the risk of conflicts
 * by ensuring that every parameters which will be set are correctly initialised.
 */
#define LED_DRIVER_DEFAULT {{}, LED_DRIVER_UNINITIALISED}


/**
 * @ingroup leds_driver
 * @enum led_driver_status_e
 * @brief Status of the leds driver instance
 *
 * Defines whether or not the leds driver instance has been correctly initialised.
 */
typedef enum{
	LED_DRIVER_UNINITIALISED,				/**< The driver is initialised*/
	LED_DRIVER_INITIALISED					/**< The driver is uninitialised*/
}led_driver_status_e;

/**
 * @ingroup leds_driver
 * @enum led_driver_led_state_e
 * @brief defines the running mode of one of the driver's LED instance
 */
typedef enum{
	LED_OFF,									/**< Sets an LED in OFF mode*/
	LED_ON,										/**< Sets an LED in ON mode*/
	LED_BLINK									/**< Sets an LED in blink mode*/
}led_driver_led_state_e;

/**
 * @ingroup leds_driver
 * @enum led_driver_pin_e
 * @brief defines the driver's LED pin numbers
 */
typedef enum{
	LED_STATE_PIN 	 = GPIO_PIN_12,				/**< Pin number of the driver's "STATE" LED*/
	LED_ERROR_PIN	 = GPIO_PIN_14,				/**< Pin number of the driver's "ERROR" LED*/
	LED_NETWORK_PIN	 = GPIO_PIN_15				/**< Pin number of the driver's "NETWORK" LED*/
}led_driver_pin_e;

/**
 * @ingroup leds_driver
 * @enum led_driver_led_e
 * @brief defines index of the driver's LED to be used
 * 		  to select a specific led from the driver's led pool
 */
typedef enum{
	LED_STATE,									/**< Index of the driver's "STATE" LED*/
	LED_ERROR,									/**< Index of the driver's "ERROR" LED*/
	LED_NETWORK									/**< Index of the driver's "NETWORK" LED*/
}led_driver_led_e;


/**
 * @ingroup leds_driver
 * @struct led_driver_led_s
 * @brief Defines a driver's LED instance
 */
typedef struct{
	led_driver_pin_e pin;						/**< The LED's pin number @see led_driver_pin_e */
	led_driver_led_state_e state;				/**< The LED's running mode @see led_driver_led_state_e*/
}led_driver_led_s;

/**
 * @ingroup leds_driver
 * @struct led_driver_s
 * @brief Defines the leds driver instance
 */
typedef struct{
	led_driver_led_s leds[LED_DRIVER_COUNT];	/**< List of LED instances */
	led_driver_status_e status;					/**< current status of the driver @see led_driver_status_e*/ //TODO: THis is old and not really needed, maybe remove ?
}led_driver_s;


void leds_driver_init(void);
void leds_driver_set(led_driver_led_e led, led_driver_led_state_e state);


#endif
