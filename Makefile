#
# COMP30023 Computer Systems
# Project 1
# Author: traillj
#
# Makefile.
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS 	= -Wall  


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		main.c linked_list.c process.c process_control.c scheduling.c \
			memory.c
OBJ =		main.o linked_list.o process.o process_control.o scheduling.o \
			memory.o
EXE = 		simulation

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE) 

## Dependencies

main.o:	main.c process_control.h process.h linked_list.h
linked_list.o:	linked_list.c linked_list.h
process.o:	process.c process.h
process_control.o:	process_control.c process_control.h memory.h scheduling.h \
					process.h linked_list.h
scheduling.o:	scheduling.c scheduling.h process.h linked_list.h
memory.o:	memory.c memory.h process.h linked_list.h
