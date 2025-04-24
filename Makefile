CC = g++
CFLAGS = -std=c++17 -Wall
SRC = steg.cpp bmp_utils.cpp steg_utils.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = bin/steg_tool

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: clean
