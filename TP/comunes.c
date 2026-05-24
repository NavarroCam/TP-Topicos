#include "comunes.h"

int crear_Vector(tda_vector* v, size_t tam)
{
    v->tam = tam;
    v->vec = malloc(v->tam * MINTAM);
    if(!v->vec)
    {
        return ERROR_MEMORIA;
    }

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
        {
            return ERROR_MEMORIA;
        }

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

void sSort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*))
{
    int i;
    char* base = (char*)vec;
    void* pmenor;

    for(i=0;i<ce-1;i++)
    {
        pmenor = buscarMenor(base,ce-i,tam,cmp);
        if(pmenor != base)
        {
            intercambiar(base,pmenor,tam);
        }
        base += tam;
    }
}

void* buscarMenor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*))
{
    char* menor = (char*)vec;
    char* base = (char*)vec;
    int i;
    base += tam;

    for(i=1;i<ce;i++)
    {
        if(cmp(base,menor) < 0)
        {
            menor = base;
        }
        base += tam;
    }
    return menor;
}

void intercambiar(void* a, void* b, size_t tam)
{
    int i;
    char temp;
    for(i=0;i<tam;i++)
    {
        temp = *(char*)a;
        *(char*)a = *(char*)b;
        *(char*)b = temp;
        a++;
        b++;
    }
}

