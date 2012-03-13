CXX = g++

LIB_DIR = ../server
SRC_DIR = ./src

CFLAGS = -g -Wall -I$(LIB_DIR)/include -I./include
LDFLAGS = -lpthread #-lserver -L$(LIB_DIR)

CLIENT = client

$(CLIENT): $(SRC_DIR)/Client.cpp
	$(CXX) $^  $(LIB_DIR)/libserver.a $(CFLAGS) $(LDFLAGS) -o $@

clean: 
	rm $(CLIENT)
	
all: $(CLIENT)
