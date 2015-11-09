CFLAGS=-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror
EXEC=correr_pruebas
OBJ=strdup.o pila.o cola.o testing.o abb.o 
CC=gcc

all: $(EXEC)

zip:
	rm -f ./ABB\ -\ 98373\ -\ Bianchi\ -\ 96251\ -\ Queirolo\ Dominguez.zip
	zip ./ABB\ -\ 98373\ -\ Bianchi\ -\ 96251\ -\ Queirolo\ Dominguez.zip *.c *.h Makefile

vtest:
	valgrind --leak-check=full --track-origins=yes ./correr_pruebas

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clear:
	rm $(OBJ) $(EXEC)

$(EXEC): $(OBJ) pruebas_alumno.c main.c
	$(CC) $(CFLAGS) $(OBJ) pruebas_alumno.c main.c -o $(EXEC)