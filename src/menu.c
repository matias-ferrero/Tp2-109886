#include "menu.h"
#include <ctype.h>

#define ERROR -1
#define CAPACIDAD_MENU 15

struct opcion{
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

opcion_t *crear_opcion(char *texto, menu_operacion_t funcion)
{
	opcion_t *opcion = malloc(sizeof(opcion_t));
	if (!opcion)
		return NULL;

	opcion->operacion = funcion;
	opcion->informacion = texto;
	return opcion;
}

menu_t *menu_agregar(menu_t *menu, char *clave, char *texto,	//AGREGAR COMANDOS DE UNA SOLA LETRA
		     menu_operacion_t funcion)
{
	if (!menu || !clave || !texto || !funcion)
		return NULL;

	*clave = (char)toupper(*clave);
	if (hash_contiene(menu->opciones, &clave[0])) {
		printf("Operacion ya existente\n");
		return NULL;
	}

	opcion_t *opcion = crear_opcion(texto, funcion);
	if (!opcion)
		return NULL;

	if (!hash_insertar(menu->opciones, &clave[0], opcion, NULL))
		return NULL;

	return menu;
}

opcion_t *menu_obtener(menu_t *menu, char *clave)
{
	if (!menu || !clave || !menu_cantidad(menu))
		return NULL;

	for (int i = 0;  i < strlen(clave); i++)
		clave[i] = (char)toupper(clave[i]);

	return hash_obtener(menu->opciones, clave);
}

int menu_ejecutar(opcion_t *operacion, void *dato)
{
	if (!operacion || !operacion->operacion)
		return ERROR;

	return operacion->operacion(dato);
}

size_t menu_cantidad(menu_t *menu)
{
	if (!menu)
		return 0;

	return hash_cantidad(menu->opciones);
}

bool mostrar_opcion(const char *clave, void *op, void *aux)
{
	aux = aux;
	if (!clave)
		return false;

	opcion_t *opcion = op;
	printf("%c: %s\n", clave[0], opcion->informacion);
	return true;
}

int menu_mostrar(menu_t *menu)
{
	return (int)menu_con_cada_operacion(menu, mostrar_opcion, NULL);
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

size_t menu_con_cada_operacion(menu_t *menu,
			   bool (*f)(const char *clave, void *op, void *aux),
			   void *aux) {
	if (!menu)
		return 0;

	return hash_con_cada_clave(menu->opciones, f, aux);
}

void destruir_opcion(void *op)
{
	if (!op)
		return;

	free(op);
}

void menu_destruir(menu_t *menu)
{
	menu_destruir_todo(menu, NULL);
}

void menu_destruir_todo(menu_t *menu, void (*destruir)(void *))
{
	if (!menu)
		return;

	if (destruir != NULL)
		destruir(menu->contenido);

	hash_destruir_todo(menu->opciones, destruir_opcion);
	free(menu);
}
