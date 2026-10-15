PREFIX ?= /usr/local
CXX := g++

CXXFLAGS := -Wall -Iinclude

SRC_DIR := ./src
INCLUDE_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRCS))

LIB_NAME := libByteS.a


all: $(LIB_NAME)


$(LIB_NAME): $(OBJS)
	ar rcs $@ $^


$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


install: $(LIB_NAME)

	install -d $(PREFIX)/lib/ByteS
	install -d $(PREFIX)/include/ByteS

	install -m 644 $(LIB_NAME) $(PREFIX)/lib/ByteS/

	install -m 644 $(INCLUDE_DIR)/ByteS.hpp $(PREFIX)/include/ByteS/


clean:
	rm -f $(SRC_DIR)/*.o $(LIB_NAME)
