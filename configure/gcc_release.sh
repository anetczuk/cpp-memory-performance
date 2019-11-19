#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


BUILD_TYPE=Release


SRC_DIR=$SCRIPT_DIR/../src


if [ "$#" -ge 1 ]; then
    BUILD_DIR_NAME=$1
else
    BUILD_DIR_NAME=$(echo $BUILD_TYPE"_gcc" | tr '[:upper:]' '[:lower:]')
fi

BUILD_DIR=$SCRIPT_DIR/../build/$BUILD_DIR_NAME


## remove old configuration
rm -rf $BUILD_DIR 


echo -e "Creating build directory: $BUILD_DIR\n"

mkdir -p $BUILD_DIR
cd $BUILD_DIR


export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE $SRC_DIR
