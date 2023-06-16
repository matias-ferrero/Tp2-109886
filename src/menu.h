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
menu_t *menu_crear();

/**
 *
 */
menu_t *menu_agregar(menu_t *menu, char *clave, char *texto, menu_operacion_t funcion);

/**
 * 
 */
opcion_t *menu_obtener(menu_t *menu, char *clave);

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
void menu_destruir(menu_t *menu);

#endif // PILA_H_
