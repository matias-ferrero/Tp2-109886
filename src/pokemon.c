#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

#define CANT_DATOS 4
#define FORMATO_LECTURA "%zu,%[^,],%zu,%[^\n]\n"
//<ID>,<NOMBRE>,<SALUD>,<NOMBRE ENTRENADOR>

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if (!string)
		return NULL;

	pokemon_t *pokemon;
	pokemon = malloc(sizeof(pokemon_t));
	if (!pokemon)
		return NULL;

	int leidos = sscanf(string, FORMATO_LECTURA, &(pokemon->id),
			    pokemon->nombre, &(pokemon->salud),
			    pokemon->nombre_entrenador);

	if (leidos != CANT_DATOS) {
		pokemon_destruir(pokemon);
		return NULL;
	}

	return pokemon;
}

pokemon_t *pokemon_copiar(pokemon_t *pokemon)
{
	if (!pokemon)
		return NULL;

	pokemon_t *copia_pokemon = NULL;
	copia_pokemon = malloc(sizeof(pokemon_t));
	if (!copia_pokemon)
		return NULL;

	copia_pokemon->id = pokemon->id;
	copia_pokemon->salud = pokemon->salud;
	strcpy(copia_pokemon->nombre, pokemon->nombre);
	strcpy(copia_pokemon->nombre_entrenador, pokemon->nombre_entrenador);

	return copia_pokemon;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (!pokemon1 || !pokemon2)
		return false;

	int comparar_entrenador = strcmp(pokemon1->nombre_entrenador,
					 pokemon2->nombre_entrenador);
	int comparar_nombre = strcmp(pokemon1->nombre, pokemon2->nombre);

	if ((pokemon1->id == pokemon2->id) &&
	    (pokemon1->salud == pokemon2->salud) &&
	    (comparar_entrenador == 0) && (comparar_nombre == 0))
		return true;

	else
		return false;
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (!pokemon)
		return NULL;

	return pokemon->nombre;
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (!pokemon)
		return NULL;

	return pokemon->nombre_entrenador;
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (!pokemon)
		return 0;

	return pokemon->salud;
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (!pokemon)
		return 0;

	return pokemon->id;
}

void pokemon_destruir(pokemon_t *pokemon)
{
	if (!pokemon)
		return;

	free(pokemon);
}
