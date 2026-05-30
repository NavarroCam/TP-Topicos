#include "escuderias.h"

void menuEscuderias(tda_vector* escuderias)
{
    int op;
    do{
        printf("\n--- ESCUDERIAS ---\n");
        printf("1. Listar escuderias\n");
        printf("2. Alta escuderia\n");
        printf("3. Baja escuderia\n");
        printf("0. Volver\n");
        printf("Opción: ");
        scanf("%d", &op);
        system("cls");

        switch (op)
        {
            case 0: break;
            case 1: listarEscuderias(escuderias); break;
            case 2: altaEscuderias(escuderias); break;
            //case 3: bajaEscuderia(escuderias); break;
            default:
                printf("Opcion inválida.\n");
        }
    }while(op != 0);
}
//void listarEscuderias(const char *binPath)
//{
//    t_escuderia e1;
//    t_escuderia *escuderia = &e1;
//    FILE *fbin;
//    int contador;
//
//    fbin = fopen(binPath, "rb");
//    if (fbin == NULL)
//    {
//        printf("Error al abrir %s\n", binPath);
//        return;
//    }
//
//    printf("\n===== ESCUDERIAS =====\n");
//    contador = 0;
//    while (fread(escuderia, sizeof(t_escuderia), 1, fbin) == 1)
//    {
//        printf("ID: %u | Codigo: %s | Nombre: %s | Pais: %s | Estado: %d\n",
//               escuderia->id,
//               escuderia->codigo,
//               escuderia->nombre,
//               escuderia->pais,
//               escuderia->estado);
//        contador++;
//    }
//    printf("Total escuderias leidas: %d\n", contador);
//
//    fclose(fbin);
//}
void listarEscuderias(const tda_vector* v)
{
    system("cls");
    puts("==================================================================================");
    printf("| %-3s | %-6s | %-30s | %-30s |\n", "ID", "CODIGO", "NOMBRE", "PAIS");
    puts("==================================================================================");
    map_(v->vec,v->ce,v->tam,mostrarEscuderia);
}
void mostrarEscuderia(void* escuderias)
{

    t_escuderia* esc = (t_escuderia*)escuderias;
    if(esc->estado == 1)
    {
        printf("| %-3u | %-6s | %-30s | %-30s |\n",
               esc->id,
               esc->codigo,
               esc->nombre,
               esc->pais);
    }
}
int compararIdEscuderia(const void* a, const void* b)
{
    unsigned idBuscado = *(unsigned*)a;
    unsigned idEsc = ((t_escuderia*)b)->id;
    if(idBuscado < idEsc) return -1;
    if(idBuscado > idEsc) return  1;
    return 0;
}

int escuderiaValida(unsigned idEsc, tda_vector* v)
{
    t_escuderia* resultado;

    resultado = (t_escuderia*)bsearch(&idEsc, v->vec, v->ce, sizeof(t_escuderia), compararIdEscuderia);

    if(!resultado)
    {
        printf("Escuderia no encontrada.\n");
        return ERROR;
    }

    if(resultado->estado != 1)
    {
        printf("La escuderia %s esta inactiva.\n", resultado->nombre);
        return ERROR;
    }
    return OK;
}
int altaEscuderias(tda_vector* esc)
{
    t_escuderia nuevo;
    int aux;
    char *p;

    nuevo.id = generarNuevoIdEscuderias(esc);
    do{
        printf("==== ALTA ESCUDERIA ====\n");
        printf("Codigo: ");
        fflush(stdin);
        fgets(nuevo.codigo, sizeof(nuevo.codigo), stdin);
        if ((p = strchr(nuevo.codigo, '\n')) != NULL)
            *p = '\0';
        aux = BuscarCodigoEscuderia(esc,nuevo.codigo);
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
    if ((p = strchr(nuevo.nombre, '\n')) != NULL)
        *p = '\0';

    printf("Pais: ");
    fflush(stdin);
    fgets(nuevo.pais, sizeof(nuevo.pais), stdin);
    if ((p = strchr(nuevo.pais, '\n')) != NULL)
        *p = '\0';

    nuevo.estado = 1;

    insertarAlFinal_Vector(esc, &nuevo);

    printf("Escuderia %s dado de alta con ID %u.\n", nuevo.nombre, nuevo.id);
    return OK;
}

int BuscarCodigoEscuderia(tda_vector *escuderias, char* cod)
{
    size_t cant = escuderias->ce;
    size_t i = 0;
    int band = ERROR;
    t_escuderia tEsc;

    while(i<cant && band != OK)
    {
        memcpy(&tEsc,(escuderias->vec)+(i*sizeof(t_escuderia)),sizeof(t_escuderia));
        if(strcmp(tEsc.codigo,cod) == 0)
            band = OK;
        i++;
    }
    return band;
}

unsigned generarNuevoIdEscuderias(tda_vector* v)
{
    t_escuderia* esc = (t_escuderia*)v->vec;
    unsigned maxId = 0;
    int i;

    for(i = 0; i < v->ce; i++)
    {
        if(esc->id > maxId)
            maxId = esc->id;
        esc++;
    }
    return maxId + 1;
}
