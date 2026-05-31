#include "pilotos.h"
#include <ctype.h>

void __menuPilotos(FILE* pilotos, FILE* escuderias)
{
    int op;

    do{
        puts("\n\n===============================================");
        printf("\t\t   PILOTOS\n");
        puts("===============================================");
        printf("1. Listar pilotos y puntajes(FUNCIONA)\n");
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
//            case 2: altaPiloto(pilotos, escuderias); break;
//            case 3: bajaPiloto(pilotos); break;
//            case 4: modificarPiloto(pilotos, escuderias); break;
//            case 5: mostrarRanking(pilotos); break;
            case 6: ///elegir 1 -> Preguntar a Caro
//                listarPilotosPorEscuderia_Op1(pilotos, escuderias);//OK
                //listarPilotosPorEscuderia_Op2(pilotos, escuderias);//OK
                //PUEDEN SER AMBAS
                break;
            case 7: puts("Exportar pilotos"); break;
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

unsigned generarNuevoId(const char* pilotos)
{
    FILE* arch = fopen(pilotos,"rb");
    if(!arch) return 1;

    t_piloto pil;
    unsigned maxId = 0;

    while(fread(&pil,sizeof(t_piloto),1,arch)==1)
    {
        if(pil.id>maxId)
            maxId=pil.id;
    }
    fclose(arch);
    return maxId + 1;
}

/*int altaPiloto(const char* pilotos, const char* escuderias)
{
    t_piloto nuevo;
    char fechaStr[20];

    nuevo.id = generarNuevoId(pilotos);

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
    }while(escuderiaValida(nuevo.id_escuderia, escuderias) != OK);

    nuevo.puntos_acumulados = 0;
    nuevo.estado = 'A';

    printf("Fecha nacimiento (AAAAMMDD): ");
    fflush(stdin);
    fgets(fechaStr, sizeof(fechaStr), stdin);
    fechaStr[strlen(fechaStr) - 1] = '\0';
    sscanf(fechaStr, "%I64u", &nuevo.fechaNacimiento);

    FILE *arch = fopen(pilotos,"ab");
    if(!arch) return ERROR;
    fwrite(&nuevo,sizeof(t_piloto),1,arch);
    fclose(arch);


    printf("Piloto %s dado de alta con ID %u.\n", nuevo.nombre, nuevo.id);
    return OK;
}
*/
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

/*int bajaPiloto(const char* pilotos)
{
    unsigned idbuscado;
    t_piloto pil;
    FILE* arch;
    FILE* fbajas;
    int encontrado=0;

    printf("\n==== BAJA DE PILOTO ====\n");
    printf("ID del piloto a dar de baja: ");
    scanf("%u", &idbuscado);

    arch = fopen(pilotos,"r+b");
    if(!arch) return ERROR;

    while(!encontrado && fread(&pil,sizeof(t_piloto),1,arch)==1)
          {
              if(pil.id==idbuscado)
                {
                    encontrado=1;
                    if(pil.estado=='R')
                    {
                        printf("El piloto ya fue dado de baja.\n");
                        fclose(arch);
                        return ERROR;
                    }
                    pil.estado='R';
                    fseek(arch, -sizeof(t_piloto),SEEK_CUR);
                    fwrite(&pil,sizeof(t_piloto),1,arch);

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
    fclose(arch);
    return encontrado ? OK : ERROR;
}
*/
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

/*int modificarPiloto(const char* pilotos, const char* escuderias)
{
    unsigned idBuscado;
    t_piloto piloto;
    char fechaStr[20];
    unsigned nuevoIdEsc;
    FILE* arch;
    int encontrado=0;

    printf("\n==== MODIFICAR PILOTO ====\n");
    printf("ID del piloto a modificar: ");
    scanf("%u", &idBuscado);

    arch = fopen(pilotos,"r+b");
    if(!arch) return ERROR;

    while(!encontrado && fread(&piloto,sizeof(t_piloto),1,arch)==1)
    {
        if(piloto.id==idBuscado)
        {
            encontrado=1;

            printf("\nNombre: (%s): ", piloto.nombre);
            if(confirmarModificacion("Modificar nombre"))
            {
                printf("\nNombre (%s): ", piloto.nombre);
                fflush(stdin);
                fgets(piloto.nombre, sizeof(piloto.nombre), stdin);
                piloto.nombre[strlen(piloto.nombre) - 1] = '\0';
            }

            printf("\nNacionalidad: %s\n", piloto.nacionalidad);
            if(confirmarModificacion("Modificar nacionalidad"))
            {
                printf("Nacionalidad (%s): ", piloto.nacionalidad);
                fflush(stdin);
                fgets(piloto.nacionalidad, sizeof(piloto.nacionalidad), stdin);
                piloto.nacionalidad[strlen(piloto.nacionalidad) - 1] = '\0';
            }

            printf("\nID Escuderia: %u\n", piloto.id_escuderia);
            if(confirmarModificacion("Modificar escuderia"))
            {
                do{
                    printf("ID escuderia: ");
                    scanf("%u", &nuevoIdEsc);
                }while(escuderiaValida(nuevoIdEsc, escuderias) != OK);
                piloto.id_escuderia = nuevoIdEsc;
            }

            printf("\nEstado: %c\n", piloto.estado);
            if(confirmarModificacion("Modificar estado"))
            {
                if(piloto.estado == 'A')
                {
                    piloto.estado = 'S';
                }
                else
                {
                    piloto.estado = 'A';
                }
            }

            FECHA_FORMATO(piloto.fechaNacimiento, anio, mes, dia)
            printf("\nFecha nacimiento (%04u-%02u-%02u)\n", anio, mes, dia); //acá se usa la macro para mostrar bien por consola la fecha
            if(confirmarModificacion("Modificar fecha de nacimiento"))
            {
                printf("\nFecha nacimiento (AAAAMMDD) (%I64u): ", piloto.fechaNacimiento);
                fflush(stdin);
                fgets(fechaStr, sizeof(fechaStr), stdin);
                fechaStr[strlen(fechaStr) - 1] = '\0';
                sscanf(fechaStr, "%I64u", &piloto.fechaNacimiento);
            }

            fseek(arch,-sizeof(t_piloto),SEEK_CUR);
            fwrite(&piloto,sizeof(t_piloto),1,arch);

            printf("\nPiloto modificado correctamente!\n");
        }
    }
    if(!encontrado)
        printf("Piloto no encontrado.\n");
    fclose(arch);
    return encontrado ? OK : ERROR;
}*/


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

void mostrarRanking(const char* pilotos)
{
    FILE * arch = fopen(pilotos,"rb");
    if(!arch) return;

    tda_vector ranking;
    t_piloto pil;
    int i,puesto_real = 1;

    crear_Vector(&ranking,sizeof(t_piloto));

    while(fread(&pil,sizeof(t_piloto),1,arch)==1)
    {
        if(pil.estado=='A'||pil.estado=='S')
            insertarAlFinal_Vector(&ranking,&pil);
    }
    fclose(arch);

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

/*void listarPilotosPorEscuderia_Op1(tda_vector* v, tda_vector* esc)
{
    t_escuderia* escu = (t_escuderia*)esc->vec;
    t_piloto* pil;
    int i, j;

    for(i = 0; i < esc->ce; i++)
    {
        printf("\n\n=============\t %15s \t=============\n", escu->nombre);
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
*/
void listarPilotosPorEscuderia_Op2(const char* pilotos, const char* escuderias)
{
    t_escuderia esc;
    t_piloto pil;
    unsigned idEsc;
    int encontradoEsc = 0,encontradoPil = 0;

    printf("\nID escuderia: ");
    scanf("%u", &idEsc);

    FILE* archEsc = fopen(escuderias,"rb");
    if(!archEsc) return;

    while(!encontradoEsc && fread(&esc,sizeof(t_escuderia),1,archEsc)==1)
    {
        if(esc.id==idEsc) encontradoEsc=1;
    }
    fclose(archEsc);

    if(!encontradoEsc)
    {
        printf("Escuderia no encontrada.\n");
        return;
    }

    system("cls");
    printf("\n\n=============\t %15s \t=============\n", esc.nombre);
    puts("-----------------------------------------------------");
    printf("| %-3s | %-30s | %6s | %s |\n", "ID", "NOMBRE", "PUNTOS", "E");
    puts("-----------------------------------------------------");


    FILE* archPil=fopen(pilotos,"rb");
    if(archPil)
    {
        while(fread(&pil,sizeof(t_piloto),1,archPil)==1)
        {
            if(pil.id_escuderia==idEsc&&(pil.estado=='A'||pil.estado=='S'))
            {
                printf("| %-3u | %-30s | %-6u | %c |\n",pil.id, pil.nombre, pil.puntos_acumulados, pil.estado);
                encontradoPil=1;
            }
        }
        fclose(archPil);
    }
    puts("=====================================================");
    if(!encontradoPil)
        printf("No hay pilotos para esta escuderia.\n");
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
