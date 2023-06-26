#include "src/menu.h"
#include "tp2.h"

#define TERMINAR -2
#define MAX_CLAVE 25
#define MAX_ARCHIVO 200

typedef struct nodo_menu {
	size_t id;
	bool activo;
	char *nombre_archivo;
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

	*(posicion++);
	return -1;
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
	fgets(nodo->nombre_archivo, MAX_ARCHIVO, stdin);

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

	return *id - nodo->id;
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
	lista_con_cada_elemento(lista, mostrar_hospital, NULL);

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

	lista_quitar_de_posicion(lista, posicion);
	return EXITO;
}

/**
 * ----------------------------------------------------------------------------
 */

bool mostrar_pokemon(void *elemento, void *contador)
{
	if (!elemento || !contador)
		return false;

	size_t *numero = contador;
	*(numero++);
	pokemon_t *pokemon = (pokemon_t *)elemento;
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

bool mostrar_pokemon_detallado(void *elemento, void *contador)
{
	if (!elemento || !contador)
		return false;

	size_t *numero = contador;
	*(contador++);
	pokemon_t *pokemon = (pokemon_t *)elemento;
	printf("%zu) %s - Id: N°%zu - Salud: %zu - Entrenador: %zu\n", *numero,
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

bool mostrar_opcion(const char *clave, void *op, void *aux)
{
	aux = aux;
	if (!clave || !op)
		return false;

	opcion_t *opcion = op;
	printf("%s: %s\n", clave, obtener_informacion(opcion));
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

	nodo_menu_t *nodo = n;
	hospital_destruir(nodo->hospital);
	free(nodo);
}

int menu_salir(void *menu1)
{
	if (!menu1)
		return TERMINAR;

	menu_t *menu = (menu_t *)menu1;
	menu_destruir_todo(menu, destruir_nodo);
	return TERMINAR;
}

/**
 * ----------------------------------------------------------------------------
 */

bool comparar_claves(const char *clave, void *op, void *aux)
{
	if (!clave || !aux)
		return true;

	char *palabra = (char *)aux;
	opcion_t *opcion = op;

	if (clave[0] == palabra[0]) {
		printf("Quisiste decir la operacion %c?\n", *clave);
		return false;
	}

	return true;
}

void buscar_operaciones_similares(menu_t *menu, char *clave)
{
	menu_con_cada_operacion(menu, comparar_claves, &clave);
}

bool menu_interactuar(menu_t *menu)
{
	if (!menu)
		return true;

	char buffer[MAX_CLAVE];
	printf("Ingrese la operacion que quiera realizar:\n");
	fgets(buffer, MAX_CLAVE, stdin);

	opcion_t *opcion = menu_obtener(menu, buffer);
	int resultado = menu_ejecutar(opcion, menu_obtener_contenido(menu));

	if (resultado == TERMINAR)
		return true;

	if (resultado == ERROR) {
		if (!opcion) {
			printf("No se encontro la operacion\n");
			buscar_operaciones_similares(menu, buffer);
		}
		printf("H para ayuda\n");
	}
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

	menu_agregar(menu, "C", "Cargar un hospital", cargar_hospital);
	menu_agregar(menu, "cargar", "Cargar un hospital", cargar_hospital);

	menu_agregar(menu, "A", "Activar un hospital", activar_hospital);
	menu_agregar(menu, "activar", "Activar un hospital", activar_hospital);

	menu_agregar(menu, "E", "Mostrar los hospitales", mostrar_hospitales);
	menu_agregar(menu, "estado", "Mostrar los hospitales", mostrar_hospitales);

	menu_agregar(menu, "D", "Destruir hospital activo", destruir_hospital);
	menu_agregar(menu, "destruir", "Destruir hospital activo", destruir_hospital);

	menu_agregar(menu, "M", "Mostrar los pokemones de un hospital",
		     mostrar_pokemones);
	menu_agregar(menu, "mostrar", "Mostrar los pokemones de un hospital",
		     mostrar_pokemones);

	menu_agregar(menu, "L", "Mostrar los pokemones detallados de un hospital",
		     mostrar_pokemones_detallados);
	menu_agregar(menu, "listar", "Mostrar los pokemones detallados de un hospital",
		     mostrar_pokemones_detallados);

	menu_agregar(menu, "H", "Mostrar los comandos", menu_mostrar);
	menu_agregar(menu, "ayuda", "Mostrar los comandos", menu_mostrar);
	menu_agregar(menu, "help", "Mostrar los comandos", menu_mostrar);

	menu_agregar(menu, "S", "Salir del menu", menu_salir);
	menu_agregar(menu, "salir", "Salir del menu", menu_salir);
	menu_agregar(menu, "exit", "Salir del menu", menu_salir);

	menu_mostrar(menu);

	bool salir = false;
	while (!salir)
		salir = menu_interactuar(menu);

	menu_salir(menu);

	return 0;
}