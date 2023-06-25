#ifndef TP2_H_
#define TP2_H_

#include "src/menu.h"
#include "src/tp1.h"
#include "src/hash.h"
#include "src/lista.h"
#include "src/pokemon.h"

/**
 * Pide un archivo y un numero de identificacion para cargar un hospital
 * al menu.
 * Crea el hospital con los pokemones del archivo y lo inserta en la lista
 * del menu.
 */
int cargar_hospital(menu_t *menu);

/**
 * Pide un numero de identificacion para activar y ejecutar operaciones
 * sobre un hospital.
 */
int activar_hospital(menu_t *menu);

/**
 * Muestra por pantalla todos los hospitales insertados en la lista del menu.
 */
int mostrar_hospitales(menu_t *menu);

/**
 * Destruye el hospital activo del menu.
 */
int destruir_hospital(menu_t *menu);

/**
 * Muestra los nombres de todos los pokemones del hospital activo.
 */
int mostrar_pokemones(menu_t *menu);

/**
 * Muestra detalladamente todos los pokemones del hospital activo.
 */
int mostrar_pokemones_detallados(menu_t *menu);

#endif // TP2_H_
