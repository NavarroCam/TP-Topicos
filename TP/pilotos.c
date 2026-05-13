#include "pilotos.h"

void imprimirPilotos(const char *binPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbin;
    int contador;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return;
    }

    printf("\n===== PILOTOS =====\n");
    contador = 0;
    while (fread(piloto, sizeof(t_piloto), 1, fbin) == 1)
    {
        printf("ID: %u | Nombre: %s | Nac: %s | Escuderia: %u | Puntos: %u | Estado: %c | FechaNac: %llu\n",
               piloto->id,
               piloto->nombre,
               piloto->nacionalidad,
               piloto->id_escuderia,
               piloto->puntos_acumulados,
               piloto->estado,
               piloto->fechaNacimiento);
        contador++;
    }
    printf("Total pilotos leidos: %d\n", contador);

    fclose(fbin);
}
