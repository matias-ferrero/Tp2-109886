#include "pa2m.h"
#include "tp2.h"
#include "src/menu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXTO 100

int cantidad_op(void *menu)
{
	if (!menu)
		return -1;

	return (int)menu_cantidad((menu_t *)menu);
}

bool mostrar_operacion(const char *clave, void *op, void *aux)
{
	aux = aux;
	if (!clave || !op)
		return false;

	opcion_t *opcion = op;
	printf("%c: %s\n", clave[0], obtener_informacion(opcion));
	return true;
}

void destruir_lista(void *contenido)
{
	if (!contenido)
		return;

	lista_t *lista = (void *)contenido;
	lista_destruir(lista);
}

void pruebas_de_creacion_y_destruccion_del_menu()
{
	menu_t *menu = menu_crear(NULL);

	pa2m_afirmar(menu != NULL, "Se puede crear un menu con exito");
	pa2m_afirmar(!menu_cantidad(menu), "El menu esta vacio");

	menu_destruir(menu);
}

void pruebas_obtener_y_modificar_contenido_del_menu()
{
	menu_t *menu = menu_crear(NULL);

	pa2m_afirmar(
		!menu_obtener_contenido(menu),
		"Pedir el contenido de un menu sin contenido devuelve NULL");

	pa2m_afirmar(menu_cambiar_contenido(menu, lista_crear(), NULL) != NULL,
		     "Se puede cambiar el contenido actual por una lista");

	pa2m_afirmar(menu_obtener_contenido(menu) != NULL,
		     "Se puede obtener la lista del contenido del menu");

	pa2m_afirmar(menu_cambiar_contenido(menu, lista_crear(),
					    destruir_lista) != NULL,
		     "Se puede cambiar la lista actual por una nueva lista");

	pa2m_afirmar(menu_obtener_contenido(menu) != NULL,
		     "Se puede obtener la nueva lista del contenido del menu");

	menu_destruir_todo(menu, destruir_lista);
}

void pruebas_agregar_y_destruir()
{
	menu_t *menu = menu_crear(NULL);
	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto1[MAX_TEXTO] = "Texto de prueba";
	char texto2[MAX_TEXTO] = "Texto de prueba 2";

	pa2m_afirmar(menu_agregar(menu, &clave1, texto1, cantidad_op) != NULL,
		     "Se pueden agregar operaciones al menu");

	pa2m_afirmar(menu_agregar(menu, &clave2, texto1, cantidad_op) != NULL,
		     "Se puede agregar una operacion con clave no mayuscula");

	pa2m_afirmar(
		menu_agregar(menu, clave3, texto1, cantidad_op) != NULL,
		"Se puede agregar la misma operacion con una clave sinonimo");

	pa2m_afirmar(
		menu_cantidad(menu) == 3,
		"Se agregaron 3 operaciones y la cantidad de operaciones es 3");

	pa2m_afirmar(menu_agregar(menu, &clave2, texto2, cantidad_op) != NULL,
		     "Se puede modificar una operacion");

	pa2m_afirmar(
		menu_cantidad(menu) == 3,
		"Se modifico una operacion y la cantidad de operaciones sigue siendo 3");

	menu_destruir(menu);
}

void pruebas_buscar_operaciones_por_clave()
{
	menu_t *menu = menu_crear(NULL);
	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto1[MAX_TEXTO] = "Texto de prueba";
	char texto2[MAX_TEXTO] = "Texto de prueba 2";

	pa2m_afirmar(!menu_obtener(menu, &clave1),
		     "No se puede obtener una operacion de un menu vacio");

	menu_agregar(menu, &clave1, texto1, cantidad_op);

	pa2m_afirmar(!menu_obtener(menu, &clave2),
		     "No se puede obtener una operacion que no se agrego");

	menu_agregar(menu, &clave2, texto1, cantidad_op);
	menu_agregar(menu, clave3, texto1, cantidad_op);

	pa2m_afirmar(menu_obtener(menu, &clave1) != NULL,
		     "Se puede obtener una operacion agregada");

	pa2m_afirmar(menu_obtener(menu, &clave2) != NULL,
		     "Se puede obtener una operacion con clave no mayuscula");

	char *texto = obtener_informacion(menu_obtener(menu, &clave2));
	pa2m_afirmar(!strcmp(texto, texto1),
		     "Se puede obtener la informacion de una operacion");

	pa2m_afirmar(
		menu_obtener(menu, clave3) != NULL,
		"Se puede obtener la misma operacion con una clave sinonimo");

	menu_agregar(menu, &clave2, texto2, cantidad_op);
	pa2m_afirmar(menu_obtener(menu, &clave2) != NULL,
		     "Se puede volver a obtener la operacion modificada");

	texto = obtener_informacion(menu_obtener(menu, &clave2));
	pa2m_afirmar(
		!strcmp(texto, texto2),
		"Se puede obtener la informacion modficada de una operacion");

	menu_destruir(menu);
}

void pruebas_ejecutar_operaciones()
{
	menu_t *menu = menu_crear(NULL);
	opcion_t *opcion;
	char clave1 = 'A';
	char clave2 = 'b';
	char texto1[MAX_TEXTO] = "Texto de prueba";
	char texto2[MAX_TEXTO] = "Texto de prueba 2";

	menu_agregar(menu, &clave1, texto1, cantidad_op);
	menu_agregar(menu, &clave2, texto1, cantidad_op);

	opcion = menu_obtener(menu, &clave1);
	pa2m_afirmar(menu_ejecutar(opcion, menu) == menu_cantidad(menu),
		     "Se puede ejecutar la operacion guardada");

	opcion = menu_obtener(menu, &clave2);
	pa2m_afirmar(
		menu_ejecutar(opcion, menu) == menu_cantidad(menu),
		"Se puede ejecutar la operacion guardada con una clave sinonimo");

	menu_agregar(menu, &clave2, texto2, cantidad_op);
	opcion = menu_obtener(menu, &clave2);
	pa2m_afirmar(menu_ejecutar(opcion, menu) == menu_cantidad(menu),
		     "Se puede ejecutar una operacion modificada");

	menu_destruir(menu);
}

void pruebas_menu_iterador_interno()
{
	menu_t *menu = menu_crear(NULL);
	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto1[MAX_TEXTO] = "Texto de prueba";
	char texto2[MAX_TEXTO] = "Texto de prueba 2";

	pa2m_afirmar(!menu_con_cada_operacion(menu, mostrar_operacion, NULL),
		     "No se pueden iterar operaciones de un menu vacio");

	menu_agregar(menu, &clave1, texto1, cantidad_op);
	menu_agregar(menu, &clave2, texto1, cantidad_op);
	menu_agregar(menu, clave3, texto1, cantidad_op);

	pa2m_afirmar(menu_con_cada_operacion(menu, mostrar_operacion, NULL) ==
			     menu_cantidad(menu),
		     "Se pueden iterar las operaciones del menu");

	menu_agregar(menu, &clave2, texto2, cantidad_op);

	pa2m_afirmar(menu_con_cada_operacion(menu, mostrar_operacion, NULL) ==
			     menu_cantidad(menu),
		     "Se pueden iterar las operaciones modificadas del menu");

	menu_destruir(menu);
}

void pruebas_de_operaciones_del_tda_menu()
{
	pa2m_nuevo_grupo("PRUEBAS DE CREACION Y DESTRUCCION");
	pruebas_de_creacion_y_destruccion_del_menu();

	pa2m_nuevo_grupo(
		"PRUEBAS DE OBTENER Y MODIFICAR EL CONTENIDO DEL MENU");
	pruebas_obtener_y_modificar_contenido_del_menu();

	pa2m_nuevo_grupo("PRUEBAS DE AGREGAR OPERACION Y DESTRUIR MENU");
	pruebas_agregar_y_destruir();

	pa2m_nuevo_grupo("PRUEBAS DE BUSCAR OPERACIONES DEL MENU");
	pruebas_buscar_operaciones_por_clave();

	pa2m_nuevo_grupo("PRUEBAS DE EJECUTAR LA OPERACION RECIBIDA");
	pruebas_ejecutar_operaciones();

	pa2m_nuevo_grupo("PRUEBAS DEL ITERADOR INTERNO");
	//pruebas_menu_iterador_interno();
}

/*
-------------------------------------------------------------------------------
*/

void destruir_todo(void *lista)
{
	lista_destruir_todo((lista_t *)lista, free);
}

void pruebas_de_destruir_todo_en_el_menu()
{
	menu_t *menu = menu_crear(lista_crear());
	lista_insertar(menu_obtener_contenido(menu), malloc(sizeof(char)));
	lista_insertar(menu_obtener_contenido(menu), malloc(sizeof(int)));
	lista_insertar(menu_obtener_contenido(menu), malloc(sizeof(float)));
	lista_insertar(menu_obtener_contenido(menu), malloc(sizeof(size_t)));

	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto1[MAX_TEXTO] = "Texto de prueba";

	menu_agregar(menu, &clave1, texto1, cantidad_op);
	menu_agregar(menu, &clave2, texto1, cantidad_op);
	menu_agregar(menu, clave3, texto1, cantidad_op);

	menu_destruir_todo(menu, destruir_todo);
}

/*
-------------------------------------------------------------------------------
*/

void pruebas_del_tda_menu_con_parametros_nulos()
{
	menu_t *menu = menu_crear(lista_crear());
	lista_t *nueva_lista = lista_crear();
	char clave = 'C';
	char texto[] = "Texto";

	pa2m_afirmar(!menu_agregar(NULL, &clave, texto, cantidad_op),
		     "No se puede agregar una operacion a un menu invalido");

	pa2m_afirmar(!menu_agregar(menu, NULL, texto, cantidad_op),
		     "No se puede agregar una operacion con clave invalida");

	pa2m_afirmar(!menu_agregar(menu, &clave, NULL, cantidad_op),
		     "No se puede agregar una operacion con texto invalido");

	pa2m_afirmar(!menu_agregar(menu, &clave, texto, NULL),
		     "No se puede agregar una operacion invalida a un menu");

	pa2m_afirmar(!menu_obtener(NULL, &clave),
		     "No se puede obtener una operacion de un menu invalido");

	pa2m_afirmar(!menu_obtener(menu, NULL),
		     "No se puede obtener una operacion con clave invalida");

	pa2m_afirmar(menu_ejecutar(NULL, NULL) == -1,
		     "No se puede ejecutar una operacion invalida");

	pa2m_afirmar(
		!menu_cantidad(NULL),
		"No se puede obtener la cantidad de operaciones de un menu invalido");

	pa2m_afirmar(!menu_obtener_contenido(NULL),
		     "No se puede obtener el contenido de un menu invalido");

	pa2m_afirmar(!menu_cambiar_contenido(NULL, nueva_lista, NULL),
		     "No se puede cambiar el contenido de un menu invalido");

	pa2m_afirmar(
		!menu_cambiar_contenido(menu, NULL, NULL),
		"No se puede cambiar el contenido del menu por un contenido invalido");

	pa2m_afirmar(!menu_con_cada_operacion(NULL, mostrar_operacion, NULL),
		     "No se pueden iterar operaciones de un menu invalido");

	pa2m_afirmar(
		!menu_con_cada_operacion(menu, NULL, NULL),
		"No se pueden iterar operaciones de un menu con una funcion invalida");

	lista_destruir(nueva_lista);
	menu_destruir_todo(menu, destruir_lista);
}

/*
-------------------------------------------------------------------------------
*/

int main()
{
	pa2m_nuevo_grupo("----------- PRUEBAS DEL TP: TDA-MENU -----------");

	pa2m_nuevo_grupo("--- PRUEBAS DE LAS OPERACIONES DEL MENU ---");
	pruebas_de_operaciones_del_tda_menu();
	printf("\n");

	pa2m_nuevo_grupo("--- PRUEBAS DEL DESTRUCTOR ---");
	printf("Librerar el menu, sus operaciones, y su contenido no debe perder memoria");
	pruebas_de_destruir_todo_en_el_menu();
	printf("\n");

	pa2m_nuevo_grupo("--- PRUEBAS DE FUNCIONES CON PARAMETROS NULOS ---");
	pruebas_del_tda_menu_con_parametros_nulos();

	return pa2m_mostrar_reporte();
}