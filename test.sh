#!/bin/bash
# Author: Ian Wu
# Test script for copy program

TEST_DATA='Here is text that is in a file'

# Compile the program
gcc -o copy pa1.c

echo "Starting copy program tests..."

# --- Test case 1 ---
# Source file does not exist
# Destination file doesn't exist
echo "Test 1: Source and destination files don't exist"
echo "The following message is the copy command aborting, can't get error redirection working"
./copy doesnotexist alsodoesnotexist
echo "----------------"

# --- Test case 2 ---
# Source file exists
# Destination file doesn't exist
echo "Test 2: Source exists, destination doesn't"
echo "$TEST_DATA" > source.test1
./copy source.test1 dest.test1
cat dest.test1
echo "----------------"

# --- Test case 3 ---
# Source file exists
# Destination file exists
echo "Test 3: Both source and destination exist (overwrite test)"
echo "$TEST_DATA" > source.test2
echo "dummy data" > dest.test2
./copy source.test2 dest.test2
cat dest.test2
echo "----------------"

# --- Test case 4 ---
# Source directory exists with files
# Destination directory does not exist
echo "Test 4: Directory copy (destination doesn't exist)"
mkdir source.test3
echo "$TEST_DATA" > source.test3/file1
echo "$TEST_DATA" > source.test3/file2
echo "$TEST_DATA" > source.test3/file3
./copy -r source.test3/ dest.test3
cat dest.test3/file1
cat dest.test3/file2
cat dest.test3/file3
echo "----------------"

# --- Test case 5 ---
# Source directory exists
# Destination directory exists with some files
echo "Test 5: Directory copy with existing files"
cp -r source.test3/ source.test4
cp -r dest.test3/ dest.test4
echo "SHOULD NOT BE TOUCHED" > dest.test4/untouched
echo "SHOULD BE OVERWRITTEN" > dest.test4/file1
./copy -r source.test4/ dest.test4
cat dest.test4/untouched
cat dest.test4/file1
echo "----------------"

