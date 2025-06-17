UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    # macOS settings
    CC = clang++
    CFLAGS = -D_XOPEN_SOURCE -g -Wall -std=c++17 -Wno-deprecated-declarations
    LIBFSCLIENT = libfs_client_macos.o
    LIBFSSERVER = libfs_server_macos.o
    
    INCLUDE_PATH = -I/opt/homebrew/include
    LIB_PATH = -L/opt/homebrew/lib
    
    BOOST_THREAD = boost_thread-mt
    export CPLUS_INCLUDE_PATH = /opt/homebrew/include
    export LIBRARY_PATH = /opt/homebrew/lib
    export LD_LIBRARY_PATH = /opt/homebrew/lib
else
    # Linux/CAEN settings
    ifneq ($(shell hostname | grep caen),)
        CC = g++
        CFLAGS = -g -Wall -std=c++17 -Wno-deprecated-declarations
        
        INCLUDE_PATH = 
        LIB_PATH = 
    else
        CC = g++
        CFLAGS = -g -Wall -std=c++17 -Wno-deprecated-declarations
        INCLUDE_PATH =
        LIB_PATH =
        BOOST_THREAD = boost_thread
    endif
    LIBFSCLIENT = libfs_client.o
    LIBFSSERVER = libfs_server.o
endif

FS_SOURCES = kittyServer.cpp main.cpp parseRequests.cpp diskManager.cpp FsOperations.cpp
FS_OBJS = kittyServer.o main.o parseRequests.o diskManager.o FsOperations.o 

all: fs testSpec testFs testInvalid testEx testMultiple testEthan testSys testDel testDirBlock testEmpty testDeep testEdge testMax testMix testExhaustive testSystem testFull testSys2 testFullSys testAllocation testAllocation2 testEthanFill testExpansive testExpansive2 testFullEx testLoad testMaxLength testMaxLength0 testMaxLength2 testerrors testDeleteSetup testDelete testRecurse testFile

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

# Server build command
fs: kittyServer.cpp main.cpp parseRequests.cpp diskManager.cpp FsOperations.cpp $(LIBFSSERVER)
	./autotag.sh
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) \
	kittyServer.cpp main.cpp parseRequests.cpp diskManager.cpp FsOperations.cpp $(LIBFSSERVER) \
	-lboost_thread -lboost_system -pthread -ldl \
	-o $@

testSpec: testSpec.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testFs: testFs.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testInvalid: testInvalid.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testMultiple: testMultiple.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testEthan: testEthan.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testSys: testSys.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testSys2: testSys2.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testEx: testEx.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testDel: testDel.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testDirBlock: testDirBlock.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testEmpty: testEmpty.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testDeep: testDeep.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testEdge: testEdge.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testMax: testMax.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testMix: testMix.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testExhaustive: testExhaustive.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@ 

testSystem: testSystem.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@ 

testFull: testFull.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@ 

testFill1: testFill1.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@ 

testFullSys: testFullSys.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@ 

testAllocation: testAllocation.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testAllocation2: testAllocation2.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testEthanFill: testEthanFill.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testExpansive: testExpansive.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testExpansive2: testExpansive2.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testFullEx: testFullEx.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testLoad: testLoad.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testMaxLength: testMaxLength.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testMaxLength0: testMaxLength0.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@
	
testMaxLength2: testMaxLength2.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testerrors: testerrors.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testZ: testZ.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testDeleteSetup: testDeleteSetup.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testDelete: testDelete.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testRecurse: testRecurse.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testFile: testFile.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testF: testF.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@

testF1: testF1.cpp $(LIBFSCLIENT)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $^ -o $@


clean:
	rm -f fs fs testSpec testFs testInvalid testEx testMultiple testEthan testSys testDel testDirBlock testEmpty testDeep testEdge testMax testMix  testExhaustive testSystem testFull testSys2 testFullSys testAllocation testAllocation2 testEthanFill testExpansive testExpansive2 testFullEx testLoad testMaxLength testMaxLength0 testMaxLength2 testerrors testZ testDeleteSetup testDelete testRecurse testFile testF $(FS_OBJS)
	rm -rf *.dSYM
