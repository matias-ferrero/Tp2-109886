#ifndef TP2_H_
#define TP2_H_

#include "src/tp1.h"
#include "src/hash.h"
#include "src/lista.h"
#include "src/pokemon.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct opciones opciones_t;

typedef struct menu menu_t;


/**
 * 
 */
menu_t *crear_menu();

/**
 *
 */
void agregar_operacion();

/**
 *
 */
size_t cantidad_operaciones(opciones_t *opciones);

/**
 * 
 */
void mostrar_menu(menu_t *menu);

/**
 * 
 */
void destruir_menu(menu_t *menu);

#endif // PILA_H_
