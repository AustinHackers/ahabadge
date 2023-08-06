#!/bin/sh
cmake -DCMAKE_TOOLCHAIN_FILE="../KSDK_1.2.0/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug "$@" .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="../KSDK_1.2.0/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release "$@" .
make -j4
