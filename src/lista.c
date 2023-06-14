#include "lista.h"

#include <stddef.h>
#include <stdlib.h>

#define POSICION_CERO 0

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_fin;
	size_t cantidad_nodos;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *nodo;
};

nodo_t *buscar_nodo_por_posicion(nodo_t *nodo, size_t posicion_en_lista,
				 size_t posicion_buscada)
{
	if (posicion_buscada != posicion_en_lista)
		nodo = buscar_nodo_por_posicion(nodo->siguiente,
						posicion_en_lista + 1,
						posicion_buscada);

	return nodo;
}

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return NULL;

	nodo_t *nodo = calloc(1, sizeof(nodo_t));
	if (!nodo)
		return NULL;

	if (lista_vacia(lista))
		lista->nodo_inicio = nodo;
	else
		lista->nodo_fin->siguiente = nodo;

	lista->nodo_fin = nodo;
	lista->cantidad_nodos++;

	nodo->elemento = elemento;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista)
		return NULL;

	if (lista_vacia(lista) || posicion >= lista_tamanio(lista))
		return lista_insertar(lista, elemento);

	nodo_t *nodo = calloc(1, sizeof(nodo_t));
	if (!nodo)
		return NULL;

	if (!posicion) {
		nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo;
	} else {
		nodo_t *nodo_anterior_a_insertar = buscar_nodo_por_posicion(
			lista->nodo_inicio, POSICION_CERO, posicion - 1);
		nodo->siguiente = nodo_anterior_a_insertar->siguiente;
		nodo_anterior_a_insertar->siguiente = nodo;
	}

	nodo->elemento = elemento;
	lista->cantidad_nodos++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	nodo_t *nodo = lista->nodo_fin;
	void *elemento = lista->nodo_fin->elemento;

	if (lista_tamanio(lista) == 1) {
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
	} else {
		nodo_t *nuevo_ultimo_nodo = buscar_nodo_por_posicion(
			lista->nodo_inicio, POSICION_CERO,
			lista->cantidad_nodos - 2);
		lista->nodo_fin = nuevo_ultimo_nodo;
		nuevo_ultimo_nodo->siguiente = NULL;
	}

	lista->cantidad_nodos--;
	free(nodo);

	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	nodo_t *nodo = NULL;

	if (lista_tamanio(lista) == 1 || posicion >= (lista_tamanio(lista) - 1))
		return lista_quitar(lista);

	if (!posicion) {
		nodo = lista->nodo_inicio;
		lista->nodo_inicio = nodo->siguiente;
	} else {
		nodo_t *nodo_anterior_a_quitar = buscar_nodo_por_posicion(
			lista->nodo_inicio, POSICION_CERO, posicion - 1);
		nodo = nodo_anterior_a_quitar->siguiente;
		nodo_anterior_a_quitar->siguiente = nodo->siguiente;
	}

	lista->cantidad_nodos--;
	void *elemento = nodo->elemento;
	free(nodo);

	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion >= lista->cantidad_nodos)
		return NULL;

	nodo_t *nodo = buscar_nodo_por_posicion(lista->nodo_inicio,
						POSICION_CERO, posicion);

	return nodo->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador || lista_vacia(lista))
		return NULL;

	nodo_t *nodo = lista->nodo_inicio;

	for (size_t i = POSICION_CERO; i < lista_tamanio(lista); i++) {
		if (!comparador(nodo->elemento, contexto))
			return nodo->elemento;

		nodo = nodo->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || !lista_tamanio(lista))
		return true;

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;

	return lista->cantidad_nodos;
}

void lista_destruir(lista_t *lista)
{
	lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista)
		return;

	while (!lista_vacia(lista)) {
		nodo_t *nodo_auxiliar = lista->nodo_inicio->siguiente;
		if (funcion != NULL)
			funcion(lista->nodo_inicio->elemento);

		free(lista->nodo_inicio);
		lista->nodo_inicio = nodo_auxiliar;
		lista->cantidad_nodos--;
	}

	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;

	iterador->lista = lista;
	iterador->nodo = lista->nodo_inicio;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo)
		return false;

	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista))
		return false;

	if (lista_iterador_tiene_siguiente(iterador)) {
		iterador->nodo = iterador->nodo->siguiente;
		if (iterador->nodo != NULL)
			return true;
	}

	return false;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo)
		return NULL;

	return iterador->nodo->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion || lista_vacia(lista))
		return 0;

	bool validar_funcion = true;
	size_t iterados = 0;
	nodo_t *nodo = lista->nodo_inicio;

	while (validar_funcion && iterados < lista->cantidad_nodos) {
		validar_funcion = funcion(nodo->elemento, contexto);
		nodo = nodo->siguiente;
		iterados++;
	}

	return iterados;
}