#include <stdlib.h>
#include "cola.h"

/* Defino la estructura que tendran los nodos de la cola enlazada. Guardan un puntero al dato a guardar y una referencia al siguiente*/
struct nodo_cola {
    void* dato;
    struct nodo_cola* siguiente;
};

/* Defino la estructura de la cola enlazada, con una referencia al primer elemento de la cola y uno al final, para que encolar sea de orden 1*/
struct cola {
    struct nodo_cola* primero;
    struct nodo_cola* ultimo;
};

/* PRIMITIVAS DE LA COLA */

cola_t* cola_crear(void) {
    cola_t* cola = malloc(sizeof(cola_t));
    if (cola == NULL) {
	return NULL;
    }
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)) {
    if (cola != NULL) {
        if (!cola_esta_vacia(cola)) {
            struct nodo_cola* nodo = cola->primero;
            struct nodo_cola* nodo_aux;
            void* dato;
            while (nodo != NULL) {
                if (destruir_dato != NULL) {
                    dato = nodo->dato;
                    destruir_dato(dato);                    
                }
                nodo_aux = nodo->siguiente;
                free(nodo);
                nodo = nodo_aux;
            }
        }
        free(cola);   
    }
}

bool cola_esta_vacia(const cola_t *cola) {
    if (cola != NULL) {
        if ((cola->primero == NULL) && (cola->ultimo == NULL)) {
            return true;
        }
    }
    return false;
}

bool cola_encolar(cola_t *cola, void* valor) {
    if (cola != NULL) {
    struct nodo_cola* nuevo_nodo = malloc(sizeof(struct nodo_cola));
    if (nuevo_nodo == NULL) {
        return false;
    }
    nuevo_nodo->dato = valor;
    nuevo_nodo->siguiente = NULL;
    if (!cola_esta_vacia(cola)) {   
        cola->ultimo->siguiente = nuevo_nodo;
        cola->ultimo = nuevo_nodo;
        return true;    
    }
    cola->primero = nuevo_nodo;
    cola->ultimo = nuevo_nodo;
    return true;
    }
    return false;
}

void* cola_ver_primero(const cola_t *cola) {
    if (cola != NULL) {
        if (cola_esta_vacia(cola)) {
            return NULL;
        }  
        void* dato = cola->primero->dato;
        return dato;
    }
    return NULL;
}

void* cola_desencolar(cola_t *cola) {
    if (cola != NULL) {
        if (cola_esta_vacia(cola)) {
            return NULL;
        }
        struct nodo_cola* nodo_a_desencolar = cola->primero;
        void* dato_a_devolver = nodo_a_desencolar->dato;
        cola->primero = nodo_a_desencolar->siguiente;
        if (cola->primero == NULL) {
            cola->ultimo = NULL;
        }
        free(nodo_a_desencolar);
        return dato_a_devolver;
    }
    return NULL;
}
