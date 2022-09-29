# to run in terminal $ make
CC = gcc

# List all your .c (or .cpp or .cc) files here (source files, excluding header files)
SRC_CODE = driver.c input.c scheduler.c

# tags
TAG = -w

#################################

# default rule

all: $(SRC_CODE)
	$(CC) $(TAG) -o Scheduler $(SRC_CODE) 