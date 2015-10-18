#include "abb.h"
#include "strdup.h"

// Estructura del ABB (y en este caso del nodo mismo)

struct abb {
	char* clave;
	void* dato;
	abb_t* izq;
	abb_t* der;
	size_t nodos;
	abb_comparar_clave_t func_comp;
	abb_destruir_dato_t func_destruct;
};


// Funciones principales del ABB

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc(sizeof(abb_t));
	if (arbol == NULL)
		return NULL;
	arbol->clave = NULL;
	arbol->dato = NULL;
	arbol->izq = NULL;
	arbol->der = NULL;
	arbol->nodos = 0;
	arbol->func_comp = cmp;
	arbol->func_destruct = destruir_dato;

	return arbol;
}

// Recalcula los nodos que posee un ABB. Se utiliza al guardar o borrar dentro del mismo.
void abb_recalcular_nodos(abb_t* arbol){
	arbol->nodos = 0;
	// Esta condición es equivalente a decir que el arbol es nil.
	if (arbol->clave == NULL)
		return;
	// Si no es nil, entonces la raíz implica que existe por lo menos un nodo, sin contar
	// los de las ramas.
	arbol->nodos++;
	if (arbol->izq != NULL)
		arbol->nodos += arbol->izq->nodos;
	if (arbol->der != NULL)
		arbol->nodos += arbol->der->nodos;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	bool se_guardo = true;
	// Esta condición también es equivalente a decir que el arbol es nil.
	if (arbol->nodos == 0){
		char* copia_clave = strdup(clave);
		if (copia == NULL)
			return false;
		arbol->clave = copia_clave;
		arbol->dato = dato;
	} else {
		int cmp = (arbol->func_comp(clave, arbol->clave) );
		if (cmp == 0){
			arbol->dato = dato;
			// No hace falta recalcular nodos en este caso, puedo retornar directamente.
			return true;
		} else if (cmp < 0){
			if (arbol->izq == NULL){
				abb_t* nuevo_arbol_izq = abb_crear(arbol->func_comp, arbol->func_destruct);
				if (nuevo_arbol_izq == NULL)
					return false;
				arbol->izq = nuevo_arbol_izq;
			}
			se_guardo = abb_guardar(arbol->izq, clave, dato);
			
			if (!se_guardo){
				abb_destruir(arbol->izq);
				arbol->izq = NULL;
				return false;
			}
		} else if (cmp > 0){
			if (arbol->der == NULL){
				abb_t* nuevo_arbol_der = abb_crear(arbol->func_comp, arbol->func_destruct);
				if (nuevo_arbol_der == NULL)
					return false;
				arbol->der = nuevo_arbol_der;
			}
			se_guardo = abb_guardar(arbol->der, clave, dato);

			if (!se_guardo){
				abb_destruir(arbol->der);
				arbol->der = NULL;
				return false;
			}
		}
	}

	abb_recalcular_nodos(arbol);
	return se_guardo;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (arbol->nodos == 0)
		return NULL;

	void* dato;

	int cmp = (arbol->func_comp(clave, arbol->clave) );

	if (cmp < 0){
		if (arbol->izq == NULL)
			return NULL;
		dato = abb_borrar(arbol->izq, clave);
	} else if (cmp > 0){
		if (arbol->der == NULL)
			return NULL;
		dato = abb_borrar(arbol->der, clave);
	} else if (cmp == 0){
		// Hacer después, caso más complejo.
	} 

	abb_recalcular_nodos(arbol);
	return dato;
}