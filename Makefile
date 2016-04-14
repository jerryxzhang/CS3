BIN = ./bin/
BINTEST = $(BIN)test/
SOURCE = ./src/
TEST = ./test/
LIBS = -lm -lboost_system -lboost_thread -lgtest -pthread 
CC = g++
CFLAGS = -std=c++0x -Wall -pedantic -Wextra -g
INC = -I./inc/ 
SOURCEFILES = $(SOURCE)config_parser.cpp

LIST = $(BIN)webserver $(BINTEST)config_parser_test

all: $(LIST)

test: $(BINTEST)config_parser_test
	$(BINTEST)config_parser_test

$(BIN)%:  $(SOURCE)%.cpp
	$(CC) $< $(CFLAGS) -o $@ $(LIBS) $(INC)

$(BINTEST)%:  $(TEST)%.cpp
	$(CC) $< $(SOURCEFILES) /usr/src/gtest/src/gtest_main.cc $(CFLAGS) -o $@ $(LIBS) $(INC)

