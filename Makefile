CC= gcc
CFLAGS+= -Wall -g

tag : main.o stateMachine.o inputStack.o
	$(CC) $(CFLAGS) -o tag main.o stateMachine.o inputStack.o

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

stateMachine.o : stateMachine.c stateMachine.h
	$(CC) $(CFLAGS) -c stateMachine.c

inputStack.o : inputStack.c inputStack.h
	$(CC) $(CFLAGS) -c inputStack.c

clean :
	rm tag main.o stateMachine.o inputStack.o
