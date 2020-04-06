/**
* @ingroup DMX512_engine
* @defgroup DMX512_defs
* Definition of fundamental constant values to be used by the DMX512 engine
* @{
*/

#ifndef DMX512_defs_H
#define DMX512_defs_H

#define DMX512_CHANNEL_ADDRESS_MIN 0		/** @ingroup DMX512_defs @def DMX512_CHANNEL_ADDRESS_MIN @brief DMX512 min channel address*/
#define DMX512_CHANNEL_ADDRESS_MAX 512		/** @ingroup DMX512_defs @def DMX512_CHANNEL_ADDRESS_MAX @brief DMX512 max channel address*/
#define DMX512_CHANNEL_VALUE_MIN   0		/** @ingroup DMX512_defs @def DMX512_CHANNEL_VALUE_MIN @brief DMX512 min channel value*/
#define DMX512_CHANNEL_VALUE_MAX   255		/** @ingroup DMX512_defs @def DMX512_CHANNEL_VALUE_MAX @brief DMX512 max channel addvalueress*/

/**
 * @brief DMX512 engine error index constants
 *
 * Series of pre-defined constant error values to be used
 * throughout the whole DMX512 engine lifecycle.
 */
typedef enum{
	DMX512_ENGINE_OK,							/**< No error*/
	DMX512_ENGINE_INSTANCE_DUPLICATE 	 = -1,	/**< A duplicate id was found*/
	DMX512_ENGINE_INSTANCE_INVALID	 	 = -2,	/**< Provided arguments are not valid*/
	DMX512_ENGINE_INSTANCE_UNDEFINED	 = -3,	/**< The instance id could not be found*/
	DMX512_ENGINE_CONFIG_DUMP_EXCEPTION	 = -4,	/**< An error occurred during configuration load process*/
	DMX512_ENGINE_CONFIG_LOAD_EXCEPTION	 = -5,	/**< An error occurted during configuration save process*/
	DMX512_ENGINE_MEMORY_OVERFLOW		 = -6,	/**< Not enough memory*/
	DMX512_ENGINE_API_ENDPOINT_UNDEF	 = -7,	/**< Requested API endpoint does not exist (this shouldn't be defined here)*/
	DMX512_ENGINE_API_METHOD_UNDEF	 	 = -8,	/**< Requested API endpoint does not exist (this shouldn't be defined here)*/
}DMX512_engine_err_e;

/**
 * @brief DMX512 engine error strings constants
 * TODO: this should be put into DMX512 API
 */
static char *const DMX512_engine_errs_str[] = {
	"Success",
	"Ressource id already exist",
	"Wrong parameter values",
	"Ressource id not found",
	"Load config error",
	"Save config error",
	"API endoint doesn't exist",
	"Unauthorised method",
};

#endif

/**
 * @} Grouping in DMX512_defs submodule ends
 * @} Grouping in DMX512_engine module ends
 **/
