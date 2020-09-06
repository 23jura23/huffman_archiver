HW_N = 03

#######################################################

.PHONY: all build debug debug_noopt release fsanitize test test_fsanitize clean ticket

CXX = g++

CFLAGS = --std=c++17 -Wall -Wextra -Werror -Wshadow
INC = -Iinclude
EXTRA_FLAGS = 
LINK_FLAGS =

DEBUG_FLAGS = -g -D_GLIBCXX_DEBUG
RELEASE_FLAGS = -Ofast
FS_FLAGS = -fsanitize=address

FILES = $(shell find src -maxdepth 1 -name "*.cpp" -printf "%f\n")
SRCS = $(FILES:%.cpp=src/%.cpp)
OBJS = $(FILES:%.cpp=obj/%.o)
DEP = $(OBJS:%.o=%.d)
BIN = huffman

TEST_FILES = $(subst main.cpp,,$(shell find src -maxdepth 1 -name "*.cpp" -printf "%f\n") $(shell find test -maxdepth 1 -name "*.cpp" -printf "%f\n")) 
TEST_SRCS = $(TEST_FILES:%.cpp=test/%.cpp)
TEST_OBJS = $(TEST_FILES:%.cpp=obj/%.o)
TEST_DEP = $(TEST_OBJS:%.o=%.d)
TEST_BIN = huffman_test

all: release
build: bin/${BIN} bin/${TEST_BIN}

debug_noopt: EXTRA_FLAGS += ${DEBUG_FLAGS}
debug_noopt: build

# debug: EXTRA_FLAGS += -Ofast
debug: debug_noopt

release: EXTRA_FLAGS += ${RELEASE_FLAGS}
release: build

fsanitize: EXTRA_FLAGS += ${FS_FLAGS}
fsanitize: debug

clang: CXX = clang++
clang: EXTRA_FLAGS += -Wno-c++98-compat -Wno-c++-compat -Wno-padded -O2
clang: build

ticket: CXX = g++-7
ticket: release

test: debug
	./bin/${TEST_BIN}

test_fsanitize: fsanitize test

bin/${BIN}: ${OBJS} | obj bin
	${CXX} ${CFLAGS} ${EXTRA_FLAGS} ${OBJS} -o bin/${BIN} ${LINK_FLAGS}

bin/${TEST_BIN}: ${TEST_OBJS} | obj bin
	${CXX} ${CFLAGS} ${EXTRA_FLAGS} ${TEST_OBJS} -o bin/${TEST_BIN} ${LINK_FLAGS}

-include ${DEP}
-include ${TEST_DEP}

obj/%.o: src/%.cpp | obj
	${CXX} ${CFLAGS} ${EXTRA_FLAGS} ${INC} -MMD -c $< -o $@

obj/%.o: test/%.cpp | obj
	${CXX} ${CFLAGS} ${EXTRA_FLAGS} ${INC} -MMD -c $< -o $@

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm -rf obj bin

