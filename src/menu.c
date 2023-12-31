#include "menu.h"
#include <ctype.h>

#define ERROR -1
#define CAPACIDAD_MENU 15
#define MAX_NOMBRE 50

struct opcion {
	menu_operacion_t operacion;
	char *informacion;
};

struct menu {
	hash_t *opciones;
	void *contenido;
};

menu_t *menu_crear(void *contexto)
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->contenido = contexto;
	menu->opciones = hash_crear(CAPACIDAD_MENU);
	if (!menu->opciones) {
		menu_destruir(menu);
		return NULL;
	}

	return menu;
}

void *menu_obtener_contenido(menu_t *menu)
{
	if (!menu)
		return NULL;

	return menu->contenido;
}

void *menu_cambiar_contenido(menu_t *menu, void *nuevo,
			     void (*destruir)(void *))
{
	if (!menu || !nuevo)
		return NULL;

	if (destruir != NULL)
		destruir(menu->contenido);

	menu->contenido = nuevo;

	return menu->contenido;
}

opcion_t *crear_opcion(char *texto, menu_operacion_t funcion)
{
	opcion_t *opcion = malloc(sizeof(opcion_t));
	if (!opcion)
		return NULL;

	opcion->operacion = funcion;
	opcion->informacion = texto;
	return opcion;
}

menu_t *menu_agregar(menu_t *menu, char *nombre_clave, char *texto,
		     menu_operacion_t funcion)
{
	if (!menu || !nombre_clave || !texto || !funcion)
		return NULL;

	char nombre[MAX_NOMBRE];
	memset(nombre, 0, MAX_NOMBRE);
	for (size_t i = 0; i < strlen(nombre_clave); i++)
		nombre[i] = (char)toupper(nombre_clave[i]);

	opcion_t *actual = hash_obtener(menu->opciones, nombre);
	if (actual != NULL) {
		actual->informacion = texto;
		actual->operacion = funcion;
		return menu;
	}

	opcion_t *opcion = crear_opcion(texto, funcion);
	if (!opcion)
		return NULL;

	if (!hash_insertar(menu->opciones, nombre, opcion, NULL))
		return NULL;

	return menu;
}

int menu_ejecutar_operacion(menu_t *menu, char *nombre_clave, void *dato)
{
	if (!menu || !nombre_clave || !menu_cantidad(menu))
		return ERROR;

	char nombre[MAX_NOMBRE];
	memset(nombre, 0, MAX_NOMBRE);
	for (int i = 0; i < strlen(nombre_clave); i++)
		nombre[i] = (char)toupper(nombre_clave[i]);

	opcion_t *opcion = hash_obtener(menu->opciones, nombre);
	if (!opcion)
		return ERROR;

	return opcion->operacion(dato);
}

char *obtener_informacion(opcion_t *opcion)
{
	if (!opcion)
		return NULL;

	return opcion->informacion;
}

size_t menu_cantidad(menu_t *menu)
{
	if (!menu)
		return 0;

	return hash_cantidad(menu->opciones);
}

size_t menu_con_cada_operacion(menu_t *menu,
			       bool (*f)(const char *nombre, void *opcion,
					 void *aux),
			       void *aux)
{
	if (!menu || !f || !menu_cantidad(menu))
		return 0;

	return hash_con_cada_clave(menu->opciones, f, aux);
}

void destruir_opcion(void *opcion)
{
	if (!opcion)
		return;

	free(opcion);
}

void menu_destruir(menu_t *menu)
{
	menu_destruir_todo(menu, NULL);
}

void menu_destruir_todo(menu_t *menu, void (*destruir)(void *contenido))
{
	if (!menu)
		return;

	if (destruir != NULL)
		destruir(menu->contenido);

	hash_destruir_todo(menu->opciones, destruir_opcion);
	free(menu);
}