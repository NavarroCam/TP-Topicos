#include <stdio.h>
#include <locale.h>
#include <windows.h>

#include "archivos.h"
#include "carreras.h"
#include "estadisticas.h"

int main()
{
    setlocale(LC_ALL, "");
    int op;

    FILE* test;
    FILE* pPilotos, *pEscuderias, *pCarreras, *pPuntajes, *pEstadisticas;

//    Carga inicial si no existen los .dat
    test = fopen(PILOTOS_DAT,"rb");
    if(!test)
    {
        puts("Cargando datos iniciales...");
        cargarPilotosTxtABin(PILOTOS_TXT,PILOTOS_DAT);//para poder modificarlos
        generarArchivoOrdenado(PILOTOS_DAT,sizeof(t_piloto),compararIdPiloto);
        cargarEscuderiasTxtABin(ESCUDERIAS_TXT,ESCUDERIAS_DAT);//para poder modificarlos
        generarArchivoOrdenado(ESCUDERIAS_DAT,sizeof(t_escuderia),compararIdEscuderia);
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

    printf("=== SISTEMA DE GESTION TEMPORADA F1 ===");
    do
    {
        puts("\n===============================================");
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
            menuEscuderias(pPilotos, pEscuderias);
            break;
        case 3:
            pCarreras = fopen(CARRERAS_DAT, "r+b");

            if(!pCarreras)
            {
                pCarreras = fopen(CARRERAS_DAT, "w+b");

                if(!pCarreras)
                {
                    printf("Error al abrir el archivo\n");
                    fclose(pPilotos);
                    fclose(pEscuderias);
                    return ERROR_ARCH;
                }
            }
            pPuntajes = fopen(PUNTAJES_TXT,"rt");
            if(!pPuntajes)
            {
                fclose(pPilotos);
                fclose(pEscuderias);
                fclose(pCarreras);
                return ERROR_ARCH;
            }

            pEstadisticas = fopen(ESTADISTICAS_DAT, "r+b");
            if(!pEstadisticas)
            {
                if(!pEstadisticas)
                {
                    pEstadisticas = fopen(ESTADISTICAS_DAT, "w+b");

                    if(!pEstadisticas)
                    {
                        fclose(pPilotos);
                        fclose(pEscuderias);
                        fclose(pCarreras);
                        fclose(pPuntajes);
                        return ERROR_ARCH;
                    }
                }
            }
            menuCarreras(pCarreras,pPilotos,pPuntajes,pEstadisticas);

            fclose(pCarreras);
            fclose(pPuntajes);
            fclose(pEstadisticas);
            break;
        case 4:
            pEstadisticas = fopen(ESTADISTICAS_DAT,"rb");
            if(!pEstadisticas)
            {
                puts("No fue posible abrir el archivo de estadisticas :(\n Puede agregar una carrera y volver a intentarlo\n");
                break;
            }
            menuEstadisticas(pEstadisticas);
            fclose(pEstadisticas);
            break;
        case 0:
            printf("Saliendo..\n");
            Sleep(1000);
            break;
        default:
            printf("Opcion inválida.\n");
        }
    }
    while (op != 0);

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
