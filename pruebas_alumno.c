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
	menu_t *menu = menu_crear();

	pa2m_afirmar(menu != NULL, "Se puede crear un menu con exito");
	pa2m_afirmar(!menu_cantidad_opciones(menu), "El menu esta vacio");

	menu_destruir(menu);
}

void pruebas_agregar_y_destruir()
{
	menu_t *menu = menu_crear();
	char clave = 'C';
	char texto[MAX_TEXTO] = "Texto de prueba";
	pa2m_afirmar(menu_agregar(menu, &clave, texto, funcion) != NULL,
		     "Se pueden agregar operaciones al menu");

	pa2m_afirmar(menu_cantidad_opciones(menu) == 1,
		     "Se agrega 1 operacion y hay 1 operacion en el menu");

	menu_destruir(menu);
}

void pruebas_de_operaciones_del_tda_menu()
{
	pa2m_nuevo_grupo("PRUEBAS DE CREACION Y DESTRUCCION");
	pruebas_de_creacion_y_destruccion_del_menu();

	pa2m_nuevo_grupo("PRUEBAS DE AGREGAR OPERACION Y DESTRUIR MENU");
	pruebas_agregar_y_destruir();

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
