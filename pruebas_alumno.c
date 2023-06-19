#include "pa2m.h"
#include "src/menu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXTO 100

void funcion(void *v1, void *v2)
{
}

void pruebas_de_creacion_y_destruccion_del_menu()
{
	menu_t *menu = menu_crear(NULL);

	pa2m_afirmar(menu != NULL, "Se puede crear un menu con exito");
	pa2m_afirmar(!menu_cantidad(menu), "El menu esta vacio");

	menu_destruir(menu);
}

void pruebas_agregar_y_destruir()
{
	menu_t *menu = menu_crear(NULL);
	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto[MAX_TEXTO] = "Texto de prueba";

	pa2m_afirmar(menu_agregar(menu, &clave1, texto, funcion) != NULL,
		     "Se pueden agregar operaciones al menu");

	pa2m_afirmar(menu_agregar(menu, &clave2, texto, funcion) != NULL,
		     "Se puede agregar una operacion con clave no mayuscula");

	pa2m_afirmar(menu_agregar(menu, clave3, texto, funcion) != NULL,
		     "Se puede agregar una operacion con una palabra como clave");

	pa2m_afirmar(menu_cantidad(menu) == 3,
		     "Se agregaron 3 operaciones y la cantidad de operaciones es 3");

	menu_destruir(menu);
}

void pruebas_buscar_operaciones_por_clave()
{
	menu_t *menu = menu_crear(NULL);
	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto[MAX_TEXTO] = "Texto de prueba";

	menu_agregar(menu, &clave1, texto, funcion);
	menu_agregar(menu, &clave2, texto, funcion);
	menu_agregar(menu, clave3, texto, funcion);
	
	menu_destruir(menu);
}

void pruebas_mostrar_operaciones_del_menu()
{
	menu_t *menu = menu_crear(NULL);
	char clave1 = 'A';
	char clave2 = 'b';
	char clave3[] = "clAvE";
	char texto[MAX_TEXTO] = "Texto de prueba";

	menu_agregar(menu, &clave1, texto, funcion);
	menu_agregar(menu, &clave2, texto, funcion);
	menu_agregar(menu, clave3, texto, funcion);

	menu_mostrar(menu);

	menu_destruir(menu);
}

void pruebas_de_operaciones_del_tda_menu()
{
	pa2m_nuevo_grupo("PRUEBAS DE CREACION Y DESTRUCCION");
	pruebas_de_creacion_y_destruccion_del_menu();

	pa2m_nuevo_grupo("PRUEBAS DE AGREGAR OPERACION Y DESTRUIR MENU");
	pruebas_agregar_y_destruir();

	pa2m_nuevo_grupo("PRUEBAS DE BUSCAR OPERACIONES DEL MENU");
	pruebas_buscar_operaciones_por_clave();

	pa2m_nuevo_grupo("PRUEBAS DE MOSTRAR LAS OPERACIONES DEL MENU");
	pruebas_mostrar_operaciones_del_menu();

}

/*
-------------------------------------------------------------------------------
*/

void pruebas_de_destruir_todo_en_el_menu()
{
}

/*
-------------------------------------------------------------------------------
*/

void pruebas_del_tda_menu_con_parametros_nulos()
{
	menu_t *menu = menu_crear(NULL);
	char clave = 'C';
	char texto[] = "Texto";

	pa2m_afirmar(!menu_agregar(NULL, &clave, texto, funcion),
		     "No se puede agregar una operacion a un menu invalido");

	pa2m_afirmar(!menu_agregar(menu, NULL, texto, funcion),
		     "No se puede agregar una operacion con clave invalida");

	pa2m_afirmar(!menu_agregar(menu, &clave, NULL, funcion),
		     "No se puede agregar una operacion con texto invalido");

	pa2m_afirmar(!menu_agregar(menu, &clave, texto, NULL),
		     "No se puede agregar una operacion invalida a un menu");

	pa2m_afirmar(!menu_obtener(NULL, &clave),
		     "No se puede obtener una operacion de un menu invalido");

	pa2m_afirmar(!menu_obtener(menu, NULL),
		     "No se puede obtener una operacion con clave invalida");
/*
	pa2m_afirmar(!menu_ejecutar(NULL, &clave, NULL),
		     "No se puede ejecutar una operacion de un menu invalido");

	pa2m_afirmar(!menu_ejecutar(menu, NULL, NULL),
		     "No se puede ejecutar una operacion con clave invalida");
*/	//FALTA MENU_MOSTRAR PORQUE ES DE TIPO VOID
	pa2m_afirmar(!menu_cantidad(NULL),
		     "No se puede obtener la cantidad de operaciones de un menu invalido");

	pa2m_afirmar(!menu_obtener_contenido(NULL),
		     "No se puede obtener el contenido de un menu invalido");

	menu_destruir(menu);
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
	printf("Librerar el menu, sus operaciones, y sus hospitales no debe perder memoria");
	pruebas_de_destruir_todo_en_el_menu();
	printf("\n");

	pa2m_nuevo_grupo("--- PRUEBAS DE FUNCIONES CON PARAMETROS NULOS ---");
	pruebas_del_tda_menu_con_parametros_nulos();

	return pa2m_mostrar_reporte();
}
