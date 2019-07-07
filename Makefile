CC= gcc
CFLAGS+= -Wall -g
VPATH= src

tag : main.o stateMachine.o inputStack.o
	$(CC) $(CFLAGS) -o tag main.o stateMachine.o inputStack.o

main.o : main.c
	$(CC) $(CFLAGS) -c $<

stateMachine.o : stateMachine.c stateMachine.h
	$(CC) $(CFLAGS) -c $<

inputStack.o : inputStack.c inputStack.h
	$(CC) $(CFLAGS) -c $<

clean :
	rm tag main.o stateMachine.o inputStack.o
