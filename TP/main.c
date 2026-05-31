#include <stdio.h>
#include <locale.h>

#include "archivos.h"
#include "carreras.h"
//#include "pilotos.h" //PREGUNTAR A CARO: Esta bien que la referencia venga de otro archivo?
///PILOTOS.DAT ORDENAR y BUSCAR en el archivo
int main()
{
    setlocale(LC_ALL, "");
    int op;

    FILE* test;
    FILE* pPilotos, *pEscuderias, *pCarreras, *pPuntajes;

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

    pPilotos = fopen(PILOTOS_DAT,"r+b");
    if(!pPilotos)
    {
        return ERROR;
    }
    pEscuderias = fopen(ESCUDERIAS_DAT, "r+b");
    if(!pEscuderias)
    {
        fclose(pPilotos);
        return ERROR;
    }

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
                __menuPilotos(pPilotos, pEscuderias);
                break;
            case 2:
                ///REVISAR TODO LO DE ESCUDERIAS
//                menuEscuderias(ESCUDERIAS_DAT);
                break;
            case 3:
                pCarreras = fopen(CARRERAS_DAT,"a+b");
                if(!pCarreras)
                {
                    fclose(pPilotos);
                    fclose(pEscuderias);
                    return ERROR;
                }
                pPuntajes = fopen(PUNTAJES_TXT,"rt");
                if(!pPuntajes)
                {
                    fclose(pPilotos);
                    fclose(pEscuderias);
                    fclose(pCarreras);
                    return ERROR;
                }
                menuCarreras(pCarreras,pPilotos,pPuntajes);
                //lo pongo aca porque no se si seria correcto o como hacer para cerrarlo al final
                fclose(pCarreras);
                fclose(pPuntajes);
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
    fclose(pPilotos);
    fclose(pEscuderias);
    return 0;
}
