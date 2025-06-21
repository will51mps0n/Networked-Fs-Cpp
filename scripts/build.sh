#!/bin/bash

export CPLUS_INCLUDE_PATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
export LD_LIBRARY_PATH=/opt/homebrew/lib

clang++ -g -Wall -std=c++17 -Wno-deprecated-declarations \
    -D_XOPEN_SOURCE \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    kittyServer.cpp main.cpp parseRequests.cpp diskManager.cpp FsOperations.cpp libfs_server_macos.o \
    -lboost_thread -lboost_system -pthread -ldl \
    -o fs

clang++ -g -Wall -std=c++17 -Wno-deprecated-declarations \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testSpec.cpp libfs_client_macos.o \
    -o testSpec

clang++ -g -Wall -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testFs.cpp libfs_client_macos.o \
    -o testFs

clang++ -g -Wall -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testInvalid.cpp libfs_client_macos.o \
    -o testInvalid

clang++ -g -Wall -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testMultiple.cpp libfs_client_macos.o \
    -o testMultiple

clang++ -g -Wall -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testEthan.cpp libfs_client_macos.o \
    -o testEthan

clang++ -g -Wall -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testSys.cpp libfs_client_macos.o \
    -o testSys

clang++ -g -Wall -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    testThreads.cpp libfs_client_macos.o \
    -o testThreads