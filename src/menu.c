#include "menu.h"
#include <ctype.h>

#define CAPACIDAD_MENU 15

struct opcion{
	menu_operacion_t operacion;
	char *informacion;
};

struct menu {
	hash_t *opciones;
	size_t cantidad;
	//lista_t *hospitales;
};

menu_t *menu_crear()
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->opciones = hash_crear(CAPACIDAD_MENU);
	if (!menu->opciones)
		menu_destruir(menu);
 
	menu->cantidad = hash_cantidad(menu->opciones);
	return menu;
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

menu_t *menu_agregar(menu_t *menu, char *clave, char *texto,
		     menu_operacion_t funcion)
{
	if (!menu || !clave || !texto)
		return NULL;

	*clave = (char)toupper(*clave);

	opcion_t *opcion = crear_opcion(texto, funcion);
	if (!opcion)
		return NULL;

	if (!hash_insertar(menu->opciones, clave, opcion, NULL))
		return NULL;

	menu->cantidad = hash_cantidad(menu->opciones);
	return menu;
}

bool buscar_operacion_similar(const char *clave, void *op, void *aux)
{
	op = op;
	char *palabra_clave = aux;
	if (!strcmp(clave, &palabra_clave[0])) {
		printf("Quisiste decir: %c?", *clave);
		return false;
	}
	return true;
}

bool menu_buscar(menu_t *menu, char *clave)
{
	if (!menu || !clave || !menu_cantidad_opciones(menu))
		return false;

	for (int i = 0;  i < strlen(clave); i++)
		clave[i] = (char)toupper(clave[i]);

	if (!strcmp(clave, "AYUDA"))
		return true;

	if (strcmp(clave, "EXIT"))
		return true;

	if (hash_contiene(menu->opciones, &clave[0]))
		return true;

	printf("No se encontro la operacion que pediste\n");
	hash_con_cada_clave(menu->opciones, buscar_operacion_similar, clave);
	return false;
}

size_t menu_cantidad_opciones(menu_t *menu)
{
	if (!menu)
		return 0;

	return menu->cantidad;
}

bool mostrar_opcion(const char *clave, void *op, void *aux)
{
	aux = aux;
	if (!clave)
		return false;

	opcion_t *opcion = op;
	printf("%c: %s\n", *clave, opcion->informacion);
	return true;
}

void menu_mostrar(menu_t *menu)
{
	if (!menu || !menu_cantidad_opciones(menu))
		return;

	size_t mostrados = hash_con_cada_clave(menu->opciones,
					       mostrar_opcion, NULL);
	if (mostrados != menu->cantidad)
		printf("Error desplejando el menu\n");
}

void destruir_opcion(void *op)
{
	if (!op)
		return;

	free(op);
}

void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	hash_destruir_todo(menu->opciones, destruir_opcion);
	free(menu);
}
