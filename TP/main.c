#include <stdio.h>
#include "archivos.h"
#include "pilotos.h" //Esta bien que la referencia venga de otro archivo?
int main()
{
    int op;
    FILE* test;
//    Carga inicial si no existen los .dat
    test = fopen(PILOTOS_DAT,"rb");
    if(!test)
    {
        puts("Cargando datos iniciales...");
        cargarPilotosTxtABin(PILOTOS_TXT,PILOTOS_DAT);//para poder modificarlos
        cargarEscuderiasTxtABin(ESCUDERIAS_TXT,ESCUDERIAS_DAT);//para poder modificarlos
//        reconstruirIndice(PILOTOS_DAT,PILOTOS_IDX);
    }

    printf("=== SISTEMA DE GESTION TEMPORADA F1 ===\n");
    do
    {
        printf("\n======= MENU PRINCIPAL =======\n");
        printf("1. Pilotos\n");
        printf("2. Escuderias\n");
        printf("3. Carreras\n");
        printf("4. Estadisticas\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            __menuPilotos();
            break;
        case 2:
            printf("menu escuderias");
            //menuEscuderias();
            break;
        case 3:
            printf("menu carreras");
            //menuCarreras();
            break;
        case 4:
            printf("menu estadisticas");
            //menuEstadisticas();
            break;
        case 0:
            printf("Saliendo..\n");
            break;
        default:
            printf("Opcion inválida.\n");
        }
    }
    while (op != 0);
    return 0;
}
