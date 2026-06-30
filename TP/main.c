#include <stdio.h>
#include <locale.h>
#include <windows.h>

#include "archivos.h"
#include "carreras.h"
#include "estadisticas.h"
//#include "interfaz.h"

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

    do
    {
        limpiarPantalla();
        tituloSistema();
        tituloMenu("MENÚ PRINCIPAL");
        color(COLOR_TEXTO);
        printf("   [1] Gestion de Pilotos\n");
        printf("   [2] Gestion de Escuderias\n");
        printf("   [3] Gestion de Carreras\n");
        printf("   [4] Estadisticas\n");
        printf("   [0] Salir\n");

        color(COLOR_TITULO);
        printf("\n====================================================\n\n");
        restaurarColor();
        printf("Seleccione una opcion > ");
        scanf("%d", &op);
        while (getchar() != '\n');

        switch (op)
        {
        case 1:
            menuPilotos(pPilotos, pEscuderias);
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
            limpiarPantalla();
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
