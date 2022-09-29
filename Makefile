# to run in terminal $ make
CC = gcc

# List all your .c (or .cpp or .cc) files here (source files, excluding header files)
SRC_CODE = main.c input.c scheduler.c

#################################

# default rule

all: $(SRC_CODE)
	$(CC) -w -o Scheduler $(SRC_CODE) 