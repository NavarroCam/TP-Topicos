#include "escuderias.h"

void menuEscuderias(FILE* pilotos, FILE* escuderias)
{
    int op;
    do{
        printf("\n--- ESCUDERIAS ---\n");
        printf("1. Listar escuderias (Funciona)\n");
        printf("2. Alta escuderia (Funciona)\n");
        printf("3. Baja escuderia\n");
        printf("4. Listar pilotos x escuderia\n");
        printf("0. Volver\n");
        printf("Opción: ");
        scanf("%d", &op);
        system("cls");

        switch (op)
        {
            case 1: listarEscuderias(escuderias); break;
            case 2: altaEscuderias(escuderias); break;
            case 3: bajaEscuderia(escuderias); break;
//            case 4: listarpilotosxescuderia(); break;
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
                return ERROR;
            }
            return OK;
        }
    }
    printf("Escuderia no encontrada.\n");
    return ERROR;
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
        fflush(stdin);
        fgets(nuevo.codigo, sizeof(nuevo.codigo), stdin);
        if((p = strchr(nuevo.codigo, '\n')) != NULL)
            *p = '\0';
        aux = BuscarCodigoEscuderia(escuderias,nuevo.codigo);
        if(aux != ERROR)
        {
            printf("Codigo ya existente\n");
            system("pause");
            system("cls");
        }
    }while(aux != ERROR);

    printf("Nombre: ");
    fflush(stdin);
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);
    if((p = strchr(nuevo.nombre, '\n')) != NULL)
            *p = '\0';

    printf("Pais: ");
    fflush(stdin);
    fgets(nuevo.pais, sizeof(nuevo.pais), stdin);
    if((p = strchr(nuevo.pais, '\n')) != NULL)
            *p = '\0';

    nuevo.estado = 1;

    fseek(escuderias, 0, SEEK_END);
    fwrite(&nuevo, sizeof(t_escuderia), 1, escuderias);
    fflush(escuderias);

    printf("Escuderia %s dado de alta con ID %u.\n", nuevo.nombre, nuevo.id);
}

int BuscarCodigoEscuderia(FILE* escuderias, const char* cod)
{
    t_escuderia escuderia;
    int band = ERROR;

    rewind(escuderias);

    while(fread(&escuderia, sizeof(t_escuderia), 1, escuderias) == 1 && band != OK)
        if (strcmp(escuderia.codigo, cod) == 0)
            band = OK;

    return band;
}
unsigned generarNuevoIdEscuderias(FILE* escuderias)
{
    t_escuderia escuderia;
    unsigned maxId = 0;

    rewind(escuderias);

    while (fread(&escuderia, sizeof(t_escuderia), 1, escuderias) == 1)
        if (escuderia.id > maxId)
            maxId = escuderia.id;

    return maxId + 1;
}
void bajaEscuderia(FILE* escuderias)
{
    unsigned idbuscado;
    t_escuderia esc;
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
                return;
            }
            esc.estado=0;
            fseek(escuderias,-sizeof(t_escuderia),SEEK_CUR);
            fwrite(&esc,sizeof(t_escuderia),1,escuderias);
            fflush(escuderias);
            printf("Escuderia %s dada de baja exitosamente.\n",esc.nombre);
        }
    }
    if(!encontrado)
        printf("Escuderia no encontrada.\n");

}
