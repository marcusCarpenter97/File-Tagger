#include <stdlib.h>
#include "stateMachine.h"
#include "inputStack.h"
#include "databaseManager.h"

int main(int argc, char* argv[]) {

	initialize_stack(argc, argv);
	
	initialize_database();
	
	state_machine();	

	return EXIT_SUCCESS;
}
