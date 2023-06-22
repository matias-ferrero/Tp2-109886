#ifndef TP2_H_
#define TP2_H_

#include "tp1.h"
#include "hash.h"
#include "lista.h"
#include "pokemon.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct opcion opcion_t;

typedef struct menu menu_t;

/**
 * La funcion a ejecutar asociada a una operacion.
 * Recibe un menu y no devuelve nada.
 */
typedef void (*menu_operacion_t)(menu_t *);

/**
 * Crea un menú. Recibe un elemento que sirve como contexto para el menú.
 * 
 * Devuelve el menú creado, o NULL en caso de error.
 */
menu_t *menu_crear(void *contenido);

/**
 * Agrega o actualiza una operacion en el menú.
 * 
 * Se recibe una funcion, no NULL, que ejecuta la operacion que se desea
 * agregar, un texto informativo que describe la funcion, y una clave que
 * se asocia a esta operacion.
 * 
 * La funcion guarda una copia de la primera letra de la clave, en mayuscula.
 * 
 * Devuelve el menu si pudo guardar la operacion, o NULL en caso de error.
 */
menu_t *menu_agregar(menu_t *menu, char *clave, char *texto,
 		     menu_operacion_t funcion);

/**
 * Busca y devuelve la operacion asociada a la clave dada, o NULL en caso de
 * que no exista la operacion en el menu (o en caso de error). 
 */
opcion_t *menu_obtener(menu_t *menu, char *clave);

/**
 * Recibe una operacion y ejecuta su funcion asociada, con el dato que se pasa
 * por parametro.
 */
void menu_ejecutar(opcion_t *operacion, void *dato);

/**
 * Devuelve la cantidad de operaciones almacenadas en el menu o 0 en
 * caso de error.
 */
size_t menu_cantidad(menu_t *menu);

/**
 * Muestra por pantalla todas las operaciones disponibles en el menu.
 */
void menu_mostrar(menu_t *menu);

/**
 * Devuelve el contenido almacenado en el menu, o NULL si no exite
 * (o en caso de error).
 */
void *menu_obtener_contenido(menu_t *menu);

/**
 * Intercambia el contenido almacenado en el menu por el nuevo contenido
 * pasado por parametro.
 * 
 * La funcion se encarga de destruir el contenido viejo, liberando su memoria
 * con la funcion destructora que se pasa por parametro, en caso de que esta
 * no sea NULL.
 * 
 * Devuelve el nuevo contenido almacenado en el menu. 
 */
void *menu_cambiar_contenido(menu_t *menu, void *nuevo,
			     void (*destruir)(void *));

/**
 * Destruye el menu liberando la memoria reservada.
 */
void menu_destruir(menu_t *menu);

/**
 * Destruye el menu liberando la memoria reservada y asegurandose de
 * invocar la funcion destructora con el contenido almacenado en el menu.
 */
void menu_destruir_todo(menu_t *menu, void (*f)(void *));

#endif // PILA_H_
