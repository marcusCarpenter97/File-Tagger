#include <stdio.h>
#include <stdlib.h>
#include "inputStack.h"

int top = EMPTY;

void initialize_stack(int stack_size, char* stack_items[]) {

	int stack_index;
	/* Add command line arguments in reverse order to stack without adding the programmes name. */
	for (stack_index = stack_size - 1; stack_index > 0; stack_index--) {
		push(stack_items[stack_index]);
	}
}

int is_empty() {
	if (top == EMPTY) {
		return 1;
	}
	else {
		return 0;
	}
}

int is_full() {
	if (top == MAX_SIZE) {
		return 1;
	} 
	else {
		return 0;
	}
}

const char* peek() {
	return input_stack[top];
}

const char* pop() {

	if (!is_empty()) {
		return input_stack[top--];
	}
	else {
		printf("Error: could not pop input_stack because it is empty.\n");
		return "error";
	}
}

void push(const char* item) {
	
	if (!is_full()) {
		input_stack[++top] = item;
	}
	else {
		printf("Error: could not push to input_stack because it is full.\n");
		exit(EXIT_FAILURE);
	}
}
