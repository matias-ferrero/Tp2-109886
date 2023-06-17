#ifndef TP2_H_
#define TP2_H_

#include "tp1.h"
#include "hash.h"
#include "lista.h"
#include "pokemon.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 
 */
typedef void (*menu_operacion_t)(void *, void *);

typedef struct opcion opcion_t;

typedef struct menu menu_t;


/**
 * 
 */
menu_t *menu_crear(void *contenido);

/**
 *
 */
menu_t *menu_agregar(menu_t *menu, char *clave, char *texto,
 		     menu_operacion_t funcion);

/**
 * 
 */
opcion_t *menu_obtener_operacion(menu_t *menu, char *clave);

/**
 *
 */
size_t menu_cantidad_opciones(menu_t *menu);

/**
 * 
 */
void menu_mostrar(menu_t *menu);

/**
 * 
 */
void *menu_obtener_contenido(menu_t *menu);

/**
 * 
 */
void *menu_cambiar_contenido(menu_t *menu, void *nuevo,
			     void (*f)(void *));

/**
 * 
 */
void menu_destruir(menu_t *menu);

/**
 * 
 */
void menu_destruir_todo(menu_t *menu, void (*f)(void *));

#endif // PILA_H_
