#include "pilotos.h"
#include <ctype.h>

/*void menuPilotos(const char *binPath, const char *bajasPath)
{
    int opcion;

    do
    {
        printf("\n=============================\n");
        printf("      GESTION PILOTOS\n");
        printf("=============================\n");

        printf("1 - Listar pilotos\n");
        printf("2 - Alta piloto\n");
        printf("3 - Baja piloto\n");
        printf("4 - Modificar piloto\n");
        printf("5 - Listar bajas\n");
        printf("0 - Salir\n");

        printf("\nOpcion: ");
        scanf("%d", &opcion);

        switch(opcion)
        {
            case 1:
                listarAltas(binPath);
                break;

            case 2:
                altaPiloto(binPath);
                break;

            case 3:
                bajaPiloto(binPath, bajasPath);
                break;

            case 4:
                modificarPiloto(binPath);
                break;

            case 5:
                listarBajas(bajasPath);
                break;

            case 0:
                printf("\nFin.\n");
                break;

            default:
                printf("\nOpcion invalida.\n");
        }

    }while(opcion != 0);
}*/

void __menuPilotos()
{
    int op;
//    unsigned int idEsc;
//    unsigned int idPil;

    tda_vector pilotos;
    crear_Vector(&pilotos,sizeof(t_piloto));
    cargarEnTDA(PILOTOS_DAT,&pilotos,sizeof(t_piloto));

    do{
        printf("\n--- PILOTOS ---\n");
        printf("1. Listar pilotos(SOLO FUNCIONA ESTA)\n");
        printf("2. Alta piloto\n");
        printf("3. Baja piloto\n");
        printf("4. Modificar piloto\n");
        printf("5. Buscar piloto por ID\n");
        printf("6. Mostrar ranking\n");
        printf("7. Pilotos por escuderia\n");
        printf("8. Exportar pilotos\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &op);
        system("cls");

        switch (op) {
            case 1: listarPilotos(&pilotos); break;
//            case 2: _altaPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
//            case 3: _bajaPiloto(PILOTOS_DAT, PILOTOS_IDX, BAJAS_PILOTOS); break;
//            case 4: _modificarPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
//            case 5: {
//                FILE *fp;
//                Piloto p;
//                idPil = (unsigned int)leerEntero("ID piloto: ");
//                fp = fopen(PILOTOS_DAT, "rb");
//                if (fp) {
//                    if (buscarPilotoPorId(fp, idPil, &p))
//                        printf("Encontrado: [%u] %s - %s - Pts:%u - Estado:%c\n",
//                               p.id, p.nombre, p.nacionalidad,
//                               p.puntos_acumulados, p.estado);
//                    else
//                        printf("No encontrado.\n");
//                    fclose(fp);
//                }
//                break;
//            }
//            case 6: mostrarRanking(PILOTOS_DAT); break;
//            case 7:
//                idEsc = (unsigned int)leerEntero("ID escuderia: ");
//                listarPilotosPorEscuderia(PILOTOS_DAT, idEsc);
//                break;
//            case 8: exportarPilotos(PILOTOS_DAT, EXP_PILOTOS); break;
            case 0: break;
            default: printf("Opcion invalida.\n");
        }
    }while(op != 0);
    destruir_Vector(&pilotos);
}


int cargarEnTDA(const char* archNom, tda_vector* v, size_t tamElem)
{
    void* elem;
    FILE* pf;
    pf = fopen(archNom, "rb");
    if(!pf)
        return ERROR;
    elem = malloc(tamElem);
    if(!elem)
    {
        fclose(pf);
        return ERROR_MEMORIA;
    }

    fread(elem,tamElem,1,pf);
    while(!feof(pf))
    {
        insertarAlFinal_Vector(v,elem);
        fread(elem,tamElem,1,pf);
    }
    fclose(pf);
    free(elem);
    return OK;
}

int compararPuntos(const void* a, const void* b)
{
    t_piloto* pil1 = (t_piloto*)a;
    t_piloto* pil2 = (t_piloto*)b;
    return pil2->puntos_acumulados - pil1->puntos_acumulados;
}
void mostrarPiloto(void* pilotos)
{
    t_piloto* pil = (t_piloto*)pilotos;
    printf("| %-30s | %-10u |\n",
               pil->nombre,
               pil->puntos_acumulados);
}

void listarPilotos(tda_vector* v)
{
    system("cls");
    puts("===============================================");
    printf("| %-30s | %-10s |\n", "PILOTO", "PUNTOS");
    puts("===============================================");
    map_(v->vec,v->ce,v->tam,mostrarPiloto);
    puts("===============================================");
}
