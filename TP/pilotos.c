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
        puts("\n\n===============================================");
        printf("\t\t   PILOTOS\n");
        puts("===============================================");
        printf("1. Listar pilotos y puntajes(FUNCIONA)\n"); //sacar columna de estado, solo está para checkear las funciones posteriores
        printf("2. Alta piloto(FUNCIONA)\n");
        printf("3. Baja piloto(FUNCIONA)\n");
        printf("4. Modificar piloto(FUNCIONA)\n");
        printf("5. Mostrar ranking(FUNCIONA)\n");
        printf("6. Pilotos por escuderia(FUNCIONAN LAS 2)\n");
        printf("7. Exportar pilotos\n");
        printf("0. Volver\n");

        printf("\nOpcion: ");
        scanf("%d", &op);
        system("cls");

        switch (op)
        {
            case 1: listarPilotos(pilotos); break;
            case 2: altaPiloto(pilotos, escuderias); break;
                //_altaPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
            case 3: bajaPiloto(pilotos); break;
            case 4: modificarPiloto(pilotos, escuderias); break;
                //_modificarPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
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
            case 5: mostrarRanking(pilotos); break;

            case 6: //elegir 1 -> Preguntar a Caro
                //listarPilotosPorEscuderia_Op1(pilotos, escuderias);
                listarPilotosPorEscuderia_Op2(pilotos, escuderias);
                break;
//            case 7: exportarPilotos(PILOTOS_DAT, EXP_PILOTOS); break;
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

int compararIdPiloto(const void* a, const void* b)
{
    unsigned idBuscado = *(unsigned*)a;
    unsigned idPiloto = ((t_piloto*)b)->id;
    if(idBuscado < idPiloto) return -1;
    if(idBuscado > idPiloto) return  1;
    return 0;
}

void mostrarPiloto(void* pilotos)
{
    t_piloto* pil = (t_piloto*)pilotos;
    if(pil->estado == 'A' || pil->estado == 'S')
    {
        printf("| %-30s | %-6u | %c |\n",
               pil->nombre,
               pil->puntos_acumulados,
               pil->estado);
    }
}

void listarPilotos(tda_vector* v)
{
    system("cls");
    puts("===============================================");
    printf("| %-30s | %-6s | %s |\n", "NOMBRE", "PUNTOS", "E");
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

int exportarBajasPilotosTxt(const char* binPath, const char* txtPath)
{
    t_piloto p;
    FILE* fbin;
    FILE* ftxt;

    fbin = fopen(binPath, "rb");
    if(!fbin)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR;
    }

    ftxt = fopen(txtPath, "wt");
    if(!ftxt)
    {
        printf("Error al abrir %s\n", txtPath);
        fclose(fbin);
        return ERROR;
    }

    while(fread(&p, sizeof(t_piloto), 1, fbin) == 1)
    {
        fprintf(ftxt, "%u;%s;%s;%u;%u;%c;%I64u\n",
                p.id, p.nombre, p.nacionalidad,
                p.id_escuderia, p.puntos_acumulados,
                p.estado, p.fechaNacimiento);
    }

    fclose(fbin);
    fclose(ftxt);
    return OK;
}

int bajaPiloto(tda_vector* v)
{
    unsigned id;
    t_piloto* piloto;
    FILE* fbajas;

    printf("\n==== BAJA DE PILOTO ====\n");
    printf("ID del piloto a dar de baja: ");
    scanf("%u", &id);

    piloto = bsearch(&id, v->vec, v->ce, sizeof(t_piloto), compararIdPiloto);
    if(!piloto)
    {
        printf("Piloto no encontrado.\n");
        return ERROR;
    }
    if(piloto->estado == 'R')
    {
        printf("El piloto ya esta dado de baja.\n");
        return ERROR;
    }

    piloto->estado = 'R';

    fbajas = fopen(BAJAS_PILOTOS_DAT, "ab");
    if(!fbajas)
    {
        printf("Error al abrir archivo de bajas.\n");
        return ERROR;
    }
    fwrite(piloto, sizeof(t_piloto), 1, fbajas);
    fclose(fbajas);
    exportarBajasPilotosTxt(BAJAS_PILOTOS_DAT, BAJAS_PILOTOS_TXT); //Para leer los pilotos en el txt

    printf("Piloto %s dado de baja.\n", piloto->nombre);
    return OK;
}

int confirmarModificacion(const char *mensaje)
{
    char opcion;

    do{
        printf("%s (S/N): ", mensaje);
        scanf(" %c", &opcion);
        opcion = toupper(opcion);
        if(opcion != 'S' && opcion != 'N')
        {
            printf("Opcion invalida.\n");
        }
    }while(opcion != 'S' && opcion != 'N');

    return opcion == 'S';
}

int modificarPiloto(tda_vector* v, tda_vector* esc)
{
    unsigned id;
    t_piloto* piloto;
    char fechaStr[20];
    unsigned nuevoIdEsc;

    printf("\n==== MODIFICAR PILOTO ====\n");
    printf("ID del piloto a modificar: ");
    scanf("%u", &id);

    piloto = bsearch(&id, v->vec, v->ce, sizeof(t_piloto), compararIdPiloto);
    if(!piloto)
    {
        printf("Piloto no encontrado.\n");
        return ERROR;
    }

    printf("\nNombre: %s\n", piloto->nombre);
    if(confirmarModificacion("Modificar nombre"))
    {
        printf("\nNombre (%s): ", piloto->nombre);
        fflush(stdin);
        fgets(piloto->nombre, sizeof(piloto->nombre), stdin);
        piloto->nombre[strlen(piloto->nombre) - 1] = '\0';
    }

    printf("\nNacionalidad: %s\n", piloto->nacionalidad);
    if(confirmarModificacion("Modificar nacionalidad"))
    {
        printf("Nacionalidad (%s): ", piloto->nacionalidad);
        fflush(stdin);
        fgets(piloto->nacionalidad, sizeof(piloto->nacionalidad), stdin);
        piloto->nacionalidad[strlen(piloto->nacionalidad) - 1] = '\0';
    }

    printf("\nID Escuderia: %u\n", piloto->id_escuderia);
    if(confirmarModificacion("Modificar escuderia"))
    {
        do{
            printf("ID escuderia: ");
            scanf("%u", &nuevoIdEsc);
        }while(escuderiaValida(nuevoIdEsc, esc) != OK);
        piloto->id_escuderia = nuevoIdEsc;
    }

    printf("\nEstado: %c\n", piloto->estado);
    if(confirmarModificacion("Modificar estado"))
    {
        if(piloto->estado == 'A')
        {
            piloto->estado = 'S';
        }
        else
        {
            piloto->estado = 'A';
        }
    }

    FECHA_FORMATO(piloto->fechaNacimiento, anio, mes, dia)
    printf("\nFecha nacimiento (%04u-%02u-%02u)\n", anio, mes, dia); //acá se usa la macro para mostrar bien por consola la fecha
    if(confirmarModificacion("Modificar fecha de nacimiento"))
    {
        printf("\nFecha nacimiento (AAAAMMDD) (%I64u): ", piloto->fechaNacimiento);
        fflush(stdin);
        fgets(fechaStr, sizeof(fechaStr), stdin);
        fechaStr[strlen(fechaStr) - 1] = '\0';
        sscanf(fechaStr, "%I64u", &piloto->fechaNacimiento);
    }

    printf("\nPiloto modificado correctamente!\n");
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



void listarPilotosPorEscuderia_Op1(tda_vector* pilotos, tda_vector* escuderias)
{
    t_escuderia* esc = (t_escuderia*)escuderias->vec;
    t_piloto* pil;
    int i, j;

    for(i = 0; i < escuderias->ce; i++)
    {
        printf("\n\n=============\t %15s \t=============\n", esc->nombre);
        puts("-----------------------------------------------------");
        printf("| %-3s | %-30s | %6s | %s |\n",
               "ID", "NOMBRE", "PUNTOS", "E");
        puts("-----------------------------------------------------");

        pil = (t_piloto*)pilotos->vec;
        for(j = 0; j < pilotos->ce; j++)
        {
            if(pil->id_escuderia == esc->id && (pil->estado == 'A' || pil->estado == 'S'))
            {
                printf("| %-3u | %-30s | %6u | %c |\n",
                       pil->id,
                       pil->nombre,
                       pil->puntos_acumulados,
                       pil->estado);
            }
            pil++;
        }
        puts("=====================================================");
        esc++;
    }
}

void listarPilotosPorEscuderia_Op2(tda_vector* pilotos, tda_vector* escuderias)
{
    t_escuderia* esc;
    t_piloto* pil;
    unsigned idEsc;
    int i, encontrado = 0;

    printf("\nID escuderia: ");
    scanf("%u", &idEsc);

    esc = (t_escuderia*)bsearch(&idEsc, escuderias->vec, escuderias->ce, sizeof(t_escuderia), compararIdEscuderia);
    if(!esc)
    {
        printf("Escuderia no encontrada.\n");
        return;
    }

    system("cls");
    printf("\n\n=============\t %15s \t=============\n", esc->nombre);
    puts("-----------------------------------------------------");
    printf("| %-3s | %-30s | %6s | %s |\n", "ID", "NOMBRE", "PUNTOS", "E");
    puts("-----------------------------------------------------");

    pil = (t_piloto*)pilotos->vec;
    for(i = 0; i < pilotos->ce; i++)
    {
        if(pil->id_escuderia == idEsc && (pil->estado == 'A' || pil->estado == 'S'))
        {
            printf("| %-3u | %-30s | %-6u | %c |\n", pil->id, pil->nombre, pil->puntos_acumulados, pil->estado);
            encontrado = 1;
        }
        pil++;
    }
    puts("=====================================================");
    if(!encontrado)
        printf("No hay pilotos para esta escuderia.\n");
}
