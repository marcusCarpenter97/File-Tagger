#ifndef INPUTSTACK_H
#define INPUTSTACK_H

#define MAX_SIZE 5 
#define EMPTY -1

const char *input_stack[MAX_SIZE];

extern int top;

void initialize_stack(int stack_size, char* stack_items[]);
int is_empty();
int is_full();
const char* peek();
const char* pop();
void push(const char* item);

#endif /* INPUTSTACK_H */
