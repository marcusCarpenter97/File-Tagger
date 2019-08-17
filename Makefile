CC := gcc
CFLAGS := -Wall -g
LIBS := -pthread -lcheck_pic -pthread -lrt -lm -lsubunit -lsqlite3

SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

SOURCES := $(wildcard $(SRC_DIR)/*.c )
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all : tag state_machine_test database_manager_test

tag : $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lsqlite3

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

state_machine_test : $(BUILD_DIR)/stateMachine-test.o $(BUILD_DIR)/stateMachine.o $(BUILD_DIR)/inputStack.o $(BUILD_DIR)/databaseManager.o
	$(CC) $(CFLAGS) -o state_machine_test $^ $(LIBS)

$(BUILD_DIR)/stateMachine-test.o : $(TEST_DIR)/stateMachine-test.c
	$(CC) $(CFLAGS) -c $< -o $@

database_manager_test : $(BUILD_DIR)/databaseManager-test.o $(BUILD_DIR)/databaseManager.o
	$(CC) $(CFLAGS) -o database_manager_test $^ $(LIBS)

$(BUILD_DIR)/databaseManager-test.o : $(TEST_DIR)/databaseManager-test.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm tag state_machine_test database_manager_test $(BUILD_DIR)/*.o $(TEST_DIR)/*.c
