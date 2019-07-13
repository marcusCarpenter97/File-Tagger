#!/bin/bash

mkdir build

TEST_DIR="tests/"
SOURCE_DIR="src/"
BUILD_DIR="build/"

DOT_C=".c"
DOT_CHECK=".check"

CLEAN="clean"

CHOICE="$1"
EXIT="$2"

if [ "$CHOICE" = "clean" ]; then
	make clean
fi

if [ "$EXIT" = "only" ]; then
	exit 1
fi

echo "Creating unit tests using checkmk..."

#Find all .check files in test dir
for check_file in `ls $TEST_DIR*.check`
do
	#Substitute .check extention with .c 
	test_file=$(echo "${check_file/$DOT_CHECK/$DOT_C}")

	echo $test_file

	#Make unit test .c file from .check file
	checkmk $check_file >$test_file
done

echo "Done."

make

if [ "$CHOICE" = "tests" ]; then
	./state_machine_test
fi

