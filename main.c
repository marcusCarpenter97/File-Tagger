#include <stdio.h>
#include "fileTagger.c"
#include "inputStack.c"

int main(int argc, char* argv[]) {

	verify_input();

	int stack_state = is_empty();
	printf("\n%d\n", stack_state);

	push("Hello");
	push("World");
	push("Test");
	push("stack");
	push("ok");
	push("full");

	stack_state = is_full();
	printf("%d\n", stack_state);

	const char* ret = peek();
	printf("%s\n", ret);

	stack_state = is_empty();
	printf("%d\n", stack_state);
	
	pop();
	pop();
	pop();
	pop();
	pop();
	pop();

	stack_state = is_empty();
	printf("%d\n", stack_state);
	
	return 0;
}
