#!/bin/bash

#Gets all directories necessary.
#Assumes this script is executed from the test dir.
TEST_DIR="$(pwd)"
echo $TEST_DIR
SOURCE_DIR=$(echo $TEST_DIR | rev | cut -d'/' -f2- | rev)
echo $SOURCE_DIR
BUILD_DIR="${SOURCE_DIR}/build/"
echo $BUILD_DIR

#Make build dir if necessary
mkdir "$BUILD_DIR"

#Find all .check files in test dir
for check_file in `ls *.check`
do
	echo $check_file

	#Remove .check extention and -test sufix
	file_name=$(echo "$check_file" | cut -f 1 -d '-')

	echo $file_name

	#Add .c extention
	file_name_c="${file_name}.c"

	echo $file_name_c

	#Get main C file
	implementation_file="$SOURCE_DIR/$file_name_c"

	echo $implementation_file

	#Make unit test .c file from .check file
	checkmk $check_file >$file_name_c

	#Compile unit test .c file with source code.
	gcc -Wall -o "$BUILD_DIR$file_name.run" "$implementation_file" "$file_name_c" -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

	#Run unit tests
	."/../build/$file_name.run"
done
