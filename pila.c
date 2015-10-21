#include "pila.h"
#include <stdlib.h>
/* Defino el tamaño inicial con el que se creará la pila */
#define TAM_INICIAL 20
/* Defino el factor por el cual el tamaño de la pila aumentará en cada redimención */
#define FACTOR_INCREMENTO 2

/* Definición del struct pila proporcionado por la cátedra. */
struct pila {
    void **datos;
    size_t tam;
    size_t largo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * **************************************************************** */

// ...

pila_t* pila_crear() {
    pila_t* pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
	return NULL;
    }
    pila->datos = malloc(TAM_INICIAL * sizeof(void*));

    if (TAM_INICIAL > 0 && pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->tam = TAM_INICIAL;
    pila->largo = 0;
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    if (pila->largo != 0) {
        return false;
    }
    return true;
}

bool pila_apilar(pila_t *pila, void* valor) {
    /* Si la pila esta llena, la redimensiona al doble de tamaño actual */
    if (pila->tam == pila->largo) {
        size_t tam_nuevo = pila->tam * FACTOR_INCREMENTO;
        void** datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));
        if (tam_nuevo > 0 && datos_nuevo == NULL) {
        free(pila);
        return NULL;
        }
        pila->datos = datos_nuevo;
        pila->tam = tam_nuevo;
    }
    pila->datos[pila->largo] = valor;
    pila->largo++;
    return true;
}

void* pila_ver_tope(const pila_t *pila) {
    if (pila->largo != 0) {
        void* valor = pila->datos[(pila->largo - 1)];
        return valor;
    }
    return NULL;
}

void* pila_desapilar(pila_t *pila) {
    if (pila->largo != 0) {
        void* valor = pila->datos[(pila->largo - 1)];
        pila->largo--;
    /* Si la pila contiene una cantidad de elementos menor o igual a un cuarto de su capacidad total, se redimensiona a la mitad de tamaño actual */
        if ((pila->largo <= (pila->tam / 4)) && (pila->tam > TAM_INICIAL)) {
        size_t tam_nuevo = pila->tam / FACTOR_INCREMENTO;
        void** datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));
        if (tam_nuevo > 0 && datos_nuevo == NULL) {
        free(pila);
        return NULL;
        }
        pila->datos = datos_nuevo;
        pila->tam = tam_nuevo;
        }   
        return valor;
    }
    return NULL;
}
        
