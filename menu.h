#ifndef TP2_H_
#define TP2_H_

#include "src/tp1.h"
#include "src/hash.h"
#include "src/lista.h"
#include "src/pokemon.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct operacion operacion_t;

typedef struct menu menu_t;


/**
 * 
 */
menu_t *crear_menu();

/**
 *
 */
menu_t *agregar_operacion(menu_t *menu, char *clave, char *texto, void (*funcion)(void*, void *));

/**
 *
 */
size_t cantidad_operaciones(menu_t *menu);

/**
 * 
 */
void mostrar_menu(menu_t *menu);

/**
 * 
 */
void destruir_menu(menu_t *menu);

#endif // PILA_H_
