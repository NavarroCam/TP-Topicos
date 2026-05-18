#include "comunes.h"

int crear_Vector(tda_vector* v, size_t tam)
{
    v->tam = tam;
    v->vec = malloc(v->tam * MINTAM);
    if(!v->vec)
        return ERROR_MEMORIA;
    v->maxElem = MINTAM;
    v->ce = 0;
    return OK;
}
int insertarAlFinal_Vector(tda_vector* v, const void* dato)
{
    void* nvec;
    if(v->ce == v->maxElem)
    {
        nvec = realloc(v->vec, v->tam * (v->maxElem + MINTAM));
        if(!nvec)
            return ERROR_MEMORIA;
        v->vec = nvec;
        v->maxElem = v->maxElem + MINTAM;
    }
    memcpy(v->vec + (v->ce * v->tam), dato, v->tam);
    (v->ce)++;
    return OK;
}
void destruir_Vector(tda_vector* v)
{
    free(v->vec);
}
void* map_(void* vec, size_t ce, size_t tam, void accion(void*))
{
    void* ini=vec;
    int i;
    for(i=0;i<ce;i++)
    {
        accion(vec);
        vec+=tam;
    }
    return ini;
}
