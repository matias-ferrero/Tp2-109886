#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	size_t capacidad;
	size_t cantidad;
	nodo_t **vector;
};

char *duplicar_clave(const char *clave)
{
	char *aux = malloc(strlen(clave) + 1);
	if (!aux)
		return NULL;

	strcpy(aux, clave);
	return aux;
}

nodo_t *crear_nodo(const char *clave, void *valor)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;

	nodo->clave = duplicar_clave(clave);
	nodo->valor = valor;
	nodo->siguiente = NULL;

	return nodo;
}

void insertar_nodo(hash_t *hash, size_t posicion, nodo_t *nodo)
{
	nodo->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nodo;
	hash->cantidad++;
}

nodo_t *buscar_nodo_por_clave(nodo_t *nodo, const char *clave)
{
	if (!nodo)
		return NULL;

	if (!strcmp(nodo->clave, clave))
		return nodo;

	return buscar_nodo_por_clave(nodo->siguiente, clave);
}

nodo_t *quitar_nodo(hash_t *hash, nodo_t *nodo, const char *clave, void **valor)
{
	if (!nodo)
		return NULL;

	if (!strcmp(nodo->clave, clave)) {
		*valor = nodo->valor;
		nodo_t *aux = nodo->siguiente;
		free(nodo->clave);
		free(nodo);
		hash->cantidad--;
		return aux;
	}

	nodo->siguiente = quitar_nodo(hash, nodo->siguiente, clave, valor);
	return nodo;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash)
		return NULL;

	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash->vector = calloc(capacidad, sizeof(nodo_t *));
	if (!hash->vector) {
		free(hash);
		return NULL;
	}

	hash->capacidad = capacidad;
	hash->cantidad = 0;

	return hash;
}

size_t funcion_hash(const char *clave)
{
	size_t hash = 5381;
	int c;

	while ((c = *clave++))
		hash = ((hash << 5) + hash) + (size_t)c;

	return hash;
}

bool insertar_nodo_en_nuevo_hash(const char *clave, void *valor,
				 void *nuevo_hash)
{
	hash_t *hash = nuevo_hash;
	nodo_t *nodo = crear_nodo(clave, valor);
	if (!nodo)
		return false;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	insertar_nodo(hash, posicion, nodo);
	return true;
}

hash_t *rehash(hash_t *hash)
{
	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);
	if (!nuevo_hash)
		return NULL;

	size_t insertados = hash_con_cada_clave(
		hash, insertar_nodo_en_nuevo_hash, nuevo_hash);
	if (insertados != hash->cantidad) {
		hash_destruir(nuevo_hash);
		return NULL;
	}
	hash_t aux = *hash;
	*hash = *nuevo_hash;
	*nuevo_hash = aux;

	hash_destruir(nuevo_hash);
	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *valor,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	float factor_carga = (float)(hash->cantidad) / (float)(hash->capacidad);
	if (factor_carga > FACTOR_CARGA_MAXIMO)
		hash = rehash(hash);

	if (!hash)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo = hash->vector[posicion];
	while (nodo != NULL) {
		if (!strcmp(nodo->clave, clave)) {
			if (anterior != NULL)
				*anterior = nodo->valor;

			nodo->valor = valor;
			return hash;
		}
		nodo = nodo->siguiente;
	}
	if (anterior != NULL)
		*anterior = NULL;

	nodo = crear_nodo(clave, valor);
	if (!nodo)
		return NULL;

	insertar_nodo(hash, posicion, nodo);

	factor_carga = (float)(hash->cantidad) / (float)(hash->capacidad);
	if (factor_carga > FACTOR_CARGA_MAXIMO)
		hash = rehash(hash);

	if (!hash)
		return NULL;

	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave || !hash_cantidad(hash))
		return NULL;

	void *valor = NULL;
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	hash->vector[posicion] =
		quitar_nodo(hash, hash->vector[posicion], clave, &valor);
	return valor;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave || !hash_cantidad(hash))
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo = buscar_nodo_por_clave(hash->vector[posicion], clave);
	if (!nodo)
		return NULL;

	return nodo->valor;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave || !hash_cantidad(hash))
		return false;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t *nodo = buscar_nodo_por_clave(hash->vector[posicion], clave);
	return nodo != NULL;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;

	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		nodo_t *nodo = hash->vector[i];
		while (nodo) {
			nodo_t *aux = nodo->siguiente;
			if (destructor != NULL)
				destructor(nodo->valor);

			free(nodo->clave);
			free(nodo);
			hash->cantidad--;
			nodo = aux;
		}
	}
	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t recorridos = 0;
	if (!hash || !f || !hash_cantidad(hash))
		return recorridos;

	for (size_t i = 0; i < hash->capacidad; i++) {
		nodo_t *nodo = hash->vector[i];
		while (nodo) {
			bool validar = f(nodo->clave, nodo->valor, aux);
			recorridos++;
			if (!validar)
				return recorridos;

			nodo = nodo->siguiente;
		}
	}
	return recorridos;
}