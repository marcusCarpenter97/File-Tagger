#include <stdlib.h>
#include "stateMachine.h"
#include "inputStack.h"

int main(int argc, char* argv[]) {

	initialize_stack(argc, argv);

	state_machine();	

	return EXIT_SUCCESS;
}
