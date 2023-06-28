#ifndef MENU_H_
#define MENU_H_

#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct opcion opcion_t;

typedef struct menu menu_t;

/**
 * La función a ejecutar asociada a una operación.
 * Recibe un void* por parámetro y devuelve un entero.
 */
typedef int (*menu_operacion_t)(void *);

/**
 * Crea un menú. Recibe un elemento que sirve como contexto para el menú.
 * 
 * Devuelve el menú creado, o NULL en caso de error.
 */
menu_t *menu_crear(void *contenido);

/**
 * Devuelve el contenido almacenado en el menú, o NULL si no existe
 * (o en caso de error).
 */
void *menu_obtener_contenido(menu_t *menu);

/**
 * Intercambia el contenido almacenado en el menú por el nuevo contenido
 * pasado por parámetro.
 *
 * La función se encarga de destruir el contenido viejo, liberando su memoria
 * con la función destructora que se pasa por parámetro, en caso de que esta
 * no sea NULL.
 *
 * Devuelve el nuevo contenido almacenado en el menú.
 */
void *menu_cambiar_contenido(menu_t *menu, void *nuevo,
			     void (*destruir)(void *));

/**
 * Agrega o actualiza una operación en el menú asociado a la clave dada.
 *
 * Se recibe una función, no NULL, que ejecuta la operación que se desea
 * agregar, un texto informativo que describe la función, y una clave que
 * se asocia a esta operación.
 *
 * Si la clave ya existía, se reemplaza el texto y la función existente,
 * por las recibidas por parámetro.
 *
 * La funcion guarda una copia de la clave recibida en MAYÚSCULAS.
 * Todas las claves recibidas se guardan en mayúsculas.
 *
 * Devuelve el menú si pudo guardar la operación, o NULL en caso de error.
 */
menu_t *menu_agregar(menu_t *menu, char *clave, char *texto,
		     menu_operacion_t funcion);

/**
 * Busca la operación por el nombre clave dado y ejecuta su función asociada,
 * con el dato que se pasa por parámetro.
 *
 * La funcion se encarga de transformar en mayúsculas la clave recibida.
 *
 * Devuelve el resultado que devuelve la funcion de la operación recibida,
 * o -1 en caso que no exista la operación en el menú (o en caso de error).
 */
int menu_ejecutar_operacion(menu_t *menu, char *clave, void *dato);

/**
 * Recibe una operación y devuelve el texto informativo asociado, o NULL
 * en caso de error.
 */
char *obtener_informacion(opcion_t *operacion);

/**
 * Devuelve la cantidad de operaciones almacenadas en el menú o 0 en
 * caso de error.
 */
size_t menu_cantidad(menu_t *menu);

/**
 * Recorre el menú e invoca la función con cada operación guardada en el menú,
 * con su clave asociada como primer parámetro, la operación como segundo
 * parámetro, y un puntero auxiliar como tercer parámetro.
 *
 * Mientras que queden mas operaciones o la función retorne true, la iteración
 * continúa. Cuando no quedan mas operaciones o la función devuelve false, la
 * iteración se corta y la función principal retorna.
 *
 * Devuelve la cantidad de operaciones totales iteradas (la cantidad de veces
 * que fue invocada la función) o 0 en caso de error.
 */
size_t menu_con_cada_operacion(menu_t *menu,
			       bool (*f)(const char *nombre, void *opcion,
					 void *aux),
			       void *aux);

/**
 * Destruye el menú liberando la memoria reservada.
 */
void menu_destruir(menu_t *menu);

/**
 * Destruye el menú liberando la memoria reservada y asegurándose de
 * invocar la funcion destructora con el contenido almacenado en el menú.
 */
void menu_destruir_todo(menu_t *menu, void (*f)(void *contenido));

#endif // MENU_H_