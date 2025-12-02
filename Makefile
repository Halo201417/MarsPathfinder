CC = gcc
CFLAGS = -Wall -pthread -I./include

# LIBS = -lrt -lrtlib

SRC = src/main.c src/tasks.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = bin/rt_project

all: $(EXEC)

$(EXEC): $(OBJ)
		$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

clean:
		rm -f src/*.o $(EXEC)