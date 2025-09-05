#!/bin/sh

set -xe
clang -std=c23 -DOUTPUT_VERBOSE -Wall -Wextra -pedantic -g *.c -o out/op3l
rm -rf out/*.dSYM