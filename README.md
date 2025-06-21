# Networked fs cpp

This project implements a multithreaded, networked file server with a custom file system, concurrency-safe path traversal, disk block management, and a full automated test suite.

## Features

Hierarchical file and directory structure

Multithreaded TCP server with concurrency-safe operations

Boost thread integration

Lock-safe path traversal and disk manipulation

Disk-backed block allocation with createfs/showfs utilities

Custom filesystem client/server API (in fs_client.h, fs_server.h)

Comprehensive test suite with .fs disk images

## Project Layout


```├── Makefile                  # Platform-aware build system
├── README.md
├── src/                      # All core source files (.cpp and .h)
│   ├── main.cpp
│   ├── FsOperations.*
│   ├── diskManager.*
│   ├── parseRequests.*
│   └── kittyServer.*
├── tests/                    # Client-side test binaries + optional .fs images
├── scripts/                  # Utility scripts (runtests.sh, etc.)
├── lib/                      # libfs_client.o and libfs_server.o (macOS/Linux)
├── createfs / createfs_macos  # Binary to create blank FS image
├── showfs / showfs_macos      # Debugging tool to print FS image
├── fs_param.h               # Filesystem constants (e.g., block size, limits)
├── fs_client.h              # Client interface
├── fs_server.h              # Server interface
```

## Build Instructions

## macOS

cd e_server
chmod +x scripts/runtests.sh
./scripts/runtests.sh

This script will:

Clean and rebuild all test and server binaries

Detect valid test clients in tests/

Start the file server on unique ports

Load .fs disk images when available, or initialize new ones

Log all file server output to logs/

## Manual Build

make
./fs 8000
./testSpec localhost 8000

Use createfs or createfs_macos to initialize disks before starting fs.

## Running Tests

To automatically build and run all tests:

./scripts/runtests.sh

## This script will:

Detect all executable test clients in tests/

Run each client on a dedicated port

Spawn the server and kill it between tests

Output logs to logs/ for each test case

Example Test Cases

Each test is a C++ binary linked to libfs_client. Some tests use custom .fs images:

testSpec: Validates spec compliance

testInvalid: Exercises error conditions

testSys: Stresses nested directory structure

testMultiple: Concurrent operations

testMaxLength: Boundary checks for filenames and paths

testRecurse: Deep recursive create/delete tests

## Clean Up

make clean
rm -rf logs/ *.out *.fs


