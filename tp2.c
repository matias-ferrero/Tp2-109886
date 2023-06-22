#include "src/menu.h"

#define MAX_CLAVE 25
#define MAX_ARCHIVO 200

typedef struct nodo_menu {
	hospital_t *hospital;
	size_t id;
	bool activo;
} nodo_menu_t;

menu_t *cargar_hospital(menu_t *menu)
{
	if (!menu)
		return NULL;

	char archivo[MAX_ARCHIVO];
	printf("Ingrese el directorio del archivo del hospital\n");
	fgets(archivo, MAX_ARCHIVO, stdin);

	size_t id;
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

	if (!lista_insertar(menu_obtener_contenido(menu), nodo)) {
		hospital_destruir(hospital);
		free(nodo);
		return NULL;
	}

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
		printf("%zu) Hospital N°%zu: Activo", *contador, nodo->id);
	else
		printf("%zu) Hospital N°%zu: No activo", *contador, nodo->id);

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

int buscar_posicion_hospital_activo(void *elemento, void *pos)
{
	if (!elemento || !pos)
		return -1;

	size_t *posicion = pos;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;

	if (nodo->activo)
		return 0;

	*(posicion++);
	return -1;
}

void destruir_hospital(menu_t *menu)
{
	if (!menu)
		return;

	size_t posicion = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo;
	nodo = lista_buscar_elemento(lista,
				     buscar_posicion_hospital_activo,
				     &posicion);
	if (!nodo)
		return;

	lista_quitar_de_posicion(lista, posicion);
}

bool mostrar_pokemon(void *elemento, void *contador)
{
	if (!elemento || !contador)
		return false;

	size_t *numero = contador;
	*(contador++);
	pokemon_t *pokemon = (pokemon_t *)elemento;
	printf("%zu) %s\n", *numero, pokemon_nombre(pokemon));
	return true;
}

void mostrar_pokemones(menu_t *menu)
{
	size_t id;
	printf("Ingrese el id del hospital que quiere activar\n");
	scanf("%zu", &id);

	size_t contador = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo = lista_buscar_elemento(lista, comparador, &id);
	if (!nodo)
		return;

	hospital_a_cada_pokemon(nodo->hospital, mostrar_pokemon, &contador);
}

bool mostrar_pokemon_detallado(void *elemento, void *contador)
{
	if (!elemento || !contador)
		return false;

	size_t *numero = contador;
	*(contador++);
	pokemon_t *pokemon = (pokemon_t *)elemento;
	printf("%zu) %s - Id: %zu - Salud: %zu - Entrenador: %zu\n", *numero,
	       pokemon_nombre(pokemon), pokemon_id(pokemon),
	       pokemon_salud(pokemon), pokemon_entrenador(pokemon));

	return true;
}

void mostrar_pokemones_detallados(menu_t *menu)
{
	size_t id;
	printf("Ingrese el id del hospital que quiere activar\n");
	scanf("%zu", &id);

	size_t contador = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo = lista_buscar_elemento(lista, comparador, &id);
	if (!nodo)
		return;

	hospital_a_cada_pokemon(nodo->hospital, mostrar_pokemon_detallado, &contador);
}

void destruir_nodo(void *n)
{
	if (!n)
		return;

	nodo_menu_t *nodo = n;
	hospital_destruir(nodo->hospital);
	free(nodo);
}

void salir(menu_t *menu)
{
	if (!menu)
		return;

	menu_destruir_todo(menu, destruir_nodo);
}

int main()
{
	char buffer[MAX_CLAVE];
	menu_t *menu = menu_crear(lista_crear());
	if (!menu) {
		printf("Error al crear el Menu\n");
		return -1;
	}

	menu_agregar(menu, "C", "Cargar un hospital", cargar_hospital);
	menu_agregar(menu, "A", "Activar un hospital", activar_hospital);
	menu_agregar(menu, "E", "Mostrar los hospitales", mostrar_hospitales);
	menu_agregar(menu, "D", "Destruir hospital activo", destruir_hospital);
	menu_agregar(menu, "M", "Mostrar los pokemones de un hospital",
		     mostrar_pokemones);
	menu_agregar(menu, "L",
		     "Mostrar los pokemones detallados de un hospital",
		     mostrar_pokemones_detallados);
	menu_agregar(menu, "H", "Mostrar los comandos", menu_mostrar);
	menu_agregar(menu, "S", "Salir del menu", salir);

	if (!menu) {
		printf("Error al agregar operaciones al Menu\n");
		return -1;
	}
	menu_mostrar(menu);

	while (menu != NULL) {
		printf("Ingrese la operacion que quiera realizar:\n");
		fgets(buffer, MAX_CLAVE, stdin);
		opcion_t *opcion = menu_obtener(menu, buffer);
		if (!opcion)
			printf("No se encontro la operacion\n");

		menu_ejecutar(opcion, menu_obtener_contenido(menu));
	}

	salir(menu);

	return 0;
}
