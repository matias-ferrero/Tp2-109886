#ifndef MENU_H_
#define MENU_H_

#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct opcion opcion_t;

typedef struct menu menu_t;

/**
 * La funcion a ejecutar asociada a una operacion.
 * Recibe un menu y no devuelve nada.
 */
typedef int (*menu_operacion_t)(void *);

/**
 * Crea un menú. Recibe un elemento que sirve como contexto para el menú.
 * 
 * Devuelve el menú creado, o NULL en caso de error.
 */
menu_t *menu_crear(void *contenido);

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
 * Agrega o actualiza una operacion en el menú asociado a la clave dada.
 * 
 * Se recibe una funcion, no NULL, que ejecuta la operacion que se desea
 * agregar, un texto informativo que describe la funcion, y una clave que
 * se asocia a esta operacion.
 * 
 * Si la clave ya existía, se reemplaza el texto y la funcion existente,
 * por las recibidas por parametro.
 * 
 * La funcion guarda una copia de la clave recibida en MAYUSCULAS.
 * Todas las claves recibidas se guardan en mayusculas.
 * 
 * Devuelve el menu si pudo guardar la operacion, o NULL en caso de error.
 */
menu_t *menu_agregar(menu_t *menu, char *clave, char *texto,
		     menu_operacion_t funcion);

/**
 * Devuelve la cantidad de operaciones almacenadas en el menu o 0 en
 * caso de error.
 */
size_t menu_cantidad(menu_t *menu);

/**
 * Busca y devuelve la operacion asociada a la clave dada, o NULL en caso de
 * que no exista la operacion en el menu (o en caso de error).
 * 
 * La funcion se encarga de transformar en mayusculas la clave recibida.
 */
opcion_t *menu_obtener(menu_t *menu, char *clave);

/**
 * Recibe una operacion y devuelve el texto informativo asociado, o NULL
 * en caso de error.
 */
char *obtener_informacion(opcion_t *operacion);

/**
 * Recibe una operacion y ejecuta su funcion asociada, con el dato que se pasa
 * por parametro.
 * 
 * Devuelve el resultado que devuelve la funcion de la operacion recibida,
 * o -1 en caso de error.
 */
int menu_ejecutar(opcion_t *operacion, void *dato);

/**
 * Recorre el menu e invoca la funcion con cada operacion guardada en el menu,
 * con su clave asociada como primer parametro, la operacion como segundo
 * parametro, y un puntero auxiliar como tercer parametro.
 * 
 * Mientras que queden mas operaciones o la funcion retorne true, la iteración
 * continúa. Cuando no quedan mas operaciones o la función devuelve false, la
 * iteración se corta y la función principal retorna.
 * 
 * Devuelve la cantidad de operaciones totales iteradas (la cantidad de veces
 * que fue invocada la función) o 0 en caso de error.
 */
size_t menu_con_cada_operacion(menu_t *menu,
			       bool (*f)(const char *clave, void *op,
					 void *aux),
			       void *aux);

/**
 * Destruye el menu liberando la memoria reservada.
 */
void menu_destruir(menu_t *menu);

/**
 * Destruye el menu liberando la memoria reservada y asegurandose de
 * invocar la funcion destructora con el contenido almacenado en el menu.
 */
void menu_destruir_todo(menu_t *menu, void (*f)(void *));

#endif // MENU_H_