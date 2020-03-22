/**============================================================================================================================
 * Dependencies inclusion
 * Necessary dependencies should be declared here. Header file should contain as little dependecies declarations as possible
 *=============================================================================================================================*/

#include "main.h"
#include "cmsis_os.h"
#include "sys.h"


/**============================================================================================================================
 * Public functions definitions
 * These functions can be accessed outside of the file's scope
 * @see main.h for declarations
 *=============================================================================================================================*/

int main(void){

	osKernelInitialize();
	cueos_init();
	osKernelStart();

	for(;;); //Mandatory. this should never be reached

}
