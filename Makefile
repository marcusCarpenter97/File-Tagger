CC := gcc
CFLAGS := -Wall -g

SRC_DIR := src
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.c )
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all : tag

tag : $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean :
	rm tag $(BUILD_DIR)/*.o
