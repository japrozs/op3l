#!/bin/sh

set -xe
clang -Wall -Wextra -pedantic -g *.c -o out/op3l
rm -rf out/*.dSYM