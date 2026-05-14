#include "pilotos.h"
#include <ctype.h>


void listarPilotos(const char *binPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbin;
    int contador;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return;
    }

    printf("\n===== LISTADO DE PILOTOS =====\n");
    contador = 0;
    while (fread(piloto, sizeof(t_piloto), 1, fbin) == 1)
    {
        printf("ID: %u | Nombre: %s | Nac: %s | Escuderia: %u | Puntos: %u | Estado: %c\n",
               piloto->id,
               piloto->nombre,
               piloto->nacionalidad,
               piloto->id_escuderia,
               piloto->puntos_acumulados,
               piloto->estado);
        contador++;
    }
    printf("Total: %d pilotos\n", contador);

    fclose(fbin);
}


static long buscarPilotoPorId(FILE *fbin, unsigned id, t_piloto *piloto)
{
    long pos;

    rewind(fbin);
    while (fread(piloto, sizeof(t_piloto), 1, fbin) == 1)
    {
        if (piloto->id == id)
        {
            pos = ftell(fbin) - (long)sizeof(t_piloto);
            return pos;
        }
    }
    return -1;
}


int altaPiloto(const char *binPath)
{
    t_piloto p1;
    t_piloto aux1;
    t_piloto *piloto = &p1;
    t_piloto *aux = &aux1;
    FILE *fbin;
    long pos;

    printf("\n--- ALTA DE PILOTO ---\n");
    printf("ID: ");
    scanf("%u", &piloto->id);

    fbin = fopen(binPath, "r+b");
    if (fbin == NULL)
    {
        fbin = fopen(binPath, "wb");
        if (fbin == NULL)
        {
            printf("Error al abrir %s\n", binPath);
            return ERROR;
        }
    }
    else
    {
        pos = buscarPilotoPorId(fbin, piloto->id, aux);
        if (pos != -1)
        {
            printf("Error: ya existe un piloto con ID %u.\n", piloto->id);
            fclose(fbin);
            return ERROR;
        }
    }

    printf("Nombre: ");
    scanf(" %29[^\n]", piloto->nombre);
    printf("Nacionalidad: ");
    scanf(" %29[^\n]", piloto->nacionalidad);
    printf("ID Escuderia: ");
    scanf("%u", &piloto->id_escuderia);
    printf("Puntos acumulados: ");
    scanf("%u", &piloto->puntos_acumulados);
    printf("Estado (A/R/S): ");
    scanf(" %c", &piloto->estado);
    printf("Fecha de nacimiento (AAAAMMDD): ");
    scanf("%I64u", &piloto->fechaNacimiento);

    fseek(fbin, 0, SEEK_END);
    fwrite(piloto, sizeof(t_piloto), 1, fbin);

    printf("Piloto agregado correctamente.\n");

    fclose(fbin);
    return OK;
}


int bajaPiloto(const char *binPath, const char *bajasPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbin;
    FILE *fbajas;
    unsigned id;
    long pos;

    fbin = fopen(binPath, "r+b");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR;
    }

    printf("\n--- BAJA DE PILOTO ---\n");
    printf("ID del piloto a dar de baja: ");
    scanf("%u", &id);

    pos = buscarPilotoPorId(fbin, id, piloto);
    if (pos == -1)
    {
        printf("Error: no se encontro un piloto con ID %u.\n", id);
        fclose(fbin);
        return ERROR;
    }

    if (piloto->estado == 'R')
    {
        printf("El piloto ya esta dado de baja.\n");
        fclose(fbin);
        return ERROR;
    }
    // Marcar como retirado y sobreescribir
    piloto->estado = 'R';

    // Registrar en archivo de bajas
    fbajas = fopen(bajasPath, "ab");
    if (fbajas == NULL)
    {
        printf("Error al abrir %s\n", bajasPath);
        fclose(fbin);
        return ERROR;
    }
    fwrite(piloto, sizeof(t_piloto), 1, fbajas);
    fclose(fbajas);


    fseek(fbin, pos, SEEK_SET);
    fwrite(piloto, sizeof(t_piloto), 1, fbin);

    printf("Piloto %s dado de baja correctamente.\n", piloto->nombre);

    fclose(fbin);
    return OK;
}


int confirmarModificacion(const char *mensaje)
{
    char opcion;

    do
    {
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

int modificarPiloto(const char *binPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbin;
    unsigned id;
    long pos;

    fbin = fopen(binPath, "r+b");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR;
    }

    printf("\n--- MODIFICAR PILOTO ---\n");
    printf("ID del piloto a modificar: ");
    scanf("%u", &id);

    pos = buscarPilotoPorId(fbin, id, piloto);
    if (pos == -1)
    {
        printf("Error: no se encontro un piloto con ID %u.\n", id);
        fclose(fbin);
        return ERROR;
    }

    printf("Piloto encontrado: %s\n", piloto->nombre);
    if(confirmarModificacion("Modificar nombre?"))
    {
        printf("Nuevo nombre: ");
        scanf(" %29[^\n]", piloto->nombre);
    }

    if(confirmarModificacion("Modificar nacionalidad?"))
    {
        printf("Nueva nacionalidad: ");
        scanf(" %29[^\n]", piloto->nacionalidad);
    }

    if(confirmarModificacion("Modificar escuderia?"))
    {
        printf("Nuevo ID escuderia: ");
        scanf("%u", &piloto->id_escuderia);
    }

    if(confirmarModificacion("Modificar puntos?"))
    {
        printf("Nuevos puntos: ");
        scanf("%u", &piloto->puntos_acumulados);
    }

    if(confirmarModificacion("Modificar estado?"))
    {
        printf("Nuevo estado (A/R/S): ");
        scanf(" %c", &piloto->estado);
    }

    fseek(fbin, pos, SEEK_SET);
    fwrite(piloto, sizeof(t_piloto), 1, fbin);

    printf("Piloto modificado correctamente.\n");

    fclose(fbin);
    return OK;
}


void listarAltas(const char *binPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbin;
    int contador;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return;
    }

    printf("\n===== PILOTOS ACTIVOS =====\n");
    contador = 0;
    while (fread(piloto, sizeof(t_piloto), 1, fbin) == 1)
    {
        if (piloto->estado == 'A')
        {
            printf("ID: %u | Nombre: %s | Nac: %s | Escuderia: %u | Puntos: %u\n",
                   piloto->id,
                   piloto->nombre,
                   piloto->nacionalidad,
                   piloto->id_escuderia,
                   piloto->puntos_acumulados);
            contador++;
        }
    }
    printf("Total: %d pilotos activos\n", contador);

    fclose(fbin);
}


void listarBajas(const char *bajasPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbajas;
    int contador;

    fbajas = fopen(bajasPath, "rb");
    if (fbajas == NULL)
    {
        printf("No hay registros de bajas.\n");
        return;
    }

    printf("\n===== PILOTOS DADOS DE BAJA =====\n");
    contador = 0;
    while (fread(piloto, sizeof(t_piloto), 1, fbajas) == 1)
    {
        printf("ID: %u | Nombre: %s | Nac: %s | Escuderia: %u | Puntos: %u | Estado: %c\n",
               piloto->id,
               piloto->nombre,
               piloto->nacionalidad,
               piloto->id_escuderia,
               piloto->puntos_acumulados,
               piloto->estado);
        contador++;
    }
    printf("Total: %d pilotos dados de baja\n", contador);

    fclose(fbajas);
}


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

/*void __menuPilotos()
{
    int op;
    unsigned int idEsc;
    unsigned int idPil;
    do{
        printf("\n--- PILOTOS ---\n");
        printf("1. Listar pilotos\n");
        printf("2. Alta piloto\n");
        printf("3. Baja piloto\n");
        printf("4. Modificar piloto\n");
        printf("5. Buscar piloto por ID\n");
        printf("6. Mostrar ranking\n");
        printf("7. Pilotos por escudería\n");
        printf("8. Exportar pilotos\n");
        printf("0. Volver\n");
        op = leerEntero("Opción: ");

        switch (op) {
            case 1: _listarPilotos(PILOTOS_DAT); break;
            case 2: _altaPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
            case 3: _bajaPiloto(PILOTOS_DAT, PILOTOS_IDX, BAJAS_PILOTOS); break;
            case 4: _modificarPiloto(PILOTOS_DAT, PILOTOS_IDX); break;
            case 5: {
                FILE *fp;
                Piloto p;
                idPil = (unsigned int)leerEntero("ID piloto: ");
                fp = fopen(PILOTOS_DAT, "rb");
                if (fp) {
                    if (buscarPilotoPorId(fp, idPil, &p))
                        printf("Encontrado: [%u] %s - %s - Pts:%u - Estado:%c\n",
                               p.id, p.nombre, p.nacionalidad,
                               p.puntos_acumulados, p.estado);
                    else
                        printf("No encontrado.\n");
                    fclose(fp);
                }
                break;
            }
            case 6: mostrarRanking(PILOTOS_DAT); break;
            case 7:
                idEsc = (unsigned int)leerEntero("ID escuderia: ");
                listarPilotosPorEscuderia(PILOTOS_DAT, idEsc);
                break;
            case 8: exportarPilotos(PILOTOS_DAT, EXP_PILOTOS); break;
            case 0: break;
            default: printf("Opcion invalida.\n");
        }
    }while(op != 0);
}*/
