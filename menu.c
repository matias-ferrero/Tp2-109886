#include "menu.h"

#define CAPACIDAD_MENU 15

struct operacion{
	void (*funcion)(void *, void *);
	char *informacion;
};

struct menu {
	hash_t *operaciones;
	size_t cantidad;
	lista_t *hospitales;
};

menu_t *crear_menu()
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->operaciones = hash_crear(CAPACIDAD_MENU);
	if (!menu->operaciones)
		destruir_menu(menu);
 
	menu->cantidad = hash_cantidad(menu->operaciones);
	return menu;
}

operacion_t *crear_operacion(char *texto, void (*funcion)(void *,void *))
{
	operacion_t *operacion = malloc(sizeof(operacion_t));
	if (!operacion)
		return NULL;

	operacion->funcion = funcion;
	strcpy(operacion->informacion, texto);
	return operacion;
}

menu_t *agregar_operacion(menu_t *menu, char *clave, char *texto,
			  void (*funcion)(void*, void *))
{
	operacion_t *operacion = crear_operacion(texto, funcion);
	if (!operacion)
		return NULL;

	if (!hash_insertar(menu->operaciones, clave, operacion, NULL))
		return NULL;

	menu->cantidad = hash_cantidad(menu->operaciones);
	return menu;
}

size_t cantidad_operaciones(menu_t *menu)
{
	if (!menu)
		return 0;

	return menu->cantidad;
}

bool mostrar_operacion(const char *clave, void *op, void *aux)	//REVISAR: CUANDO DEVUELVE FALSE
{
	aux = aux;
	if (!clave)
		return false;

	operacion_t *operacion = op;
	printf("%s: %s\n", clave, operacion->informacion);
	return true;
}

void mostrar_menu(menu_t *menu)
{
	if (!menu)
		return;

	size_t mostrados = hash_con_cada_clave(menu->operaciones,
					       mostrar_operacion, NULL);
	if (mostrados != menu->cantidad)
		printf("Error desplejando el menu\n");
}

void destruir_operaciones(void *op)
{
	if (!op)
		return;

	free(op);
}

void destruir_menu(menu_t *menu)
{
	if (!menu)
		return;

	hash_destruir_todo(menu->operaciones, destruir_operaciones);
	free(menu);
}
