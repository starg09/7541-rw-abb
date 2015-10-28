CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror
EXEC=correr_pruebas
OBJ=strdup.o pila.o cola.o testing.o abb.o 
CC=gcc

all: $(EXEC)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clear:
	rm $(OBJ) $(EXEC)

$(EXEC): $(OBJ) pruebas_alumno.c main.c
	$(CC) $(CFLAGS) $(OBJ) pruebas_alumno.c main.c -o $(EXEC)

zip:
	rm ./ABB\ -\ 98373\ -\ Bianchi\ -\ 96251\ -\ Queirolo\ Dominguez.zip
	zip ./ABB\ -\ 98373\ -\ Bianchi\ -\ 96251\ -\ Queirolo\ Dominguez.zip *.c *.h Makefile