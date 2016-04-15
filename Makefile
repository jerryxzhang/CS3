BIN = ./bin/
BINTEST = $(BIN)test/
SOURCE = ./src/
TEST = ./test/
LIBS = -lm -lboost_system
CC = g++
CFLAGS = -std=c++0x -Wall -pedantic -Wextra -g
INC = -I./inc/
SOURCEFILES = $(SOURCE)config_parser.cpp

LIST = $(BIN)webserver $(BINTEST)config_parser_test

OS := $(shell uname)
ifeq ($(OS),Darwin)
	LIBS += -L/usr/local/lib
	INC += -I/usr/local/include
endif

all: $(LIST)

test: $(BINTEST)config_parser_test
	$(BINTEST)config_parser_test

$(BIN)%:  $(SOURCE)%.cpp
	$(CC) $< $(CFLAGS) -o $@ $(LIBS) $(INC)

$(BINTEST)%:  $(TEST)%.cpp
	$(CC) $< $(SOURCEFILES) -lgtest -pthread /usr/src/gtest/src/gtest_main.cc $(CFLAGS) -o $@ $(LIBS) $(INC)

