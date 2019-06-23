#include <stdio.h>
#include "stateMachine.c"
#include "inputStack.c"

int initialize_stack(int stack_size, char* stack_items[]) {

	int stack_index;

	for (stack_index = stack_size - 1; stack_index >= 0; stack_index--) {
		push(stack_items[stack_index]);
	}
}

int main(int argc, char* argv[]) {

	initialize_stack(argc, argv);
	
	return 0;
}
