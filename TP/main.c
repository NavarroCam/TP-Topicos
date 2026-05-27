#include <stdio.h>
#include <locale.h>
#include "archivos.h"
//#include "pilotos.h" //PREGUNTAR A CARO: Esta bien que la referencia venga de otro archivo?
///PILOTOS.DAT ORDENAR y BUSCAR en el archivo
int main()
{
    setlocale(LC_ALL, "");
    int op;

    tda_vector pilotos;
    tda_vector escuderias;

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
    else
        fclose(test);

    crear_Vector(&pilotos,sizeof(t_piloto));
    cargarEnTDA(PILOTOS_DAT,&pilotos,sizeof(t_piloto));

    crear_Vector(&escuderias, sizeof(t_escuderia));
    cargarEnTDA(ESCUDERIAS_DAT, &escuderias, sizeof(t_escuderia));

    printf("=== SISTEMA DE GESTION TEMPORADA F1 ===\n");
    do
    {
        puts("\n\n===============================================");
        puts("\t\t   MENU PRINCIPAL");
        puts("===============================================");
        printf("1. Pilotos\n");
        printf("2. Escuderias\n");
        printf("3. Carreras\n");
        printf("4. Estadisticas\n");
        printf("0. Salir\n");
        printf("\nOpcion: ");
        scanf("%d", &op);
        system("cls");

        switch (op)
        {
            case 1:
                __menuPilotos(&pilotos, &escuderias);
                break;
            case 2:
                menuEscuderias(&escuderias);
                break;
            case 3:
                printf("menu carreras -> HACER");
                //menuCarreras();
                break;
            case 4:
                printf("menu estadisticas -> HACER");
                //menuEstadisticas();
                break;
            case 0:
                printf("Saliendo..\n");
                break;
            default:
                printf("Opcion inválida.\n");
        }
    }while (op != 0);
//guardar datos archivo -> reescribo?
//exportar datos a txt -> pilotos y escuderias nomas? guardar los datos
///MODIFICAR DONDE EL TDA NO SEA ESTRICTAMENTE NECESARIO
    destruir_Vector(&pilotos);
    destruir_Vector(&escuderias);

    return 0;
}
