CC := gcc
CFLAGS := -Wall -g
LIBS := -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

SOURCES := $(wildcard $(SRC_DIR)/*.c )
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all : tag state_machine_test

tag : $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

state_machine_test : $(BUILD_DIR)/stateMachine-test.o $(BUILD_DIR)/stateMachine.o $(BUILD_DIR)/inputStack.o
	$(CC) $(CFLAGS) -o state_machine_test $^ $(LIBS)

$(BUILD_DIR)/stateMachine-test.o : $(TEST_DIR)/stateMachine-test.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm tag state_machine_test $(BUILD_DIR)/*.o $(TEST_DIR)/*.c
