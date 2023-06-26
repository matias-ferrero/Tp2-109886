#ifndef TP2_H_
#define TP2_H_

#include "src/menu.h"
#include "src/tp1.h"
#include "src/hash.h"
#include "src/lista.h"
#include "src/pokemon.h"

/**
 * Pide un archivo y un numero de identificacion para cargar un hospital al
 * menu. Crea el hospital con los pokemones del archivo y lo inserta en la
 * lista que contiene el menu.
 * 
 * Devuelve 0 si la operacion fue exitosa, -1 si la operacion fallo, o -2 en
 * caso de error, lo que termina el programa y destruye el menu. 
 */
int cargar_hospital(void *menu);

/**
 * Pide un numero de identificacion para activar y ejecutar operaciones
 * sobre un hospital. Si ya habia un hospital activo, la funcion se encarga
 * de desactivarlo.
 * 
 * Devuelve 0 si la operacion fue exitosa, -1 si la operacion fallo, o -2 en
 * caso de error, lo que termina el programa y destruye el menu. 
 */
int activar_hospital(void *menu);

/**
 * Muestra por pantalla todos los hospitales insertados en la lista del menu.
 * 
 * Devuelve 0 si la operacion fue exitosa, o -2 en caso de error, lo que
 * termina el programa y destruye el menu. 
 */
int mostrar_hospitales(void *menu);

/**
 * Destruye el hospital activo del menu liberando la memoria reservada.
 * 
 * Devuelve 0 si la operacion fue exitosa, -1 si la operacion fallo, o -2 en
 * caso de error, lo que termina el programa y destruye el menu. 
 */
int destruir_hospital(void *menu);

/**
 * Muestra los nombres de todos los pokemones del hospital activo.
 * 
 * Devuelve 0 si la operacion fue exitosa, -1 si la operacion fallo, o -2 en
 * caso de error, lo que termina el programa y destruye el menu. 
 */
int mostrar_pokemones(void *menu);

/**
 * Muestra detalladamente todos los pokemones del hospital activo.
 * 
 * Devuelve 0 si la operacion fue exitosa, -1 si la operacion fallo, o -2 en
 * caso de error, lo que termina el programa y destruye el menu. 
 */
int mostrar_pokemones_detallados(void *menu);

/**
 * Muestra todas las operaciones disponibles del menu.
 * 
 * Devuelve 0 si la operacion fue exitosa, -1 si la operacion fallo, o -2 en
 * caso de error, lo que termina el programa y destruye el menu. 
 */
int menu_mostrar(void *menu);

/**
 * Destruye el menu liberando la memoria reservada y devuelve -2,
 * lo que termina el programa. 
 */
int menu_salir(void *menu);

#endif // TP2_H_
