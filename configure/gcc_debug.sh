#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


BUILD_TYPE=Debug


SRC_DIR=$SCRIPT_DIR/../src
BUILD_DIR=$SCRIPT_DIR/../build/$BUILD_TYPE"_GCC"

mkdir -p $BUILD_DIR
cd $BUILD_DIR


## remove old configuration
rm -f CMakeCache.txt 


export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cmake --config $BUILD_TYPE $SRC_DIR
