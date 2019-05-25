# Variables
CC		= g++
CXXFLAGS	= -std=c++11 -O3 -Wall
LDFLAGS		= -lpthread
SRC_DIR		= src
INC_DIR		= include
BIN_DIR		= bin
BUILD_DIR	= build
MKDIR		= mkdir -p
RM		= rm -rf

# Main program compilation
.PHONY	= all
all: $(BIN_DIR)/data_preparation

$(BIN_DIR)/data_preparation: $(BUILD_DIR)/main.o $(BUILD_DIR)/csv.o $(BUILD_DIR)/data_preparation.o
	$(MKDIR) $(BIN_DIR)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/csv.h $(INC_DIR)/data_preparation.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BUILD_DIR)/csv.o: $(SRC_DIR)/csv.cpp $(INC_DIR)/csv.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -I$(INC_DIR) -o $@ -c $<

$(BUILD_DIR)/data_preparation.o: $(SRC_DIR)/data_preparation.cpp $(INC_DIR)/data_preparation.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -I$(INC_DIR) -o $@ -c $<

# Clean
.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)
	$(RM) $(BIN_DIR)
