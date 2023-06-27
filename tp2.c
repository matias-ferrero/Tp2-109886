#include "tp2.h"
#include "src/menu.h"
#include "src/tp1.h"
#include "src/hash.h"
#include "src/lista.h"
#include "src/pokemon.h"

#define ERROR -1
#define EXITO 0
#define TERMINAR -2
#define MAX_NOMBRE 50
#define MAX_ARCHIVO 200

typedef struct nodo_menu {
	size_t id;
	bool activo;
	char nombre_archivo[MAX_ARCHIVO];
	hospital_t *hospital;
} nodo_menu_t;

int buscar_hospital_activo(void *elemento, void *pos)
{
	if (!elemento || !pos)
		return -1;

	size_t *posicion = (size_t *)pos;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;

	if (nodo->activo)
		return 0;

	*posicion = *posicion + 1;
	return (int)*posicion;
}

/**
 * ----------------------------------------------------------------------------
 */

int cargar_hospital(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	nodo_menu_t *nodo = malloc(sizeof(nodo_menu_t));
	if (!nodo) {
		printf("Error\n");
		return TERMINAR;
	}

	printf("Ingrese el directorio del archivo del hospital\n");
	if (!fgets(nodo->nombre_archivo, MAX_ARCHIVO, stdin)) {
		printf("Archivo invalido\n");
		return true;
	}
	nodo->nombre_archivo[strlen(nodo->nombre_archivo) - 1] = 0;

	nodo->hospital = hospital_crear_desde_archivo(nodo->nombre_archivo);
	if (!nodo->hospital) {
		printf("Error al crear hospital\n");
		free(nodo);
		return ERROR;
	}

	size_t id;
	printf("Ingrese el id del hospital\n");
	scanf("%zu", &id);

	nodo->id = id;
	nodo->activo = false;

	if (!lista_insertar(menu_obtener_contenido(menu), nodo)) {
		printf("Error al guardar el hospital\n");
		hospital_destruir(nodo->hospital);
		free(nodo);
		return ERROR;
	}
	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

int comparador(void *elemento, void *aux)
{
	if (!elemento || !aux)
		return -1;

	size_t *id = (size_t *)aux;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;

	return (int)(*id - nodo->id);
}

bool desactivar_hospital(void *elemento, void *aux)
{
	if (!elemento)
		return true;

	aux = aux;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;
	if (nodo->activo) {
		nodo->activo = false;
		return false;
	}

	return true;
}

int activar_hospital(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;

	size_t id;
	printf("Ingrese el id del hospital que quiere activar\n");
	scanf("%zu", &id);

	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo = lista_buscar_elemento(lista, comparador, &id);
	if (!nodo) {
		printf("No se encontro el hospital con id N°%zu\n", id);
		return ERROR;
	}

	if (nodo->activo)
		return EXITO;

	lista_con_cada_elemento(lista, desactivar_hospital, NULL);
	nodo->activo = true;
	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

bool mostrar_hospital(void *elemento, void *aux)
{
	if (!elemento)
		return false;

	aux = aux;
	nodo_menu_t *nodo = (nodo_menu_t *)elemento;

	if (nodo->activo)
		printf("Hospital N°%zu: Activo\n", nodo->id);
	else
		printf("Hospital N°%zu: No activo\n", nodo->id);

	return true;
}

int mostrar_hospitales(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	lista_t *lista = menu_obtener_contenido(menu);
	if (!lista_con_cada_elemento(lista, mostrar_hospital, NULL))
		printf("No hay hospitales cargados\n");

	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

int destruir_hospital(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	size_t posicion = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo;
	nodo = lista_buscar_elemento(lista, buscar_hospital_activo, &posicion);
	if (!nodo) {
		printf("Error, No hay un hospital activo\n");
		return ERROR;
	}

	hospital_destruir(nodo->hospital);
	lista_quitar_de_posicion(lista, posicion);
	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

bool mostrar_pokemon(pokemon_t *pokemon, void *contador)
{
	if (!pokemon || !contador)
		return false;

	size_t *numero = contador;
	*numero = *numero + 1;
	printf("%zu) %s\n", *numero, pokemon_nombre(pokemon));
	return true;
}

int mostrar_pokemones(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	size_t contador = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo;
	nodo = lista_buscar_elemento(lista, buscar_hospital_activo, &contador);
	if (!nodo) {
		printf("Error al buscar un Hospital activo\n");
		return ERROR;
	}

	contador = 0;
	hospital_a_cada_pokemon(nodo->hospital, mostrar_pokemon, &contador);
	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

bool mostrar_pokemon_detallado(pokemon_t *pokemon, void *contador)
{
	if (!pokemon || !contador)
		return false;

	size_t *numero = contador;
	*numero = *numero + 1;
	printf("%zu) %s - Id: N°%zu - Salud: %zu - Entrenador: %s\n", *numero,
	       pokemon_nombre(pokemon), pokemon_id(pokemon),
	       pokemon_salud(pokemon), pokemon_entrenador(pokemon));

	return true;
}

int mostrar_pokemones_detallados(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	size_t contador = 0;
	lista_t *lista = menu_obtener_contenido(menu);
	nodo_menu_t *nodo;
	nodo = lista_buscar_elemento(lista, buscar_hospital_activo, &contador);
	if (!nodo)
		return ERROR;

	contador = 0;
	hospital_a_cada_pokemon(nodo->hospital, mostrar_pokemon_detallado,
				&contador);
	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

bool mostrar_opcion(const char *nombre, void *op, void *aux)
{
	aux = aux;
	if (!nombre || !op)
		return false;

	opcion_t *opcion = op;
	printf("%s: %s\n", nombre, obtener_informacion(opcion));
	return true;
}

int menu_mostrar(void *menu)
{
	if (!menu)
		return TERMINAR;

	if (!menu_con_cada_operacion((menu_t *)menu, mostrar_opcion, NULL))
		return ERROR;

	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

void destruir_nodo(void *n)
{
	if (!n)
		return;

	nodo_menu_t *nodo = (nodo_menu_t *)n;
	hospital_destruir(nodo->hospital);
	free(nodo);
}

void destruir_lista(void *lista)
{
	if (!lista)
		return;

	lista_destruir_todo((lista_t *)lista, destruir_nodo);
}

int menu_salir(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	menu_destruir_todo(menu, destruir_lista);
	return TERMINAR;
}

/**
 * ----------------------------------------------------------------------------
 */

bool menu_interactuar(menu_t *menu)
{
	if (!menu)
		return true;

	char buffer[MAX_NOMBRE];
	printf("Ingrese la operacion que quiera realizar:\n");
	if (!fgets(buffer, MAX_NOMBRE, stdin)) {
		printf("Operacion invalida\n");
		return true;
	}

	buffer[strlen(buffer) - 1] = 0;
	int resultado = menu_ejecutar_operacion(menu, buffer, menu);

	if (resultado == TERMINAR)
		return true;

	if (resultado == ERROR)
		printf("H para ayuda\n");
	else if (resultado == EXITO)
		printf("Operacion exitosa\n");

	return false;
}

/**
 * ----------------------------------------------------------------------------
 */

int main()
{
	menu_t *menu = menu_crear(lista_crear());
	if (!menu) {
		printf("Error al crear el Menu\n");
		return -1;
	}

	menu_agregar(menu, "C", "Carga un hospital", cargar_hospital);
	menu_agregar(menu, "cargar", "Carga un hospital", cargar_hospital);

	menu_agregar(menu, "A", "Activa un hospital", activar_hospital);
	menu_agregar(menu, "activar", "Activa un hospital", activar_hospital);

	menu_agregar(menu, "E", "Muestra los hospitales cargados",
		     mostrar_hospitales);
	menu_agregar(menu, "estado", "Muestra los hospitales cargados",
		     mostrar_hospitales);

	menu_agregar(menu, "D", "Destruye el hospital activo",
		     destruir_hospital);
	menu_agregar(menu, "destruir", "Destruye el hospital activo",
		     destruir_hospital);

	menu_agregar(menu, "M", "Muestra los pokemones del hospital activo",
		     mostrar_pokemones);
	menu_agregar(menu, "mostrar",
		     "Muestra los pokemones del hospital activo",
		     mostrar_pokemones);

	menu_agregar(menu, "L",
		     "Muestra los pokemones detallados del hospital activo",
		     mostrar_pokemones_detallados);
	menu_agregar(menu, "listar",
		     "Muestra los pokemones detallados del hospital activo",
		     mostrar_pokemones_detallados);

	menu_agregar(menu, "H", "Muestra los comandos disponibles",
		     menu_mostrar);
	menu_agregar(menu, "ayuda", "Muestra los comandos disponibles",
		     menu_mostrar);
	menu_agregar(menu, "help", "Muestra los comandos disponibles",
		     menu_mostrar);

	menu_agregar(menu, "S", "Salir del menu", menu_salir);
	menu_agregar(menu, "salir", "Salir del menu", menu_salir);
	menu_agregar(menu, "exit", "Salir del menu", menu_salir);

	menu_mostrar(menu);

	bool salir = false;
	while (!salir) {
		salir = menu_interactuar(menu);
		printf("\n");
	}

	//menu_salir(menu);

	return 0;
}