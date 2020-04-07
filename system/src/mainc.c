/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "main.h"
#include "cueos.h"


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see main.h for declarations
 *=============================================================================================================================*/

/**
 * @ingroup system
 * @fn main
 * @brief Main function called at device startup
 */
int main(void){
	cueos_start();
	for(;;); //Mandatory. this should never be reached
}
