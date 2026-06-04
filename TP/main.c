#include <stdio.h>
#include <locale.h>
#include <windows.h>

#include "archivos.h"
#include "carreras.h"

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
        return ERROR_ARCH;
    }
    pEscuderias = fopen(ESCUDERIAS_DAT, "r+b");
    if(!pEscuderias)
    {
        fclose(pPilotos);
        return ERROR_ARCH;
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
        while (getchar() != '\n');
        system("cls");

        switch (op)
        {
            case 1:
                __menuPilotos(pPilotos, pEscuderias);
                break;
            case 2:
                ///REVISAR TODO LO DE ESCUDERIAS
                menuEscuderias(pPilotos, pEscuderias);
                break;
            case 3:
                pCarreras = fopen(CARRERAS_DAT,"a+b");
                if(!pCarreras)
                {
                    fclose(pPilotos);
                    fclose(pEscuderias);
                    return ERROR_ARCH;
                }
                pPuntajes = fopen(PUNTAJES_TXT,"rt");
                if(!pPuntajes)
                {
                    fclose(pPilotos);
                    fclose(pEscuderias);
                    fclose(pCarreras);
                    return ERROR_ARCH;
                }
                menuCarreras(pCarreras,pPilotos,pPuntajes);

                fclose(pCarreras);
                fclose(pPuntajes);
                break;
            case 4:
                printf("menu estadisticas -> HACER");
                //menuEstadisticas();
                break;
            case 0:
                printf("Saliendo..\n");
                Sleep(1000);
                break;
            default:
                printf("Opcion inválida.\n");
        }
    }while (op != 0);

    fclose(pPilotos);
    fclose(pEscuderias);

    exportarPilotosTxt(PILOTOS_DAT,PILOTOS_ACT_TXT);
    exportarEscuderiasTxt(ESCUDERIAS_DAT,ESCUDERIAS_ACT_TXT);
    remove(PILOTOS_TXT);
    rename(PILOTOS_ACT_TXT,PILOTOS_TXT);
    remove(ESCUDERIAS_TXT);
    rename(ESCUDERIAS_ACT_TXT,ESCUDERIAS_TXT);
    system("cls");
    puts("Archivos exportados exitosamente.");
    return 0;
}
