#ifndef ABB_H
#define ABB_H


// 
// ÁRBOL BINARIO DE BÚSQUEDA
// 


// Definición de la estructura del ABB

typedef struct abb abb_t;


// Definición de los punteros a funciones a utilizar en funciones del ABB.

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


// Definición de las funciones del ABB.

// Crea un nuevo ABB. abb_comparar_clave_t compara la clave de dos nodos,
// y abb_destruir_dato_t se encarga de destruir los datos que estos contienen. 
// Post: devuelve el puntero al nuevo ABB, o NULL en caso de fallar.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Almacena un dato en el ABB. Si no se encuentra la clave, se crea un nodo, sino
// se reemplaza el valor pevio.
// Pre: El ABB fue creado.
// Post: Devuelve true al almacenar con éxito, o false en caso de error.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Devuelve un dato almacenado en el ABB, y destruye el nodo que lo contiene.
// Pre: El ABB fue creado.
// Post: Devuelve el dato almacenado y destruye el nodo que lo contenía, o devuelve
//       NULL cuando no se cumplan ciertas condiciones (Por ej.: clave no pertenece)
void *abb_borrar(abb_t *arbol, const char *clave);

// Similar a abb_borrar, solo que en este caso no destruye el nodo, solo devuelve
// el valor, o NULL de no hallarse.
// Pre: El ABB fue creado.
// Post: Devuelve el dato almacenado, o devuelve NULL cuando no se cumplan ciertas
//       condiciones (Por ej.: clave no pertenece)
void *abb_obtener(const abb_t *arbol, const char *clave);

// Averigua si existe un nodo en el ABB con la clave provista.
// Pre: El ABB fue creado.
// Post: Devuelve true de encontrar el nodo, o false en caso contrario.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de nodos en el ABB.
size_t abb_cantidad(abb_t *arbol);

// Destruye
void abb_destruir(abb_t *arbol);



// 
// ITERADOR INTERNO DEL ÁRBOL BINARIO DE BÚSQUEDA
// 

// 
void abb_in_order(abb_t *arbol, bool (*visitar)(const char *, void *, void *), void *extra);



// 
//  ITERADOR EXTERNO DEL ÁRBOL BINARIO DE BÚSQUEDA
// 


// Definición de la estructura del iterador externo del ABB

typedef struct abb_iter abb_iter_t;


// Definición de las funciones del iterador externo del ABB.

// 
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// 
bool abb_iter_in_avanzar(abb_iter_t *iter);

// 
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// 
bool abb_iter_in_al_final(const abb_iter_t *iter);

// 
void abb_iter_in_destruir(abb_iter_t* iter);


#endif