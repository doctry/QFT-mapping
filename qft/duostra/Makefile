# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
DBGFLAGS  = -g -Wall -DDEBUG
CFLAGS = -Wall -O3 -DNDEBUG

# the build target executable:
DEPS = include/device.h include/qft_mapper.h include/qft_topo.h
INC = -Iinclude/
TARGET = bin/qft_mapping
DBG = bin/qft_mapping_dbg
SRC = src/device.cpp src/main.cpp src/qft_mapper.cpp src/qft_topo.cpp

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS)
	$(CC) $(CFLAGS) $(INC) -o $@ $(SRC)

dbg: $(DBG)

$(DBG): $(SRC) $(DEPS)
	$(CC) $(DBGFLAGS) $(INC) -o $@ $(SRC)

clean:
	$(RM) $(TARGET) $(DBG)