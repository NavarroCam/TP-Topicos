#include <stdio.h>
#include "archivos.h"

int main()
{
    int resultado;

    // Cargo pilotos
    resultado = cargarPilotosTxtABin("pilotos.txt", "pilotos.dat", "pilotos_error.txt");
    if (resultado == OK)
    {
        printf("Pilotos cargados correctamente.\n");
    }
    else
    {
        printf("Error al cargar pilotos.\n");
    }

    // Cargo escuderias
    resultado = cargarEscuderiasTxtABin("escuderias.txt", "escuderias.dat", "escuderias_error.txt");
    if (resultado == OK)
    {
        printf("Escuderias cargadas correctamente.\n");
    }
    else
    {
        printf("Error al cargar escuderias.\n");
    }

    // Verifico si lee los .dat, después borrar
    listarPilotos("pilotos.dat");
    listarEscuderias("escuderias.dat");


    menuPilotos("pilotos.dat", "bajasPilotos.dat");

    return 0;
}
