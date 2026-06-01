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

void* bSearch(const void* clave, const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*))
{
    int pm, res;
    void* ppm;

    while(ce > 0)
    {
        pm = ce/2;
        ppm = (void*)vec + (pm * tam);
        res = cmp(clave, ppm);
        if(res == 0)
            return ppm;
        else if(res > 0)
        {
            ce = ce - pm - 1;
            vec = ppm + tam;
        }
        else
            ce = pm;
    }
    return NULL;
}

int cargarEnTDA(const char* archNom, tda_vector* v, size_t tamElem)
{
    void* elem;
    FILE* pf;

    pf = fopen(archNom, "rb");
    if(!pf)
        return ERROR;

    elem = malloc(tamElem);
    if(!elem)
    {
        fclose(pf);
        return ERROR_MEMORIA;
    }

    fread(elem,tamElem,1,pf);
    while(!feof(pf))
    {
        insertarAlFinal_Vector(v,elem);
        fread(elem,tamElem,1,pf);
    }
    fclose(pf);
    free(elem);
    return OK;
}
int ValidarFecha(char* fecha)
{
    int dia, mes, anio, aux = OK;
    sscanf(fecha,"%4d%2d%2d",&anio,&mes,&dia);
    if(anio < 0)
        aux = ERROR;
    if(mes < 1 || mes > 12)
        aux = ERROR;
    if((mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) && dia > 31)
        aux = ERROR;
    if((mes == 4 || mes == 6 || mes == 9 || mes ==11) && dia >30)
        aux = ERROR;
    if((anio % 4 == 0 && anio % 100 != 0)|| (anio % 400 == 0))//Bisiesto
    {
        if(mes == 2 && dia > 29)
            aux = ERROR;
    }
    else if(mes == 2 && dia > 28)
        aux = ERROR;
    return aux;
}

int confirmarModificacion(const char *mensaje)
{
    char opcion;

    do{
        printf("%s (S/N): ", mensaje);
        scanf(" %c", &opcion);
        opcion = toupper(opcion);
        if(opcion != 'S' && opcion != 'N')
        {
            printf("Opcion invalida.\n");
        }
    }while(opcion != 'S' && opcion != 'N');

    return opcion == 'S';
}
