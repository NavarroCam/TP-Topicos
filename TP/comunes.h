#ifndef COMUNES_H_INCLUDED
#define COMUNES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tipos.h"

#define MINTAM 5
#define ERROR_MEMORIA -1
#define TODOOK    0
#define ERROR_ -2
#define ERROR_ARCH -3
#define ENCONTRADO 1
#define NO_ENCONTRADO 0

// Macro para mostrar por pantalla la fecha
#define FECHA_FORMATO(f, a, m, d) \
    unsigned a = (f) / 10000; \
    unsigned m = ((f) % 10000) / 100; \
    unsigned d = (f) % 100;

typedef struct{
    void* vec;
    size_t ce;
    size_t tam;
    size_t maxElem;
}tda_vector;

int crear_Vector(tda_vector* v, size_t tam);
int insertarAlFinal_Vector(tda_vector* v, const void* dato);
void destruir_Vector(tda_vector* v);

void* map_(void* vec, size_t ce, size_t tam, void accion(void*));
void sSort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
void* buscarMenor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
void intercambiar(void* a, void* b, size_t tam);
void* bSearch(const void* clave, const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
int cargarEnTDA(const char* archNom, tda_vector* v, size_t tamElem);
int ValidarFecha(char* fecha);
void Separar_a_tfecha(unsigned long long dato, t_fecha *fecha);

int confirmarModificacion(const char *mensaje);
#endif // COMUNES_H_INCLUDED
