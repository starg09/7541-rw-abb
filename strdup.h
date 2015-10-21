#ifndef _STRDUP_H
#define _STRDUP_H

// Duplica un string, y devuelve el puntero al nuevo string (creado con malloc), o devuelve NULL de no poder hacerlo.
// Pre: s es un string valido.
// Pos: Devuelve un nuevo puntero a char, que apunta al duplicado de s, o a NULL.
char* strdup (const char* s);

#endif