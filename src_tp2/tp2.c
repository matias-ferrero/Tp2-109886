#include "src_tp2/tp2.h"

#define CAPACIDAD_OPCIONES 15

struct opciones{
	hash_t *operaciones;
	size_t cantidad;
};

struct menu {
	opciones_t *opciones;
};

menu_t *crear_menu()
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;

	opciones_t *opciones;
	opciones->operaciones = hash_crear(CAPACIDAD_OPCIONES);
	if (!opciones->operaciones)
		destruir_menu(menu);
 
	opciones->cantidad = hash_cantidad(opciones->operaciones);
	menu->opciones = opciones;

	return menu;
}

void agregar_operacion(opciones_t *opciones, char *clave, char *texto)
{
	hash_insertar(opciones->operaciones, clave, texto, NULL);
	opciones->cantidad = hash_cantidad(opciones->operaciones);
}

size_t cantidad_operaciones(opciones_t *opciones)
{
	if (!opciones)
		return 0;

	return opciones->cantidad;
}

bool mostrar_operaciones(char *clave, void *informacion, void *aux)
{
	aux = aux;
	char *texto = informacion;
	printf("%s: %s\n", clave, texto);
	return true;
}

void mostrar_menu(menu_t *menu)
{
	if (!menu)
		return;

	hash_con_cada_clave(menu->opciones->operaciones,
			    mostrar_operaciones, NULL);
}

void destruir_menu(menu_t *menu)
{
	if (!menu)
		return;

	hash_destruir(menu->opciones->operaciones);
	free(menu);
}

int main()
{
	menu_t *menu = crear_menu();

	//Mostrar menu
	//Interactuar con el usuario

	destruir_menu(menu);

	return 0;
}
