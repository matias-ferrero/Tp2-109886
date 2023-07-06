#include "tp1.h"

#include "pokemon.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 50

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

void intercambiar_pokemones(pokemon_t **pokemon, size_t n)
{
	if (!pokemon)
		return;

	if (pokemon_salud(pokemon[n]) > pokemon_salud(pokemon[n + 1])) {
		pokemon_t *pokemon_auxiliar = pokemon[n];
		pokemon[n] = (pokemon[n + 1]);
		pokemon[n + 1] = pokemon_auxiliar;
	}
}

void ordenar_pokemones(hospital_t *hospital)
{
	if (!hospital)
		return;

	for (size_t i = 1; i < hospital->cantidad_pokemon; i++) {
		for (size_t j = 0; j < (hospital->cantidad_pokemon - i); j++) {
			intercambiar_pokemones(hospital->pokemones, j);
		}
	}
}

hospital_t *hospital_crear_desde_archivo(const char *archivo_hospital)
{
	if (!archivo_hospital)
		return NULL;

	FILE *archivo = fopen(archivo_hospital, "r");
	if (!archivo)
		return NULL;

	hospital_t *hospital;
	hospital = malloc(sizeof(hospital_t));
	if (!hospital) {
		fclose(archivo);
		return NULL;
	}

	hospital->cantidad_pokemon = 0;
	hospital->cantidad_entrenadores = 0;
	hospital->pokemones = NULL;

	char un_pokemon[MAX_LINEA];
	char *leido = fgets(un_pokemon, MAX_LINEA, archivo);

	pokemon_t *pokemon;
	pokemon_t **hospital_auxiliar;
	while (leido) {
		hospital_auxiliar = realloc(hospital->pokemones,
					    (hospital->cantidad_pokemon + 1) *
						    sizeof(pokemon_t *));
		if (!hospital_auxiliar) {
			hospital_destruir(hospital);
			fclose(archivo);
			return NULL;
		}
		hospital->pokemones = hospital_auxiliar;

		pokemon = pokemon_crear_desde_string(un_pokemon);
		if (!pokemon) {
			hospital_destruir(hospital);
			fclose(archivo);
			return NULL;
		}
		hospital->pokemones[hospital->cantidad_pokemon] = pokemon;
		hospital->cantidad_pokemon++;

		leido = fgets(un_pokemon, MAX_LINEA, archivo);
	}

	if (!hospital_cantidad_pokemones(hospital)) {
		hospital_destruir(hospital);
		fclose(archivo);
		return NULL;
	}

	ordenar_pokemones(hospital);

	fclose(archivo);

	return hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (!hospital)
		return 0;

	return hospital->cantidad_pokemon;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if (!hospital || !funcion)
		return 0;

	size_t posicion_vec_prioridad = 0;
	while (posicion_vec_prioridad < hospital->cantidad_pokemon) {
		if (!funcion(hospital->pokemones[posicion_vec_prioridad], aux))
			return posicion_vec_prioridad + 1;

		posicion_vec_prioridad++;
	}
	return posicion_vec_prioridad;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if (!hospital || !pokemones_ambulancia)
		return ERROR;
	size_t pos_ambulancia = 0;
	size_t cant_auxiliar = hospital->cantidad_pokemon;
	hospital->cantidad_pokemon += cant_pokes_ambulancia;

	pokemon_t **hospital_auxiliar;
	hospital_auxiliar =
		realloc(hospital->pokemones,
			(hospital->cantidad_pokemon) * sizeof(pokemon_t *));
	if (!hospital_auxiliar)
		return ERROR;

	hospital->pokemones = hospital_auxiliar;

	for (size_t pos_hospital = cant_auxiliar;
	     pos_hospital < hospital->cantidad_pokemon; pos_hospital++) {
		hospital->pokemones[pos_hospital] =
			pokemones_ambulancia[pos_ambulancia];
		pos_ambulancia++;
	}
	ordenar_pokemones(hospital);

	return EXITO;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (!hospital || prioridad > hospital->cantidad_pokemon)
		return NULL;

	return hospital->pokemones[prioridad];
}

void hospital_destruir(hospital_t *hospital)
{
	if (!hospital)
		return;

	for (size_t i = 0; i < hospital->cantidad_pokemon; i++)
		pokemon_destruir(hospital->pokemones[i]);

	if (hospital->pokemones)
		free(hospital->pokemones);

	free(hospital);
}
