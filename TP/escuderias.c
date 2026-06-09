#include "escuderias.h"

void menuEscuderias(FILE* pilotos, FILE* escuderias)
{
    int op;
    do{
        puts("\n\n===============================================");
        printf("\t\t ESCUDERIAS\n");
        puts("===============================================");
        printf("1. Listar escuderias\n");
        printf("2. Alta escuderia\n");
        printf("3. Baja escuderia\n");
        printf("4. Modificar escuderia\n");
        printf("0. Volver\n");

        printf("\nOpción: ");
        scanf("%d", &op);

        while (getchar() != '\n');
        system("cls");

        switch (op)
        {
            case 1: listarEscuderias(escuderias); break;
            case 2: altaEscuderias(escuderias); break;
            case 3: bajaEscuderia(escuderias); break;
            case 4: modificarEscuderia(escuderias); break;
            case 0: break;
            default:
                printf("Opcion inválida.\n");
        }
    }while(op != 0);
}
void listarEscuderias(FILE* escuderias)
{
    t_escuderia esc;

    rewind(escuderias);
    system("cls");
    puts("==================================================================================");
    printf("| %-3s | %-6s | %-30s | %-30s |\n", "ID", "CODIGO", "NOMBRE", "PAIS");
    puts("==================================================================================");
    while(fread(&esc,sizeof(t_escuderia),1,escuderias) == 1)
    {
        if(esc.estado == 1)
            printf("| %-3u | %-6s | %-30s | %-30s |\n",esc.id,
                                                       esc.codigo,
                                                       esc.nombre,
                                                       esc.pais);
    }
    puts("==================================================================================");
}
//
//int compararIdEscuderia(const void* a, const void* b)
//{
//    unsigned idBuscado = *(unsigned*)a;
//    unsigned idEsc = ((t_escuderia*)b)->id;
//    if(idBuscado < idEsc) return -1;
//    if(idBuscado > idEsc) return  1;
//    return 0;
//}
//
int escuderiaValida(unsigned idEsc, FILE* escuderias)
{
    t_escuderia esc;
    int encontrado=0;
    rewind(escuderias);

    while(!encontrado && fread(&esc,sizeof(t_escuderia),1,escuderias)==1)
    {
        if(esc.id==idEsc)
        {
            encontrado=1;
            if(esc.estado!=1)
            {
                printf("La escuderia %s esta inactiva\n",esc.nombre);
                return ERROR_;
            }
            return TODOOK;
        }
    }
    printf("Escuderia no encontrada.\n");
    return ERROR_;
}

void altaEscuderias(FILE* escuderias)
{
    t_escuderia nuevo;
    int aux;
    char *p;
    nuevo.id = generarNuevoIdEscuderias(escuderias);
    do{
        printf("==== ALTA ESCUDERIA ====\n");
        printf("Codigo: ");
        fgets(nuevo.codigo, sizeof(nuevo.codigo), stdin);
        if((p = strchr(nuevo.codigo, '\n')) != NULL)
            *p = '\0';
        else while(getchar() != '\n');
        aux = BuscarCodigoEscuderia(escuderias,nuevo.codigo);
        if(aux != ERROR_)
        {
            printf("Codigo ya existente\n");
            system("pause");
            system("cls");
        }
    }while(aux != ERROR_);

    printf("Nombre: ");
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);
    if((p = strchr(nuevo.nombre, '\n')) != NULL)
            *p = '\0';
    else while(getchar() != '\n');

    printf("Pais: ");
    fgets(nuevo.pais, sizeof(nuevo.pais), stdin);
    if((p = strchr(nuevo.pais, '\n')) != NULL)
            *p = '\0';
    else while(getchar() != '\n');

    nuevo.estado = 1;

    fseek(escuderias, 0, SEEK_END);
    fwrite(&nuevo, sizeof(t_escuderia), 1, escuderias);
    fflush(escuderias);

    printf("Escuderia %s dado de alta con ID %u.\n", nuevo.nombre, nuevo.id);
}

int BuscarCodigoEscuderia(FILE* escuderias, const char* cod)///ACOMODAR
{
    t_escuderia escuderia;
    int band = ERROR_;

    rewind(escuderias);

    while(fread(&escuderia, sizeof(t_escuderia), 1, escuderias) == 1 && band != TODOOK)
        if (strcmp(escuderia.codigo, cod) == 0)
            band = TODOOK;

    return band;
}

unsigned generarNuevoIdEscuderias(FILE* escuderias)
{
    t_escuderia escuderia;
    unsigned Id = 1;

    fseek(escuderias,0,SEEK_END);
    if(ftell(escuderias) > 0)
    {
        fseek(escuderias,-(long)sizeof(t_escuderia),SEEK_END);
        fread(&escuderia,sizeof(t_escuderia),1,escuderias);
        Id = escuderia.id + 1;
    }
    return Id;
}

int exportarBajasEscuderiasTxt(const char* binPath, const char* txtPath)
{
    t_escuderia e;
    FILE* fbin;
    FILE* ftxt;

    fbin = fopen(binPath, "rb");
    if(!fbin)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR_;
    }

    ftxt = fopen(txtPath, "wt");
    if(!ftxt)
    {
        printf("Error al abrir %s\n", txtPath);
        fclose(fbin);
        return ERROR_;
    }

    while(fread(&e, sizeof(t_escuderia), 1, fbin) == 1)
    {
        fprintf(ftxt, "%u;%s;%s;%s;%d\n",
                e.id, e.codigo, e.nombre,
                e.pais, e.estado);
    }

    fclose(fbin);
    fclose(ftxt);
    return TODOOK;
}

int bajaEscuderia(FILE* escuderias)
{
    unsigned idbuscado;
    t_escuderia esc;
    FILE* fbajas;
    int encontrado=0;

    printf("\n==== BAJA DE ESCUDERIA ====\n");
    printf("ID de la escuderia a dar de baja: ");
    scanf("%u", &idbuscado);

    rewind(escuderias);
    while(!encontrado && fread(&esc,sizeof(t_escuderia),1,escuderias)==1)
    {
        if(esc.id==idbuscado)
        {
            encontrado=1;
            if(esc.estado==0)
            {
                printf("La escuderia ya fue dada de baja anteriormente.\n");
                return ERROR_;
            }

            esc.estado=0;
            fseek(escuderias,-(long)sizeof(t_escuderia),SEEK_CUR);
            fwrite(&esc,sizeof(t_escuderia),1,escuderias);
            fflush(escuderias);

            fbajas = fopen(BAJAS_ESCUDERIAS_DAT, "ab");
            if(!fbajas)
            {
                printf("Error al abrir %s\n", BAJAS_ESCUDERIAS_DAT);
                return ERROR_;
            }
            else{
                fwrite(&esc, sizeof(t_escuderia), 1, fbajas);
                fclose(fbajas);
                exportarBajasEscuderiasTxt(BAJAS_ESCUDERIAS_DAT, BAJAS_ESCUDERIAS_TXT); //Para leer los pilotos en el txt
            }
            printf("Escuderia %s dada de baja exitosamente.\n",esc.nombre);
        }
    }
    if(!encontrado)
    {
        printf("Escuderia no encontrada.\n");
    }
    return encontrado ? TODOOK : ERROR_;
}


int modificarEscuderia(FILE* escuderias)
{
    unsigned idBuscado;
    t_escuderia e;

    int encontrado=0;
    char* p;

    printf("\n==== MODIFICAR PILOTO ====\n");
    printf("ID de la escuderia a modificar: ");
    scanf("%u", &idBuscado);

    rewind(escuderias);
    while(!encontrado && fread(&e,sizeof(t_escuderia),1,escuderias)==1)
    {
        if(e.id==idBuscado)
        {
            encontrado=1;

            printf("\nCodigo: %s\n", e.codigo);
            if(confirmarModificacion("Modificar Codigo"))
            {
                printf("Codigo: ");
                fflush(stdin);
                fgets(e.codigo, sizeof(e.codigo), stdin);
                if ((p = strchr(e.codigo, '\n')) != NULL)
                    *p = '\0';
            }

            printf("\nNombre: %s\n", e.nombre);
            if(confirmarModificacion("Modificar nombre"))
            {
                printf("Nombre: ");
                fflush(stdin);
                fgets(e.nombre, sizeof(e.nombre), stdin);
                e.nombre[strlen(e.nombre) - 1] = '\0';
            }

            printf("\nPais: %s\n", e.pais);
            if(confirmarModificacion("Modificar pais"))
            {
                printf("Pais: ");
                fflush(stdin);
                fgets(e.pais, sizeof(e.pais), stdin);
                e.pais[strlen(e.pais) - 1] = '\0';
            }

            fseek(escuderias,-(long)sizeof(t_escuderia),SEEK_CUR);
            fwrite(&e,sizeof(t_escuderia),1,escuderias);
            fflush(escuderias);

            printf("\nEscuderia modificada correctamente!\n");
        }
    }
    if(!encontrado)
        printf("Escuderia no encontrada.\n");
    return encontrado ? TODOOK : ERROR_;
}
