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

void __menuPilotos(tda_vector* pilotos, tda_vector* escuderias)
{
    int op;
//    unsigned int idEsc;
//    unsigned int idPil;

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

        switch (op)
        {
            case 1: listarPilotos(pilotos); break;
            case 2: altaPiloto(pilotos, escuderias); break;
                //_altaPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
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
            case 6: mostrarRanking(pilotos); break;
//            case 7:
//                idEsc = (unsigned int)leerEntero("ID escuderia: ");
//                listarPilotosPorEscuderia(PILOTOS_DAT, idEsc);
//                break;
//            case 8: exportarPilotos(PILOTOS_DAT, EXP_PILOTOS); break;
            case 0: break;
            default: printf("Opción inválida.\n");
        }
    }while(op != 0);
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

unsigned generarNuevoId(tda_vector* v)
{
    t_piloto* pil = (t_piloto*)v->vec;
    unsigned maxId = 0;
    int i;

    for(i = 0; i < v->ce; i++)
    {
        if(pil->id > maxId)
            maxId = pil->id;
        pil++;
    }
    return maxId + 1;
}

int altaPiloto(tda_vector* v, tda_vector* esc)
{
    t_piloto nuevo;
    char fechaStr[20];

    nuevo.id = generarNuevoId(v);

    puts("==== ALTA DE PILOTO ====");

    printf("Nombre: ");
    fflush(stdin);
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);
    nuevo.nombre[strlen(nuevo.nombre) - 1] = '\0';

    printf("Nacionalidad: ");
    fflush(stdin);
    fgets(nuevo.nacionalidad, sizeof(nuevo.nacionalidad), stdin);
    nuevo.nacionalidad[strlen(nuevo.nacionalidad) - 1] = '\0';

    do{
        printf("ID escuderia: ");
        scanf("%u", &nuevo.id_escuderia);
    }while(escuderiaValida(nuevo.id_escuderia, esc) != OK);

    nuevo.puntos_acumulados = 0;
    nuevo.estado = 'A';

    printf("Fecha nacimiento (AAAAMMDD): ");
    fflush(stdin);
    fgets(fechaStr, sizeof(fechaStr), stdin);
    fechaStr[strlen(fechaStr) - 1] = '\0';
    sscanf(fechaStr, "%I64u", &nuevo.fechaNacimiento);

    insertarAlFinal_Vector(v, &nuevo);

    printf("Piloto %s dado de alta con ID %u.\n", nuevo.nombre, nuevo.id);
    return OK;
}

char* obtenerNombre(const void* p)
{
    if(!p)
        return NULL;
    return ((t_piloto*)p)->nombre;
}

unsigned obtenerPuntos(const void* p)
{
    if(!p)
        return 0;
    return ((t_piloto*)p)->puntos_acumulados;
}

void mostrarRanking(tda_vector* v)
{
    //POS NOMBRE PUNTOS
    //POS AUTOINC
    int i;
    int puesto_real = 1;
    char* nombre;
    char str_puesto[8];
    unsigned puntos_actual;
    void* piloto_actual;
    void* piloto_anterior;
    tda_vector temp;

    if(crear_Vector(&temp,v->tam) != OK)
    {
        puts("No es posible generar el ranking.");
        return;
    }
    temp.ce = v->ce;
    temp.vec = malloc(temp.ce * temp.tam);
    if(!temp.vec)
    {
        puts("Error de asignación de memoria.");
        destruir_Vector(&temp);
        return;
    }

    memcpy(temp.vec, v->vec, temp.ce * temp.tam);

    sSort(temp.vec,temp.ce,temp.tam,compararPuntos);

    listarPilotos(&temp);

    system("cls");
    puts("=======================================================");
    printf("| %-8s | %-30s | %-7s |\n","POSICIÓN", "PILOTO", "PUNTOS");
    puts("=======================================================");

    for(i=0;i<temp.ce;i++)
    {
        piloto_actual = (char*)temp.vec + (i*temp.tam);

        nombre = obtenerNombre(piloto_actual);
        puntos_actual = obtenerPuntos(piloto_actual);

        if(i > 0)
        {
            piloto_anterior = (char*)temp.vec + ((i - 1) * temp.tam);
            if(puntos_actual < obtenerPuntos(piloto_anterior))
            {
                puesto_real = i + 1;
            }
        }

        sprintf(str_puesto, "%d°", puesto_real);
        printf("| %-8s | %-30s | %-4u pts|\n", str_puesto, nombre, puntos_actual);
    }

    puts("=======================================================");
    destruir_Vector(&temp);
}
