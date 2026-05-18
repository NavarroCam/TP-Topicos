#ifndef COMUNES_H_INCLUDED
#define COMUNES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MINTAM 5
#define ERROR_MEMORIA -1
#define OK    0
#define ERROR -1

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
#endif // COMUNES_H_INCLUDED
