UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    CC = clang++
    CFLAGS = -g -Wall -std=c++17 -Wno-deprecated-declarations -I./src -I./
    LIB_PATH = -L/opt/homebrew/lib
    INCLUDE_PATH = -I/opt/homebrew/include
    LIBFSCLIENT = lib/libfs_client_macos.o
    LIBFSSERVER = lib/libfs_server_macos.o
else
    CC = g++
    CFLAGS = -g -Wall -std=c++17 -Wno-deprecated-declarations -I./src -I./
    LIB_PATH =
    INCLUDE_PATH =
    LIBFSCLIENT = lib/libfs_client.o
    LIBFSSERVER = lib/libfs_server.o
endif

SRCS = src/kittyServer.cpp src/main.cpp src/parseRequests.cpp src/diskManager.cpp src/FsOperations.cpp
OBJS = $(SRCS:.cpp=.o)

all: fs test_all

fs: $(SRCS) $(LIBFSSERVER)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $(SRCS) $(LIBFSSERVER) -lboost_thread -lboost_system -pthread -ldl -o fs

test_all:
	@cd tests && for file in test*.cpp; do \
		name=$${file%.cpp}; \
		echo "Compiling $$file..."; \
		$(CC) $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) tests/$$file $(LIBFSCLIENT) -o tests/$$name; \
	done

clean:
	rm -f fs tests/test* *.o src/*.o
