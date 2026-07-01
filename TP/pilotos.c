#include "pilotos.h"

void menuPilotos(FILE* pilotos, FILE* escuderias)
{
    int op;

    do
    {
        limpiarPantalla();
        tituloSistema();
        tituloMenu("   GESTIÓN DE PILOTOS");
        color(COLOR_TEXTO);

        printf("   [1] Listar pilotos y puntajes\n");
        printf("   [2] Alta de piloto\n");
        printf("   [3] Baja de piloto\n");
        printf("   [4] Modificar piloto\n");
        printf("   [5] Mostrar ranking\n");
        printf("   [6] Listar por escudería\n");
        printf("   [0] Volver\n");
        color(COLOR_TITULO);
        printf("\n====================================================\n\n");
        restaurarColor();
        printf("Seleccione una opción > ");
        scanf("%d", &op);
        while (getchar() != '\n');

        switch (op)
        {
            case 1:
                listarPilotos(pilotos);
                system("pause");
                break;
            case 2:
                altaPiloto(pilotos, escuderias);
                system("pause");
                break;
            case 3:
                bajaPiloto(pilotos);
                system("pause");
                break;
            case 4:
                modificarPiloto(pilotos, escuderias);
                system("pause");
                break;
            case 5:
                mostrarRanking(pilotos);
                system("pause");
                break;
            case 6:
                listarPilotosPorEscuderia(pilotos, escuderias);
                system("pause");
                break;
            case 0:
                break;
            default:
                printf("Opción inválida.\n");
        }
    }while(op != 0);
}

int compararPuntos(const void* a, const void* b)
{
    t_piloto* pil1 = (t_piloto*)a;
    t_piloto* pil2 = (t_piloto*)b;
    if(pil1->puntos_acumulados != pil2->puntos_acumulados)
    {
        return pil2->puntos_acumulados - pil1->puntos_acumulados;
    }

    return strcmp(pil1->nombre,pil2->nombre);
}

int compararIdPiloto(const void* a, const void* b)
{
    unsigned idA = ((t_piloto*)a)->id;
    unsigned idB = ((t_piloto*)b)->id;
    if(idA < idB) return -1;
    if(idA > idB) return  1;

    return 0;
}

void listarPilotos(FILE* pilotos)
{
    t_piloto pil;

    rewind(pilotos);
    limpiarPantalla();
    tituloSistema();
    tituloMenu("       PILOTOS");
    color(COLOR_MENU_PRINCIPAL);

    printf("  ------------------------------------------------\n");
    printf("  | %-30s | %-11s |\n", "NOMBRE", "PUNTOS");
    printf("  ------------------------------------------------\n");
    restaurarColor();

    while(fread(&pil,sizeof(t_piloto),1,pilotos)==1)
    {
        if(pil.estado=='A' || pil.estado=='S')
            printf("  | %-30s | %11u |\n",pil.nombre,pil.puntos_acumulados);
    }

    color(COLOR_MENU_PRINCIPAL);
    printf("  ------------------------------------------------\n");
    restaurarColor();
}

unsigned generarNuevoIdPilotos(FILE* pilotos)
{
    t_piloto pil;
    fseek(pilotos,0,SEEK_END);
    unsigned Id = 1;

    if(ftell(pilotos) > 0)
    {
        fseek(pilotos,-(long)sizeof(t_piloto),SEEK_END);
        fread(&pil,sizeof(t_piloto),1,pilotos);
        Id = pil.id + 1;
    }

    return Id;
}

int altaPiloto(FILE* pilotos, FILE* escuderias)
{
    t_piloto nuevo;
    char fechaStr[20];
    char *p;

    nuevo.id = generarNuevoIdPilotos(pilotos);
    limpiarPantalla();
    tituloSistema();
    tituloMenu("     ALTA DE PILOTO");

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

    do
    {
        printf("ID escuderia: ");
        scanf("%u", &nuevo.id_escuderia);
    }while(escuderiaValida(nuevo.id_escuderia, escuderias) != TODOOK);

    nuevo.puntos_acumulados = 0;
    nuevo.estado = 'A';
    getchar();
    do
    {
        printf("Fecha nacimiento (AAAAMMDD): ");
        fflush(stdin);
        fgets(fechaStr, sizeof(fechaStr), stdin);
        if((p = strchr(fechaStr, '\n')) != NULL)
            *p = '\0';
        else while(getchar() != '\n');

    }while (ValidarFecha(fechaStr) != TODOOK);
    sscanf(fechaStr, "%I64u", &nuevo.fechaNacimiento);

    fseek(pilotos,0,SEEK_END);
    fwrite(&nuevo,sizeof(t_piloto),1,pilotos);
    fflush(pilotos);

    printf("Piloto %s dado de alta con ID %u.\n", nuevo.nombre, nuevo.id);

    return TODOOK;
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
        return ERROR_;
    }

    ftxt = fopen(txtPath, "wt");
    if(!ftxt)
    {
        printf("Error al abrir %s\n", txtPath);
        fclose(fbin);
        return ERROR_;
    }

    while(fread(&p, sizeof(t_piloto), 1, fbin) == 1)
    {
        fprintf(ftxt, "%u;%s;%s;%u;%u;%c;%I64u\n", p.id, p.nombre, p.nacionalidad, p.id_escuderia, p.puntos_acumulados,
                                                    p.estado, p.fechaNacimiento);
    }

    fclose(fbin);
    fclose(ftxt);

    return TODOOK;
}

int bajaPiloto(FILE* pilotos)
{
    unsigned idbuscado;
    t_piloto pil;
    FILE* fbajas;
    int encontrado=0;

    limpiarPantalla();
    tituloSistema();
    tituloMenu("BAJA DE PILOTO");

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
                return ERROR_;
            }
            pil.estado='R';
            fseek(pilotos, -(long)sizeof(t_piloto),SEEK_CUR);
            fwrite(&pil,sizeof(t_piloto),1,pilotos);
            fflush(pilotos);

            fbajas = fopen(BAJAS_PILOTOS_DAT, "ab");
            if(!fbajas)
            {
                printf("Error al abrir %s\n", BAJAS_PILOTOS_DAT);
                return ERROR_ARCH;
            }
            else
            {
                fwrite(&pil, sizeof(t_piloto), 1, fbajas);
                fclose(fbajas);
                exportarBajasPilotosTxt(BAJAS_PILOTOS_DAT, BAJAS_PILOTOS_TXT); //Para leer los pilotos en el txt
            }

            printf("Piloto %s dado de baja.\n", pil.nombre);
        }
    }

    if(!encontrado)
        printf("Piloto no encontrado.\n");

    return encontrado ? TODOOK : ERROR_;
}

int modificarPiloto(FILE* pilotos, FILE* escuderias)
{
    unsigned idBuscado;
    t_piloto piloto;
    char fechaStr[20];
    unsigned nuevoIdEsc;
    int encontrado=0;
    char *p;

    limpiarPantalla();
    tituloSistema();
    tituloMenu("MODIFICAR PILOTO");
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
                else
                    while(getchar() != '\n');
            }

            printf("\nNacionalidad: %s\n", piloto.nacionalidad);
            if(confirmarModificacion("Modificar nacionalidad"))
            {
                getchar();
                printf("Nacionalidad: ");
                fgets(piloto.nacionalidad, sizeof(piloto.nacionalidad), stdin);
                if((p = strchr(piloto.nacionalidad, '\n')) != NULL)
                    *p = '\0';
                else
                    while(getchar() != '\n');
            }

            printf("\nID Escuderia: %u\n", piloto.id_escuderia);
            if(confirmarModificacion("Modificar escuderia"))
            {
                getchar();
                do
                {
                    printf("ID escuderia: ");
                    scanf("%u", &nuevoIdEsc);
                }while(escuderiaValida(nuevoIdEsc, escuderias) != TODOOK);
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
                do
                {
                    printf("Fecha nacimiento (AAAAMMDD): ");
                    fgets(fechaStr, sizeof(fechaStr), stdin);
                    if((p = strchr(fechaStr, '\n')) != NULL)
                        *p = '\0';
                    else
                        while(getchar() != '\n');
                }while (ValidarFecha(fechaStr) != TODOOK);
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

    return encontrado ? TODOOK : ERROR_;
}

void mostrarRanking(FILE* pilotos)
{
    tda_vector ranking;
    t_piloto pil;
    t_piloto* piloto_actual;
    int i;

    crear_Vector(&ranking,sizeof(t_piloto));
    rewind(pilotos);

    while(fread(&pil,sizeof(t_piloto),1,pilotos)==1)
    {
        if(pil.estado=='A'||pil.estado=='S')
            insertarAlFinal_Vector(&ranking,&pil);
    }

    sSort(ranking.vec,ranking.ce,ranking.tam,compararPuntos);

    limpiarPantalla();
    tituloSistema();
    tituloMenu("    CAMPEONATO DE PILOTOS");
    color(COLOR_MENU_PRINCIPAL);

    puts("-------------------------------------------------------");
    printf("| %-8s | %-30s | %-7s |\n","POSICIÓN", "PILOTO", "PUNTOS");
    puts("-------------------------------------------------------");
    restaurarColor();

    for(i=0; i<ranking.ce; i++)
    {
        piloto_actual = (t_piloto*)((char*)ranking.vec + (i*ranking.tam));
        printf("| %-8d | %-30s | %-4u pts|\n",i+1, piloto_actual->nombre, piloto_actual->puntos_acumulados);
    }
    color(COLOR_MENU_PRINCIPAL);
    printf("-------------------------------------------------------\n");
    restaurarColor();
    destruir_Vector(&ranking);
}

void mostrarPilotoCarrera(void* pilotos)
{
    t_piloto* pil = (t_piloto*)pilotos;
    if(pil->estado == 'A' || pil->estado == 'S')
    {
        printf("| %-8u | %-30s |\n",pil->id,pil->nombre);
    }
}

void listarPilotosPorEscuderia(FILE* pilotos, FILE* escuderias)
{
    int ce, cp, i, j;
    t_escuderia escu;
    t_piloto pil;

    fseek(escuderias, 0, SEEK_END);
    ce = ftell(escuderias) / sizeof(t_escuderia);

    fseek(pilotos, 0, SEEK_END);
    cp = ftell(pilotos) / sizeof(t_piloto);

    limpiarPantalla();
    tituloSistema();
    tituloMenu("  PILOTOS POR ESCUDERIA");

    for(i = 0; i < ce; i++)
    {
        fseek(escuderias, i * sizeof(t_escuderia), SEEK_SET);
        fread(&escu, sizeof(t_escuderia), 1, escuderias);

        color(COLOR_MENU_PRINCIPAL);
        printf("\n-----------------------------------------------------\n");
        printf("\t\t%s\n", escu.nombre);
        printf("-----------------------------------------------------\n");
        printf("| %-3s | %-30s | %6s | %s |\n", "ID", "NOMBRE", "PUNTOS", "E");
        printf("-----------------------------------------------------\n");
        restaurarColor();

        for(j = 0; j < cp; j++)
        {
            fseek(pilotos, j * sizeof(t_piloto), SEEK_SET);
            fread(&pil, sizeof(t_piloto), 1, pilotos);

            if(pil.id_escuderia == escu.id &&(pil.estado == 'A' || pil.estado == 'S'))
            {
                printf("| %-3u | %-30s | %6u | %c |\n",
                       pil.id,
                       pil.nombre,
                       pil.puntos_acumulados,
                       pil.estado);
            }
        }
        color(COLOR_MENU_PRINCIPAL);
        printf("-----------------------------------------------------\n");
        restaurarColor();
    }
}
