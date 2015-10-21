CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror
EXEC=correr_pruebas
OBJ=testing.o strdup.o abb.o
CC=gcc

all: $(EXEC)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clear:
	rm $(OBJ) $(EXEC)

$(EXEC): $(OBJ) main.c
	$(CC) $(CFLAGS) $(OBJ) pruebas_alumno.c main.c -o $(EXEC)