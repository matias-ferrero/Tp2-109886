#include "pa2m.h"
#include "src/tp1.h"
#include "src/hash.h"
#include "src/lista.h"
#include "src/pokemon.h"
#include "src/pokemon_privado.h"

#include "src_tp2/tp2.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
-------------------------------------------------------------------------------
*/

int main()
{
	pa2m_nuevo_grupo("----------- PRUEBAS DEL TP: TDA-HASH -----------");

	pa2m_nuevo_grupo(
		"--- PRUEBAS DE LAS OPERACIONES DE LA TABLA DE HASH ---");
	pruebas_de_operaciones_del_tda_hash();
	printf("\n");

	pa2m_nuevo_grupo("--- PRUEBAS DEL DESTRUCTOR DE LA TABLA DE HASH ---");
	printf("Liberar la tabla de hash y sus valores no debe perder memoria");
	pruebas_de_destruir_todo_en_el_hash();
	printf("\n");

	pa2m_nuevo_grupo("--- PRUEBAS DE FUNCIONES CON PARAMETROS NULOS ---");
	pruebas_del_tda_hash_con_parametros_nulos();

	return pa2m_mostrar_reporte();
}
