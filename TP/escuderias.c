#include "escuderias.h"

void listarEscuderias(const char *binPath)
{
    t_escuderia e1;
    t_escuderia *escuderia = &e1;
    FILE *fbin;
    int contador;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return;
    }

    printf("\n===== ESCUDERIAS =====\n");
    contador = 0;
    while (fread(escuderia, sizeof(t_escuderia), 1, fbin) == 1)
    {
        printf("ID: %u | Codigo: %s | Nombre: %s | Pais: %s | Estado: %d\n",
               escuderia->id,
               escuderia->codigo,
               escuderia->nombre,
               escuderia->pais,
               escuderia->estado);
        contador++;
    }
    printf("Total escuderias leidas: %d\n", contador);

    fclose(fbin);
}

int compararIdEscuderia(const void* a, const void* b)
{
    unsigned idBuscado = *(unsigned*)a;
    unsigned idEsc = ((t_escuderia*)b)->id;
    if(idBuscado < idEsc) return -1;
    if(idBuscado > idEsc) return  1;
    return 0;
}

int escuderiaValida(unsigned idEsc, tda_vector* v)
{
    t_escuderia* resultado;

    resultado = (t_escuderia*)bsearch(&idEsc, v->vec, v->ce, sizeof(t_escuderia), compararIdEscuderia);

    if(!resultado)
    {
        printf("Escuderia no encontrada.\n");
        return ERROR;
    }

    if(resultado->estado != 1)
    {
        printf("La escuderia %s esta inactiva.\n", resultado->nombre);
        return ERROR;
    }
    return OK;
}
