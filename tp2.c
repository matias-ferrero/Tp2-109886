#include "src/menu.h"

#define MAX_ARCHIVO 100

typedef struct nodo_menu {
	hospital_t *hospital;
	size_t id;
	bool activo;
} nodo_menu_t;

menu_t *cargar_hospital(menu_t *menu)
{
	if (!menu)
		return NULL;

	size_t id;
	char archivo[MAX_ARCHIVO];
	printf("Ingrese el directorio del archivo del hospital\n");
	fgets(archivo, MAX_ARCHIVO, stdin);
	printf("Ingrese el id del hospital\n");
	scanf("%zu", &id);

	hospital_t *hospital  = hospital_crear_desde_archivo(archivo);
	nodo_menu_t *nodo = malloc(sizeof(nodo_menu_t));
	if (!hospital || !nodo) {
		hospital_destruir(hospital);
		free(nodo);
		return NULL;
	}

	nodo->id = id;
	nodo->activo = false;
	nodo->hospital = hospital;
/*
	if (!lista_insertar(menu->hospitales, nodo)) {
		hospital_destruir(hospital);
		free(nodo);
		return NULL;
	}
*/
	return menu;
}

int comparador(void *elemento, void *num)
{
	if (!elemento || !num)
		return -1;

	size_t *id = (size_t *)num;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;

	return *id - nodo->id;
}

bool desactivar_hospital(void *elemento, void *aux)
{
	if (!elemento)
		return true;

	aux = aux;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;
	nodo->activo = false;
	return true;
}

void activar_hospital(menu_t *menu)
{
	if (!menu)
		return;

	size_t id;
	printf("Ingrese el id del hospital que quiere activar\n");
	scanf("%zu", &id);

	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo = lista_buscar_elemento(lista, comparador, &id);
	if (!nodo)
		return;

	lista_con_cada_elemento(lista, desactivar_hospital, NULL);
	nodo->activo = true;
}

bool mostrar_hospital(void *elemento, void *num)
{
	if (!elemento || !num)
		return false;

	size_t *contador = num;
	*(contador++);
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;

	if (nodo->activo)
		printf("%zu- Hospital N°%zu: Activo", *contador, nodo->id);
	else
		printf("%zu- Hospital N°%zu: No activo", *contador, nodo->id);

	return true;
}

void mostrar_hospitales(menu_t *menu)
{
	if (!menu)
		return;

	size_t contador = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	lista_con_cada_elemento(lista, mostrar_hospital, &contador);
}

void destruir_nodo(void *n)
{
	if (!n)
		return;

	nodo_menu_t *nodo = n;
	free(nodo->hospital);
	free(nodo);
}

int main()
{
	menu_t *menu = crear_menu(lista_crear());

	menu_agregar(menu, "C", "Cargar un hospital", cargar_hospital);
	menu_agregar(menu, "A", "Activar un hospital", activar_hospital);
	menu_agregar(menu, "E", "Mostrar los hospitales", mostrar_hospitales);
/*
	menu_agregar(menu, "D", "Destruir el hospital activo", funcion);
	menu_agregar(menu, "L", "Mostrar la lista detallada de pokemones de un hospital", funcion);
	menu_agregar(menu, "M", "Mostrar la lista de nombres de pokemones de un hospital", funcion);
*/
	menu_agregar(menu, "H", "Mostrar los comandos disponibles", menu_mostrar);
	menu_agregar(menu, "S", "Salir del menu", menu_destruir_todo);


	menu_mostrar(menu);
	
	menu_interactuar(menu);

	destruir_menu(menu);

	return 0;
}
