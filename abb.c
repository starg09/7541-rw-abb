#include "abb.h"
#include "strdup.h"
#include "pila.h"
#include "cola.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

// Devuelve true si el ABB está vacío. Es decir, si es nil.
bool abb_es_nil(const abb_t* arbol){
	return (arbol->clave == NULL);
}

// Recalcula los nodos que posee un ABB. Se utiliza al guardar o borrar dentro del mismo.
void abb_recalcular_nodos(abb_t* arbol){
	arbol->nodos = 0;
	// Esta condición es equivalente a decir que el arbol es nil.
	if (abb_es_nil(arbol))
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
	if (abb_es_nil(arbol)){
		char* copia_clave = strdup(clave);
		if (copia_clave == NULL)
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

// Busca el nodo con clavemínimo de un ABB.
abb_t* abb_buscar_min(abb_t* arbol){
	if (arbol->izq == NULL)
		return arbol;
	return abb_buscar_min(arbol->izq);
}

// Busca el nodo máximo de un ABB.
abb_t* abb_buscar_max(abb_t* arbol){
	if (arbol->der == NULL)
		return arbol;
	return abb_buscar_max(arbol->der);
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (abb_es_nil(arbol))
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
		dato = arbol->dato;

		if ( (arbol->izq == NULL) && (arbol->der == NULL) ){
			arbol->dato = NULL;
			free(arbol->clave);
			arbol->clave = NULL;
		} else {
			abb_t* arbol_temp;
			void* dato_temp;
			char* clave_temp;

			if (arbol->izq != NULL) {
				arbol_temp = abb_buscar_max(arbol->izq);
				clave_temp = strdup(arbol_temp->clave);
				if (clave_temp == NULL)
					return NULL;
				dato_temp = abb_borrar(arbol->izq, clave_temp);
				if (arbol->izq->nodos == 0){
					abb_destruir(arbol->izq);
					arbol->izq = NULL;
				}
			} else {
				arbol_temp = abb_buscar_min(arbol->der);
				clave_temp = strdup(arbol_temp->clave);
				if (clave_temp == NULL)
					return NULL;
				dato_temp = abb_borrar(arbol->der, clave_temp);
				if (arbol->der->nodos == 0){
					abb_destruir(arbol->der);
					arbol->der = NULL;
				}
			}

			arbol->dato = dato_temp;
			free(arbol->clave);
			arbol->clave = clave_temp;
		}
	} 

	abb_recalcular_nodos(arbol);
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	if (abb_es_nil(arbol)) {
		return NULL;
	} else {
		int comp = strcmp(arbol->clave, clave);

		if (comp == 0)
			return arbol->dato;
		else if ( (comp < 0) && (arbol->izq != NULL) )
			return abb_obtener(arbol->izq, clave);
		else if ( (comp > 0) && (arbol->der != NULL) )
			return abb_obtener(arbol->der, clave);
		else
			return NULL;
	}
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (abb_es_nil(arbol)) {
		return false;
	} else {
		int comp = strcmp(arbol->clave, clave);

		if (comp == 0)
			return true;
		else if ( (comp < 0) && (arbol->izq != NULL) )
			return abb_pertenece(arbol->izq, clave);
		else if ( (comp > 0) && (arbol->der != NULL) )
			return abb_pertenece(arbol->der, clave);
		else
			return false;
	}
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->nodos;
}

void abb_destruir(abb_t *arbol){
	if (!abb_es_nil(arbol)){
		if (arbol->izq != NULL)
			abb_destruir(arbol->izq);
		if (arbol->der != NULL)
			abb_destruir(arbol->der);

		if (arbol->func_destruct != NULL)
			arbol->func_destruct(arbol->dato);

		free(arbol->clave);
	}

	free(arbol);
}

// Iterador interno IN-ORDER

void abb_in_order(abb_t *arbol, bool (*visitar)(const char *, void *, void *), void *extra){
    if (!abb_es_nil(arbol)) {
        abb_in_order(arbol->izq, visitar, extra);
        if (visitar(arbol->clave, arbol->dato, extra)) {
            abb_in_order(arbol->der, visitar, extra);    
            }
        }
    }


// Funciones de el iterador externo IN-ORDER

struct abb_iter {
	pila_t* pila_iter;
};

void apilar_hijos_izquierdos(pila_t* pila, const abb_t* arbol) {
	if (arbol != NULL) {
		if (arbol->izq != NULL) {
			printf("Nodo a la izquierda: %s\n", arbol->izq->clave);
			pila_apilar(pila, (void*)arbol->izq);
			apilar_hijos_izquierdos(pila, arbol->izq);
		}
	}

}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) {
		return NULL;
	}
	pila_t* pila_iter = pila_crear();
	if (!pila_iter) {
		free(iter);
		return NULL;
	}

	iter->pila_iter = pila_iter;
	if (!abb_es_nil(arbol)) {
	    pila_apilar(iter->pila_iter, (void*)arbol);
		printf("Raíz apilada: %s\n", arbol->clave);
    	apilar_hijos_izquierdos(iter->pila_iter, arbol);
	}
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (pila_esta_vacia(iter->pila_iter))
		return false;
	printf("Actual: %s\n", abb_iter_in_ver_actual(iter));
	abb_t* arbol_temp = (abb_t*)pila_desapilar(iter->pila_iter);
	if (arbol_temp->der != NULL) {
		printf("Nodo a la derecha: %s\n", arbol_temp->der->clave);
		pila_apilar(iter->pila_iter, (void*)arbol_temp->der);
		apilar_hijos_izquierdos(iter->pila_iter, arbol_temp->der);
		printf("Nuevo: %s\n", abb_iter_in_ver_actual(iter));
	}
	if (pila_esta_vacia(iter->pila_iter))
		return false;
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	abb_t* actual = (abb_t*)pila_ver_tope(iter->pila_iter);
	return actual ? actual->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return (pila_ver_tope(iter->pila_iter) == NULL);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila_iter);
	free(iter);
}

// Funcion de impresion por niveles

/*void imprimir_por_niveles(const abb_t* arbol){
	if(!abb_es_nil(arbol)){
  		cola_t* cola = cola_crear(); 
  		if (cola != NULL) {
  			abb_t* arbol_temp;
  			cola_encolar(cola, (void*)arbol);
  			while(!cola_esta_vacia(cola)){
  				arbol_temp = cola_desencolar(cola);
  				printf(" %s |", arbol_temp->clave);
  				if (!abb_es_nil(arbol_temp->izq)) {
  					cola_encolar(cola, (void*)arbol_temp->izq);
  				}
  				if (!abb_es_nil(arbol_temp->der)) {
  					cola_encolar(cola, (void*)arbol_temp->der);
  				}
  			}
  		cola_destruir(cola,NULL);
  		}
  	}
}*/