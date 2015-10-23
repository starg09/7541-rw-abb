#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
// ESTRUCTURAS; VARIABLES Y DEFINICIONES AUXILARES
char* clave1 = "Maria";
char* clave2 = "Ines";
char* clave3 = "Carlos";
char* clave4 = "Roberto";
char* clave5 = "Guillermo";
char* clave6 = "Cristian";
char* clave7 = "Alan";
char* clave8 = "Barbara";

static void prueba_abb_volumen(size_t largo, bool debug)
{
    abb_t* abb3 = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = abb_guardar(abb3, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb3) == largo);

    /* Verifica que devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb3, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb3, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb3) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb3, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb borrar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb3) == 0);

    /* Destruye el abb y crea uno nuevo que sí libera */
    abb_destruir(abb3);
    abb3 = abb_crear(strcmp, free);

    /* Inserta 'largo' parejas en el abb */
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb3, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    /* Destruye el abb - debería liberar los enteros */
    abb_destruir(abb3);

}

void pruebas_abb_alumno() {
	//ABB VACIO
	abb_t* abb1 = abb_crear(strcmp, free);
	print_test("El ABB fue creado exitosamente", abb1 != NULL);
	print_test("El ABB esta vacio inicialmente", abb_cantidad(abb1) == 0);
	print_test("Existe nodo con clave 'Prueba' es false", abb_pertenece(abb1, "Prueba") == false );
	print_test("Obtener dato de nodo con clave 'Prueba' es NULL", abb_obtener(abb1, "Prueba") == NULL );
	abb_iter_t* iter1 = abb_iter_in_crear(abb1);
	print_test("El iterador fue creado", iter1 != NULL);
	print_test("Avanzar con el iterador es false", abb_iter_in_avanzar(iter1) == false);
	print_test("Ver actual con el iterador es NULL", abb_iter_in_ver_actual(iter1) == NULL);
	print_test("El iterador esta al final", abb_iter_in_al_final(iter1) == true);
    abb_iter_in_destruir(iter1);
	abb_destruir(abb1);

	//ABB 8 ELEMENTOS
	abb_t* abb2 = abb_crear(strcmp, NULL);
	print_test("El ABB fue creado exitosamente", abb2 != NULL);
	print_test("El ABB esta vacio inicialmente", abb_cantidad(abb2) == 0);
	print_test("Existe nodo con clave 'Prueba' es false", abb_pertenece(abb2, "Prueba") == false );
	print_test("Obtener dato de nodo con clave 'Prueba' es NULL", abb_obtener(abb2, "Prueba") == NULL );
	print_test("Guardar clave1 fue logrado exitosamente", abb_guardar(abb2, clave1, NULL) == true );
	print_test("Guardar clave2 fue logrado exitosamente", abb_guardar(abb2, clave2, NULL) == true );
	print_test("Guardar clave3 fue logrado exitosamente", abb_guardar(abb2, clave3, NULL) == true );
	print_test("Guardar clave4 fue logrado exitosamente", abb_guardar(abb2, clave4, NULL) == true );
	print_test("Guardar clave5 fue logrado exitosamente", abb_guardar(abb2, clave5, NULL) == true );
	print_test("Guardar clave6 fue logrado exitosamente", abb_guardar(abb2, clave6, NULL) == true );
	print_test("Guardar clave7 fue logrado exitosamente", abb_guardar(abb2, clave7, NULL) == true );
	print_test("Guardar clave8 fue logrado exitosamente", abb_guardar(abb2, clave8, NULL) == true );
	print_test("Arbol tiene 8 nodos", abb_cantidad(abb2) == 8);
	abb_iter_t* iter2 = abb_iter_in_crear(abb2);
	print_test("El iterador fue creado", iter2 != NULL);
	print_test("Primer clave es 'Alan'", strcmp(abb_iter_in_ver_actual(iter2), clave7) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Segunda clave es 'Barbara'", strcmp(abb_iter_in_ver_actual(iter2), clave8) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Tercera clave es 'Carlos'", strcmp(abb_iter_in_ver_actual(iter2), clave3) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Cuarta clave es 'Cristian'", strcmp(abb_iter_in_ver_actual(iter2), clave6) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Quinta clave es 'Guillermo'", strcmp(abb_iter_in_ver_actual(iter2), clave5) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Sexta clave es 'Ines'", strcmp(abb_iter_in_ver_actual(iter2), clave2) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Septima clave es 'Maria'", strcmp(abb_iter_in_ver_actual(iter2), clave1) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Avanzo con el iterador", abb_iter_in_avanzar(iter2) == true);
	print_test("Octava clave es 'Roberto'", strcmp(abb_iter_in_ver_actual(iter2), clave4) == 0);
	printf("%s\n", abb_iter_in_ver_actual(iter2));
	print_test("Ver actual con el iterador es NULL", abb_iter_in_ver_actual(iter2) == NULL);
	print_test("Avanzar con el iterador es false", abb_iter_in_avanzar(iter2) == false);
	print_test("El iterador esta al final", abb_iter_in_al_final(iter2) == true);
    abb_iter_in_destruir(iter2);
	abb_destruir(abb2);

	//Prueba de volumen modificada del abb
    prueba_abb_volumen(500, true);

}



