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

// Busca el nodo con clavemínimo de un ABB.
const abb_t* abb_buscar_min(abb_t* arbol){
	if (arbol->izq == NULL)
		return arbol;
	return abb_buscar_min(arbol->izq);
}

// Busca el nodo máximo de un ABB.
const abb_t* abb_buscar_max(abb_t* arbol){
	if (arbol->der == NULL)
		return arbol;
	return abb_buscar_max(arbol->der);
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
	if (arbol->nodos == 0)
		return NULL;

	void* dato;

	int cmp = (arbol->func_comp(clave, arbol->clave) );

	if (cmp < 0){
		if (arbol->izq == NULL)
			return NULL;
		dato = abb_obtener(arbol->izq, clave);
	} else if (cmp > 0){
		if (arbol->der == NULL)
			return NULL;
		dato = abb_obtener(arbol->der, clave);
	} else if (cmp == 0){
		return arbol->dato;
	}
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->nodos;
}

void abb_destruir(abb_t *arbol){
	if (arbol->nodos > 0){
		if (arbol->izq != NULL)
			abb_destruir(arbol->izq);
		if (arbol->der != NULL)
			abb_destruir(arbol->der);

		if (arbol->func_destruct != NULL)
			func_destruct(arbol->dato);

		free(arbol->clave);
	}

	free(arbol);
}