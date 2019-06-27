CC= gcc
CFLAGS+= -Wall -g

tag : main.o stateMachine.o inputStack.o
	cc -o tag main.o stateMachine.o inputStack.o

main.o : main.c
	cc -c main.c

stateMachine.o : stateMachine.c stateMachine.h
	cc -c stateMachine.c

inputStack.o : inputStack.c inputStack.h
	cc -c inputStack.c

clean :
	rm tag main.o stateMachine.o inputStack.o
