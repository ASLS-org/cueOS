/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "cmsis_os.h"
#include "leds_driver.h"

//TODO: check this, it was made in a hurry and it seems quite shitty, could be greatly improved in a minimal amount of time

/***============================================================================================================================
 * Private variables definitions
 * These variables are only accessible from within the file's scope
 *=============================================================================================================================*/

static led_driver_s this = LED_DRIVER_DEFAULT;
static const led_driver_pin_e LED_PINS[LED_DRIVER_COUNT] = {
	LED_STATE_PIN,
	LED_ERROR_PIN,
	LED_NETWORK_PIN
};


/***============================================================================================================================
 * Private functions definitions
 * These functions are only accessible from within the file's scope
 *=============================================================================================================================*/

/**
 * @brief Initialises a LED's GPIO
 *
 * @param pin the LED's GPIO pin number
 * @return led_driver_led_sthe created LED instance
 */
led_driver_led_s _leds_driver_init_led(led_driver_pin_e pin){

	led_driver_led_s led = {pin, LED_OFF};

	GPIO_InitTypeDef led_init = {0};
	HAL_GPIO_WritePin(LED_DRIVER_PORT, pin, GPIO_PIN_RESET);

	led_init.Pin   = pin;
	led_init.Mode  = GPIO_MODE_OUTPUT_PP;
	led_init.Pull  = GPIO_NOPULL;
	led_init.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(LED_DRIVER_PORT, &led_init);

	return led;

}

/**
 * @brief LED driver thread which periodically refreshes LED's according to state
 *
 * @param *arg required by cmsis_os osThreadNew prototype. not used here.
 */
static void _leds_driver_manage(void *arg){
	for(;;){
		for(uint8_t i=0; i< LED_DRIVER_COUNT; i++){
			switch(this.leds[i].state){
				case LED_ON:
					HAL_GPIO_WritePin(LED_DRIVER_PORT, this.leds[i].pin, LED_ON);
					break;
				case LED_OFF:
					HAL_GPIO_WritePin(LED_DRIVER_PORT, this.leds[i].pin, LED_OFF);
					break;
				case LED_BLINK:
					HAL_GPIO_TogglePin(LED_DRIVER_PORT, this.leds[i].pin);
					break;
			}
		}
		osDelay(LED_DRIVER_BLINK_RATE);
	}
}


/***============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see leds_driver.h for declarations
 *=============================================================================================================================*/

/**
 * @brief LED Driver initialisation
 */
void leds_driver_init(void){

	if(this.status == LED_DRIVER_UNINITIALISED){

		for(uint8_t i=0; i< LED_DRIVER_COUNT; i++){
			this.leds[i] = _leds_driver_init_led(LED_PINS[i]);
		}

		osThreadNew(_leds_driver_manage, NULL, NULL);

		this.status = LED_DRIVER_INITIALISED;

	}

}

/**
 * @brief Assigns a state to a driver's led
 *
 * @param led the led which state is to be set
 * @param state the selected state
 * @see leds_driver.h for further information regarding led states
 */
void leds_driver_set(led_driver_led_e led, led_driver_led_state_e state){
	this.leds[led].state = state;
}


