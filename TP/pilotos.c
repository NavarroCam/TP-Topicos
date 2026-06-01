#include "pilotos.h"
#include <ctype.h>

void __menuPilotos(FILE* pilotos, FILE* escuderias)
{
    int op;

    do{
        puts("\n\n===============================================");
        printf("\t\t   PILOTOS\n");
        puts("===============================================");
        printf("1. Listar pilotos y puntajes)\n");
        printf("2. Alta piloto\n");
        printf("3. Baja piloto\n");
        printf("4. Modificar piloto\n");
        printf("5. Mostrar ranking\n");
        printf("6. Exportar pilotos\n");
        printf("0. Volver\n");

        printf("\nOpcion: ");
        scanf("%d", &op);
        while (getchar() != '\n');
        system("cls");

        switch (op)
        {
            case 1: listarPilotos(pilotos); break;
            case 2: altaPiloto(pilotos, escuderias); break;
            case 3: bajaPiloto(pilotos); break;
            case 4: modificarPiloto(pilotos, escuderias); break;
            case 5: mostrarRanking(pilotos); break;
            case 6: puts("Exportar pilotos"); break;
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
        printf("| %-30s | %-6u |\n",
               pil->nombre,
               pil->puntos_acumulados);
    }
}

void listarPilotos(FILE* pilotos)
{
    t_piloto pil;

    rewind(pilotos);
    system("cls");
    puts("===========================================");
    printf("| %-30s | %-6s |\n", "NOMBRE", "PUNTOS");
    puts("===========================================");

    while(fread(&pil,sizeof(t_piloto),1,pilotos)==1)
    {
        if(pil.estado=='A'||pil.estado=='S')
            printf("| %-30s | %-6u |\n",pil.nombre,pil.puntos_acumulados);
    }
    puts("===========================================");
}

unsigned generarNuevoId(FILE* pilotos)
{
    t_piloto pil;
    unsigned maxId = 0;
    rewind(pilotos);

    while(fread(&pil,sizeof(t_piloto),1,pilotos)==1)
    {
        if(pil.id>maxId)
            maxId=pil.id;
    }
    return maxId + 1;
}

int altaPiloto(FILE* pilotos, FILE* escuderias)
{
    t_piloto nuevo;
    char fechaStr[20];
    char *p;

    nuevo.id = generarNuevoId(pilotos);

    puts("==== ALTA DE PILOTO ====");

    printf("Nombre: ");
    fflush(stdin);
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);
    if((p = strchr(nuevo.nombre, '\n')) != NULL)
            *p = '\0';
        else while(getchar() != '\n');

    printf("Nacionalidad: ");
    fflush(stdin);
    fgets(nuevo.nacionalidad, sizeof(nuevo.nacionalidad), stdin);
    if((p = strchr(nuevo.nacionalidad, '\n')) != NULL)
            *p = '\0';
        else while(getchar() != '\n');

    do{
        printf("ID escuderia: ");
        scanf("%u", &nuevo.id_escuderia);
    }while(escuderiaValida(nuevo.id_escuderia, escuderias) != OK);

    nuevo.puntos_acumulados = 0;
    nuevo.estado = 'A';
    getchar();
    do {
        printf("Fecha nacimiento (AAAAMMDD): ");
        fflush(stdin);
        fgets(fechaStr, sizeof(fechaStr), stdin);
        if((p = strchr(fechaStr, '\n')) != NULL)
            *p = '\0';
        else while(getchar() != '\n');

    } while (ValidarFecha(fechaStr) != OK);
    sscanf(fechaStr, "%I64u", &nuevo.fechaNacimiento);

    fseek(pilotos,0,SEEK_END);
    fwrite(&nuevo,sizeof(t_piloto),1,pilotos);
    fflush(pilotos);


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

int bajaPiloto(FILE* pilotos)
{
    unsigned idbuscado;
    t_piloto pil;
    FILE* fbajas;
    int encontrado=0;

    printf("\n==== BAJA DE PILOTO ====\n");
    printf("ID del piloto a dar de baja: ");
    scanf("%u", &idbuscado);

    rewind(pilotos);
    while(!encontrado && fread(&pil,sizeof(t_piloto),1,pilotos)==1)
          {
              if(pil.id==idbuscado)
                {
                    encontrado=1;
                    if(pil.estado=='R')
                    {
                        printf("El piloto ya fue dado de baja.\n");
                        return ERROR;
                    }
                    pil.estado='R';
                    fseek(pilotos, -(long)sizeof(t_piloto),SEEK_CUR);
                    fwrite(&pil,sizeof(t_piloto),1,pilotos);
                    fflush(pilotos);

                    fbajas = fopen(BAJAS_PILOTOS_DAT, "ab");
                    if(fbajas)
                    {
                        fwrite(&pil, sizeof(t_piloto), 1, fbajas);
                        fclose(fbajas);
                        exportarBajasPilotosTxt(BAJAS_PILOTOS_DAT, BAJAS_PILOTOS_TXT); //Para leer los pilotos en el txt
                    }

                    printf("Piloto %s dado de baja.\n", pil.nombre);
                }
          }

    if(!encontrado) printf("Piloto no encontrado.\n");
    return encontrado ? OK : ERROR;
}



int modificarPiloto(FILE* pilotos, FILE* escuderias)
{
    unsigned idBuscado;
    t_piloto piloto;
    char fechaStr[20];
    unsigned nuevoIdEsc;
    int encontrado=0;
    char *p;

    printf("\n==== MODIFICAR PILOTO ====\n");
    printf("ID del piloto a modificar: ");
    scanf("%u", &idBuscado);
    rewind(pilotos);

    while(!encontrado && fread(&piloto,sizeof(t_piloto),1,pilotos)==1)
    {
        if(piloto.id==idBuscado)
        {
            encontrado=1;

            printf("\nNombre: %s\n", piloto.nombre);
            if(confirmarModificacion("Modificar nombre"))
            {
                getchar();
                printf("Nombre: ");
                fgets(piloto.nombre, sizeof(piloto.nombre), stdin);
                if((p = strchr(piloto.nombre, '\n')) != NULL)
                    *p = '\0';
                else while(getchar() != '\n');

            }

            printf("\nNacionalidad: %s\n", piloto.nacionalidad);
            if(confirmarModificacion("Modificar nacionalidad"))
            {
                getchar();
                printf("Nacionalidad: ");
                fgets(piloto.nacionalidad, sizeof(piloto.nacionalidad), stdin);
                if((p = strchr(piloto.nacionalidad, '\n')) != NULL)
                    *p = '\0';
                else while(getchar() != '\n');
            }

            printf("\nID Escuderia: %u\n", piloto.id_escuderia);
            if(confirmarModificacion("Modificar escuderia"))
            {
                getchar();
                do{
                    printf("ID escuderia: ");
                    scanf("%u", &nuevoIdEsc);
                }while(escuderiaValida(nuevoIdEsc, escuderias) != OK);
                piloto.id_escuderia = nuevoIdEsc;
            }

            printf("\nEstado: %c\n", piloto.estado);
            if(confirmarModificacion("Modificar estado"))
            {
                getchar();
                if(piloto.estado == 'A')
                    piloto.estado = 'S';
                else
                    piloto.estado = 'A';
            }

            FECHA_FORMATO(piloto.fechaNacimiento, anio, mes, dia)
            printf("\nFecha nacimiento (%04u-%02u-%02u)\n", anio, mes, dia); //acá se usa la macro para mostrar bien por consola la fecha
            if(confirmarModificacion("Modificar fecha de nacimiento"))
            {
                getchar();
                do{
                    printf("Fecha nacimiento (AAAAMMDD): ");
                    fgets(fechaStr, sizeof(fechaStr), stdin);
                    if((p = strchr(fechaStr, '\n')) != NULL)
                    *p = '\0';
                else while(getchar() != '\n');
                } while (ValidarFecha(fechaStr) != OK);
                sscanf(fechaStr, "%I64u", &piloto.fechaNacimiento);
            }

            fseek(pilotos,-(long)sizeof(t_piloto),SEEK_CUR);
            fwrite(&piloto,sizeof(t_piloto),1,pilotos);
            fflush(pilotos);

            printf("\nPiloto modificado correctamente!\n");
        }
    }
    if(!encontrado)
        printf("Piloto no encontrado.\n");
    return encontrado ? OK : ERROR;
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

void mostrarRanking(FILE* pilotos)
{
    tda_vector ranking;
    t_piloto pil;
    int i,puesto_real = 1;

    crear_Vector(&ranking,sizeof(t_piloto));
    rewind(pilotos);

    while(fread(&pil,sizeof(t_piloto),1,pilotos)==1)
    {
        if(pil.estado=='A'||pil.estado=='S')
            insertarAlFinal_Vector(&ranking,&pil);
    }

    sSort(ranking.vec,ranking.ce,ranking.tam,compararPuntos);

    system("cls");
    puts("=======================================================");
    printf("| %-8s | %-30s | %-7s |\n","POSICIÓN", "PILOTO", "PUNTOS");
    puts("=======================================================");

    for(i=0;i<ranking.ce;i++)
    {
        t_piloto* piloto_actual = (t_piloto*)((char*)ranking.vec + (i*ranking.tam));

        if(i > 0)
        {
            t_piloto* piloto_anterior = (t_piloto*)((char*)ranking.vec + ((i - 1) * ranking.tam));
            if(piloto_actual->puntos_acumulados < piloto_anterior->puntos_acumulados)
            {
                puesto_real = i + 1;
            }
        }
        printf("| %-8d | %-30s | %-4u pts|\n",puesto_real, piloto_actual->nombre, piloto_actual->puntos_acumulados);
    }

    puts("=======================================================");
    destruir_Vector(&ranking);
}

void mostrarPilotoCarrera(void* pilotos)
{
    t_piloto* pil = (t_piloto*)pilotos;
    if(pil->estado == 'A' || pil->estado == 'S')
    {
        printf("| %-8u | %-30s |\n",
               pil->id,
               pil->nombre);
    }
}
