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
