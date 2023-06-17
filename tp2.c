#include "menu.h"

typedef struct nodo_menu {
	hospital_t *hospital;
	size_t id;
} nodo_menu_t;

menu_t *menu_cargar_hospital(menu_t *menu, const char *archivo, size_t id)
{
	if (!menu || !archivo)
		return NULL;

	hospital_t *hospital  = hospital_crear_desde_archivo(archivo);
	nodo_menu_t *nodo = malloc(sizeof(nodo_menu_t));
	if (!hospital || !nodo) {
		hospital_destruir(hospital);
		free(nodo);
		return NULL;
	}

	nodo->hospital = hospital;
	nodo->id = id;
/*
	if (!lista_insertar(menu->hospitales, nodo)) {
		hospital_destruir(hospital);
		free(nodo);
		return NULL;
	}
*/
	return menu;
}

void destruir_nodo(void *n)
{
	if (!n)
		return;

	nodo_menu_t *nodo = n;
	free(nodo->hospital);
	free(nodo);
}

void menu_interactuar(menu_t *menu)
{
	
}

int main()
{
	menu_t *menu = crear_menu();

	//menu_agregar();

	menu_mostrar(menu);
	
	menu_interactuar(menu);

	destruir_menu(menu);

	return 0;
}
