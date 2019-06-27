#include <stdio.h>
#include "inputStack.h"

int top = EMPTY;

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
	}
}

int push(const char* item) {
	
	if (!is_full()) {
		input_stack[++top] = item;
	}
	else {
		printf("Error: could not push input_stack because it is full.\n");
	}
}