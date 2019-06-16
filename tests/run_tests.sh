#!/bin/bash

#Gets all necessary directories.
#This script MUST be executed from the test dir.
TEST_DIR="$(pwd)"
SOURCE_DIR=$(echo $TEST_DIR | rev | cut -d'/' -f2- | rev)
BUILD_DIR="${SOURCE_DIR}/build/"

#Make build dir if necessary
mkdir -p "$BUILD_DIR"

#Find all .check files in test dir
for check_file in `ls *.check`
do
	echo
	echo "Testing file: "$check_file

	#Remove .check extention and -test sufix
	file_name=$(echo "$check_file" | cut -f 1 -d '-')

	#Add .c extention
	file_name_c="${file_name}.c"

	#Get main C file
	implementation_file="$SOURCE_DIR/$file_name_c"

	#Make unit test .c file from .check file
	checkmk $check_file >$file_name_c

	#Compile unit test .c file with source code.
	gcc -Wall -o "$BUILD_DIR$file_name.test" "$implementation_file" "$file_name_c" -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

	#Run unit tests
	."/../build/$file_name.test"
done

echo
echo "Tests finished."
